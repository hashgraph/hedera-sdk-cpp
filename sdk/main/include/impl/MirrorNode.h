/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
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
#ifndef HEDERA_SDK_CPP_IMPL_MIRROR_NODE_H_
#define HEDERA_SDK_CPP_IMPL_MIRROR_NODE_H_

#include <proto/mirror/consensus_service.grpc.pb.h>

#include "Defaults.h"
#include "impl/NodeAddress.h"
#include "impl/TLSBehavior.h"

#include <functional>
#include <grpcpp/channel.h>
#include <grpcpp/security/credentials.h>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace grpc
{
class ClientContext;
class Status;
}

namespace proto
{
class Query;
class Response;
class Transaction;
class TransactionResponse;
}

namespace Hedera::internal
{
/**
 * Internal utility class used to represent a node on a Hedera network.
 */
class MirrorNode
{
public:
  /**
   * Construct this MirrorNode from a string which contains the IP and port with which to connect.
   *
   * @param address The address to which to connect.
   */
  explicit MirrorNode(std::string_view address);

  /**
   * Attempt to connect this MirrorNode to the remote mirror node.
   *
   * @param timeout The point in time that the attempted connection will cease and be considered a failure.
   * @return \c TRUE if the Node is already connected or successfully connected, otherwise \c FALSE.
   */
  bool connect(const std::chrono::system_clock::time_point& timeout);

  /**
   * Shutdown the connection from this MirrorNode to the remote mirror node.
   */
  void shutdown();

  /**
   * Set the minimum amount of time this Node should wait before being willing to resubmit a previously failed request
   * to its remote node.
   *
   * @param backoff The minimum backoff time.
   */
  void setMinBackoff(const std::chrono::duration<double>& backoff);

  /**
   * Set the maximum amount of time this Node should wait before being willing to resubmit a previously failed request
   * to its remote node.
   *
   * @param backoff The maximum backoff time.
   */
  void setMaxBackoff(const std::chrono::duration<double>& backoff);

  /**
   * Determine if this Node is "healthy", meaning this Node has received a gRPC status responses from its remote node
   * that indicates a successful request attempt, or if it has fully backed off for its full period of backoff time
   * after an unsuccessful submission attempt.
   *
   * @return \c TRUE if this Node is "healthy", otherwise \c FALSE.
   */
  [[nodiscard]] bool isHealthy() const;

  /**
   * Increase the backoff of this Node. This should be called when this Node either fails to connect to its remote node
   * or it receives a gRPC status from its remote node that indicates a failed submission attempt.
   */
  void increaseBackoff();

  /**
   * Decrease the backoff of this Node. This should be called when this Node successfully submits a request to its
   * remote node.
   */
  void decreaseBackoff();

  /**
   * Get the amount of time remaining in this Node's current backoff.
   *
   * @return The amount of time that should pass before this Node is good to try and submit another request.
   */
  [[nodiscard]] std::chrono::duration<double> getRemainingTimeForBackoff() const;

  /**
   * Get the consensus service stub.
   *
   * @return The consensus service stub.
   */
  [[nodiscard]] inline std::shared_ptr<com::hedera::mirror::api::proto::ConsensusService::Stub>
  getConsensusServiceStub() const
  {
    return mConsensusStub;
  }

private:
  /**
   * Initialize the gRPC channel used to communicate with this Node's remote node based on mTLSBehavior.
   *
   * @param deadline The deadline to connect. Processing will continue trying until this point to establish a connection
   *                 and will return \c FALSE if not established by then.
   * @return \c TRUE if initialization was successful, otherwise \c FALSE.
   */
  bool initializeChannel(const std::chrono::system_clock::time_point& deadline);

  /**
   * The address of the MirrorNode.
   */
  std::string mAddress;

  /**
   * Pointer to this Node's TLS channel credentials. This only depends on the NodeAddress so this will be updated
   * whenever mAddress is.
   */
  std::shared_ptr<grpc::ChannelCredentials> mTlsChannelCredentials = nullptr;

  /**
   * Pointer to the gRPC channel used to communicate with the gRPC server living on the remote node.
   */
  std::shared_ptr<grpc::Channel> mChannel = nullptr;

  /**
   * Pointer to the gRPC stub used to communicate with the consensus service living on the remote mirror node.
   */
  std::shared_ptr<com::hedera::mirror::api::proto::ConsensusService::Stub> mConsensusStub = nullptr;

  /**
   * The point in time that this Node would be considered healthy again.
   */
  std::chrono::system_clock::time_point mReadmitTime = std::chrono::system_clock::now();

  /**
   * The minimum length of time this Node should wait before being willing to attempt to send a failed request to its
   * remote node again.
   */
  std::chrono::duration<double> mMinBackoff = DEFAULT_MIN_BACKOFF;

  /**
   * The maximum length of time this Node should wait before being willing to attempt to send a failed request to its
   * remote node again.
   */
  std::chrono::duration<double> mMaxBackoff = DEFAULT_MAX_BACKOFF;

  /**
   * The current backoff time. Every failed submission attempt waits a certain amount of time that is double the
   * previous amount of time a request waited for its previous submission attempt, up to the specified maximum backoff
   * time, at which point the execution is considered a failure.
   */
  std::chrono::duration<double> mCurrentBackoff = mMinBackoff;

  /**
   * Is the gRPC channel being utilized by this Node to communicate with its remote node initialized?
   */
  bool mIsInitialized = false;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_MIRROR_NODE_H_
