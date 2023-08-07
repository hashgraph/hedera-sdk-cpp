/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "TopicMessageQuery.h"
#include "Client.h"
#include "Defaults.h"
#include "SubscriptionHandle.h"
#include "TopicId.h"
#include "TopicMessage.h"
#include "impl/MirrorNetwork.h"
#include "impl/MirrorNode.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <chrono>
#include <grpcpp/grpcpp.h>
#include <proto/mirror/consensus_service.grpc.pb.h>
#include <thread>
#include <unordered_map>

namespace Hedera
{
namespace
{
// Enum to track the status of the gRPC call.
enum CallStatus
{
  STATUS_CREATE = 0,
  STATUS_PROCESSING = 1,
  STATUS_FINISH = 2
};

// Helper function used to get a connected mirror node.
std::shared_ptr<internal::MirrorNode> getConnectedMirrorNode(const std::shared_ptr<internal::MirrorNetwork>& network)
{
  std::shared_ptr<internal::MirrorNode> node = network->getNextMirrorNode();
  std::cout << "connecting..." << std::endl;
  while (!node->connect(std::chrono::system_clock::now() + std::chrono::seconds(2)))
  {
    std::cout << "retry" << std::endl;
    node = network->getNextMirrorNode();
  }

  return node;
}

// Subscriber function called by TopicMessageQuery::subscribe().
void startSubscription(const std::shared_ptr<internal::MirrorNetwork>& network,
                       com::hedera::mirror::api::proto::ConsensusTopicQuery query,
                       std::function<void(grpc::Status)> errorHandler,
                       std::function<bool(grpc::Status)> retryHandler,
                       std::function<void(void)> completionHandler,
                       std::function<void(const TopicMessage&)> onNext,
                       uint32_t maxAttempts,
                       std::chrono::duration<double> maxBackoff,
                       SubscriptionHandle* handle)
{
  // Set up the handle to allow it to unsubscribe.
  grpc::ClientContext context;
  handle->setOnUnsubscribe([&context]() { context.TryCancel(); });

  // Read responses as they arrive.
  com::hedera::mirror::api::proto::ConsensusTopicResponse response;
  std::unordered_map<TransactionId, std::vector<com::hedera::mirror::api::proto::ConsensusTopicResponse>>
    pendingMessages;
  grpc::CompletionQueue completionQueue;
  grpc::Status grpcStatus;
  CallStatus callStatus = CallStatus::STATUS_CREATE;
  uint64_t attempt = 0ULL;
  bool ok = false;
  void* tag;

  // Send the query.
  std::cout << "Sending query with topic id: " << TopicId::fromProtobuf(query.topicid()).toString() << std::endl;
  auto reader = getConnectedMirrorNode(network)->getConsensusServiceStub()->AsyncsubscribeTopic(
    &context, query, &completionQueue, &callStatus);

  // Loop as long as there are valid read operations.
  while (completionQueue.Next(&tag, &ok))
  {
    std::cout << "loop" << std::endl;
    const auto receivedTagStatus = static_cast<CallStatus>(*internal::Utilities::toTypePtr<long>(tag));
    std::cout << "Received tag " << receivedTagStatus << std::endl;
    switch (receivedTagStatus)
    {
      case CallStatus::STATUS_CREATE:
      {
        std::cout << "received create tag" << std::endl;
        if (ok)
        {
          // Update the call status to processing.
          callStatus = CallStatus::STATUS_PROCESSING;
        }

        // Let fall through to process message.
        [[fallthrough]];
      }
      case CallStatus::STATUS_PROCESSING:
      {
        std::cout << "received processing tag" << std::endl;
        if (ok)
        {
          std::cout << "response is ok" << std::endl;
          // Read the response.
          reader->Read(&response, &callStatus);
          std::cout << "contents: " << response.message() << std::endl;

          // Adjust the query timestamp and limit, in case a retry is triggered.
          if (response.has_consensustimestamp())
          {
            std::cout << "has_consensustimestamp" << std::endl;
            std::chrono::system_clock::time_point timestamp =
              internal::TimestampConverter::fromProtobuf(response.consensustimestamp());

            // Add one of the smallest denomination of time this machine can handle.
            timestamp += std::chrono::duration<int, std::ratio<1, std::chrono::system_clock::period::den>>();
            query.set_allocated_consensusstarttime(internal::TimestampConverter::toProtobuf(timestamp));
          }

          if (query.limit() > 0ULL)
          {
            std::cout << "limit" << std::endl;
            query.set_limit(query.limit() - 1ULL);
          }

          // Process the received message.
          if (!response.has_chunkinfo() || response.chunkinfo().total() == 1)
          {
            std::cout << "!response.has_chunkinfo() || response.chunkinfo().total() == 1" << std::endl;
            std::cout << "calling onNext for one chunk" << std::endl;
            onNext(TopicMessage::ofSingle(response));
            std::cout << "done calling onNext for one chunk" << std::endl;
          }
          else
          {
            std::cout << "not !response.has_chunkinfo() || response.chunkinfo().total() == 1" << std::endl;
            const TransactionId transactionId =
              TransactionId::fromProtobuf(response.chunkinfo().initialtransactionid());
            pendingMessages[transactionId].push_back(response);

            if (pendingMessages[transactionId].size() == response.chunkinfo().total())
            {
              std::cout << "calling onNext for multiple chunks" << std::endl;
              onNext(TopicMessage::ofMany(pendingMessages[transactionId]));
            }
          }
        }
        else
        {
          std::cout << "response is not ok" << std::endl;
          // If there was an error, finish the RPC.
          callStatus = CallStatus::STATUS_FINISH;
          reader->Finish(&grpcStatus, &callStatus);
        }

        break;
      }
      case CallStatus::STATUS_FINISH:
      {
        std::cout << "received finish tag" << std::endl;
        std::cout << grpcStatus.error_code() << std::endl;
        if (grpcStatus.ok())
        {
          // RPC completed successfully.
          completionHandler();
        }
        else if (attempt < maxAttempts && retryHandler(grpcStatus))
        {
          handle->unsubscribe();

          // RPC encountered an error, so backoff.
          const std::chrono::duration<double> backoff = std::chrono::milliseconds(250) * pow(2.0, attempt);
          std::this_thread::sleep_for(backoff < maxBackoff ? backoff : maxBackoff);
          ++attempt;

          // Resend the query.
          reader = getConnectedMirrorNode(network)->getConsensusServiceStub()->AsyncsubscribeTopic(
            &context, query, &completionQueue, tag);
        }
        else
        {
          handle->unsubscribe();
          errorHandler(grpcStatus);
        }

        break;
      }
      default:
      {
        std::cout << "Received unknown tag: " << *internal::Utilities::toTypePtr<long>(tag) << std::endl;
        break;
      }
    }
  }

  // Done processing responses.
  completionQueue.Shutdown();
}

} // namespace

// Implementation object for TopicMessageQuery.
struct TopicMessageQuery::TopicMessageQueryImpl
{
  // The message query to be sent.
  com::hedera::mirror::api::proto::ConsensusTopicQuery mQuery;

  // The maximum amount of attempts to try and retrieve messages.
  uint32_t mMaxAttempts = DEFAULT_MAX_ATTEMPTS;

  // The maximum amount of time to wait between submission attempts.
  std::chrono::duration<double> mMaxBackoff = DEFAULT_MAX_BACKOFF;

  // The function to run when there's an error.
  std::function<void(grpc::Status)> mErrorHandler = [](const grpc::Status& status)
  { std::cout << "Failed to subscribe to topic with error: " << status.error_message() << std::endl; };

  // The function to run when a retry is required.
  std::function<bool(grpc::Status)> mRetryHandler = [](const grpc::Status& status)
  {
    return (status.error_code() == grpc::StatusCode::NOT_FOUND) ||
           (status.error_code() == grpc::StatusCode::RESOURCE_EXHAUSTED) ||
           (status.error_code() == grpc::StatusCode::UNAVAILABLE) ||
           (status.error_code() == grpc::StatusCode::INTERNAL);
  };

  // The function to run when streaming is complete.
  std::function<void(void)> mCompletionHandler = [this]()
  {
    std::cout << "RPC subscription to topic " << TopicId::fromProtobuf(mQuery.topicid()).toString() << " is complete"
              << std::endl;
  };
};

//-----
TopicMessageQuery::TopicMessageQuery()
  : mImpl(std::make_unique<TopicMessageQueryImpl>())
{
}

//-----
TopicMessageQuery::~TopicMessageQuery() = default;

//-----
TopicMessageQuery::TopicMessageQuery(const TopicMessageQuery& other)
  : TopicMessageQuery()
{
  mImpl->mQuery = other.mImpl->mQuery;
  mImpl->mMaxAttempts = other.mImpl->mMaxAttempts;
  mImpl->mMaxBackoff = other.mImpl->mMaxBackoff;
}

//-----
TopicMessageQuery& TopicMessageQuery::operator=(const TopicMessageQuery& other)
{
  if (this != &other)
  {
    mImpl->mQuery = other.mImpl->mQuery;
    mImpl->mMaxAttempts = other.mImpl->mMaxAttempts;
    mImpl->mMaxBackoff = other.mImpl->mMaxBackoff;
  }

  return *this;
}

//-----
TopicMessageQuery::TopicMessageQuery(TopicMessageQuery&& other) noexcept
  : mImpl(std::move(other.mImpl))
{
  // Leave moved-from TopicMessageQuery in valid state.
  other.mImpl = std::make_unique<TopicMessageQueryImpl>();
}

//-----
TopicMessageQuery& TopicMessageQuery::operator=(TopicMessageQuery&& other) noexcept
{
  if (this != &other)
  {
    mImpl = std::move(other.mImpl);

    // Leave moved-from TopicMessageQuery in valid state.
    other.mImpl = std::make_unique<TopicMessageQueryImpl>();
  }

  return *this;
}

//-----
SubscriptionHandle TopicMessageQuery::subscribe(const Client& client,
                                                const std::function<void(const TopicMessage&)>& onNext)
{
  // Create the subscription handle and assign its unsubscribe function to cancel this TopicMessageQuery's context
  // (which will cancel the gRPC call).
  SubscriptionHandle handle;

  // Initiate the subscription.
  std::thread(&startSubscription,
              client.getMirrorNetwork(),
              mImpl->mQuery,
              mImpl->mErrorHandler,
              mImpl->mRetryHandler,
              mImpl->mCompletionHandler,
              onNext,
              mImpl->mMaxAttempts,
              mImpl->mMaxBackoff,
              &handle)
    .detach();

  return handle;
}

//-----
TopicMessageQuery& TopicMessageQuery::setTopicId(const TopicId& topicId)
{
  std::cout << "Setting topic id: " << topicId.toString() << std::endl;
  mImpl->mQuery.set_allocated_topicid(topicId.toProtobuf().release());
  return *this;
}

//-----
TopicMessageQuery& TopicMessageQuery::setStartTime(const std::chrono::system_clock::time_point& start)
{
  mImpl->mQuery.set_allocated_consensusstarttime(internal::TimestampConverter::toProtobuf(start));
  return *this;
}

//-----
TopicMessageQuery& TopicMessageQuery::setEndTime(const std::chrono::system_clock::time_point& end)
{
  mImpl->mQuery.set_allocated_consensusendtime(internal::TimestampConverter::toProtobuf(end));
  return *this;
}

//-----
TopicMessageQuery& TopicMessageQuery::setLimit(uint64_t limit)
{
  mImpl->mQuery.set_limit(limit);
  return *this;
}

//-----
TopicMessageQuery& TopicMessageQuery::setMaxAttempts(uint32_t attempts)
{
  mImpl->mMaxAttempts = attempts;
  return *this;
}

//-----
TopicMessageQuery& TopicMessageQuery::setMaxBackoff(const std::chrono::duration<double>& backoff)
{
  mImpl->mMaxBackoff = backoff;
  return *this;
}

//-----
TopicMessageQuery& TopicMessageQuery::setErrorHandler(const std::function<void(grpc::Status)>& func)
{
  mImpl->mErrorHandler = func;
  return *this;
}

//-----
TopicMessageQuery& TopicMessageQuery::setRetryHandler(const std::function<bool(grpc::Status)>& func)
{
  mImpl->mRetryHandler = func;
  return *this;
}

//-----
TopicMessageQuery& TopicMessageQuery::setCompletionHandler(const std::function<void()>& func)
{
  mImpl->mCompletionHandler = func;
  return *this;
}

//-----
TopicId TopicMessageQuery::getTopicId() const
{
  return TopicId::fromProtobuf(mImpl->mQuery.topicid());
}

//-----
std::chrono::system_clock::time_point TopicMessageQuery::getStartTime() const
{
  return internal::TimestampConverter::fromProtobuf(mImpl->mQuery.consensusstarttime());
}

//-----
std::chrono::system_clock::time_point TopicMessageQuery::getEndTime() const
{
  return internal::TimestampConverter::fromProtobuf(mImpl->mQuery.consensusendtime());
}

//-----
uint64_t TopicMessageQuery::getLimit() const
{
  return mImpl->mQuery.limit();
}

//-----
uint32_t TopicMessageQuery::getMaxAttempts() const
{
  return mImpl->mMaxAttempts;
}

//-----
std::chrono::duration<double> TopicMessageQuery::getMaxBackoff() const
{
  return mImpl->mMaxBackoff;
}

} // namespace Hedera
