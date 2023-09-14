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
#include "Executable.h"
#include "AccountAllowanceApproveTransaction.h"
#include "AccountAllowanceDeleteTransaction.h"
#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "AccountInfo.h"
#include "AccountInfoQuery.h"
#include "AccountRecords.h"
#include "AccountRecordsQuery.h"
#include "AccountUpdateTransaction.h"
#include "Client.h"
#include "ContractByteCodeQuery.h"
#include "ContractCallQuery.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractExecuteTransaction.h"
#include "ContractFunctionResult.h"
#include "ContractInfo.h"
#include "ContractInfoQuery.h"
#include "ContractUpdateTransaction.h"
#include "EthereumTransaction.h"
#include "FileAppendTransaction.h"
#include "FileContentsQuery.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileInfo.h"
#include "FileInfoQuery.h"
#include "FileUpdateTransaction.h"
#include "FreezeTransaction.h"
#include "ScheduleCreateTransaction.h"
#include "ScheduleDeleteTransaction.h"
#include "ScheduleInfo.h"
#include "ScheduleInfoQuery.h"
#include "ScheduleSignTransaction.h"
#include "SystemDeleteTransaction.h"
#include "SystemUndeleteTransaction.h"
#include "TokenAssociateTransaction.h"
#include "TokenBurnTransaction.h"
#include "TokenCreateTransaction.h"
#include "TokenDeleteTransaction.h"
#include "TokenDissociateTransaction.h"
#include "TokenFeeScheduleUpdateTransaction.h"
#include "TokenFreezeTransaction.h"
#include "TokenGrantKycTransaction.h"
#include "TokenInfo.h"
#include "TokenInfoQuery.h"
#include "TokenMintTransaction.h"
#include "TokenNftInfo.h"
#include "TokenNftInfoQuery.h"
#include "TokenPauseTransaction.h"
#include "TokenRevokeKycTransaction.h"
#include "TokenUnfreezeTransaction.h"
#include "TokenUnpauseTransaction.h"
#include "TokenUpdateTransaction.h"
#include "TokenWipeTransaction.h"
#include "TopicCreateTransaction.h"
#include "TopicDeleteTransaction.h"
#include "TopicInfo.h"
#include "TopicInfoQuery.h"
#include "TopicMessageSubmitTransaction.h"
#include "TopicUpdateTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionReceiptQuery.h"
#include "TransactionRecord.h"
#include "TransactionRecordQuery.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "exceptions/IllegalStateException.h"
#include "exceptions/MaxAttemptsExceededException.h"
#include "exceptions/PrecheckStatusException.h"
#include "impl/Network.h"
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <algorithm>
#include <grpcpp/impl/codegen/status.h>
#include <limits>
#include <proto/query.pb.h>
#include <proto/response.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_response.pb.h>
#include <stdexcept>
#include <thread>

namespace Hedera
{
//-----
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
SdkResponseType Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::execute(
  const Client& client)
{
  return execute(client, client.getRequestTimeout());
}

//-----
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
SdkResponseType Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::execute(
  const Client& client,
  const std::chrono::duration<double>& timeout)
{
  setExecutionParameters(client);
  onExecute(client);

  // Get the nodes associated with this Executable's node account IDs.
  const std::vector<std::shared_ptr<internal::Node>> nodes = getNodesFromNodeAccountIds(client);

  // The time to timeout.
  const std::chrono::system_clock::time_point timeoutTime =
    std::chrono::system_clock::now() + std::chrono::duration_cast<std::chrono::system_clock::duration>(timeout);

  for (unsigned int attempt = 0U;; ++attempt)
  {
    if (attempt > mCurrentMaxAttempts)
    {
      throw MaxAttemptsExceededException("Max number of attempts made (max attempts allowed: " +
                                         std::to_string(mCurrentMaxAttempts));
    }

    const unsigned int nodeIndex = getNodeIndexForExecute(nodes, attempt);
    const std::shared_ptr<internal::Node>& node = nodes.at(nodeIndex);

    // If the returned node is not healthy, then no nodes are healthy and the returned node has the shortest remaining
    // delay. Sleep for the delay period.
    if (!node->isHealthy())
    {
      std::this_thread::sleep_for(node->getRemainingTimeForBackoff());
    }

    // Make sure the Node is connected. If it can't connect, mark this Node as unhealthy and try another Node.
    if (node->channelFailedToConnect())
    {
      node->increaseBackoff();
      continue;
    }

    // Create the request based on the index of the node being used.
    const ProtoRequestType request = makeRequest(nodeIndex);

    // Submit the request and get the response.
    ProtoResponseType response;
    const grpc::Status status = submitRequest(request, node, timeoutTime, &response);

    // Increase backoff for this node but try submitting again for UNAVAILABLE, RESOURCE_EXHAUSTED, and INTERNAL
    // responses.
    if (const grpc::StatusCode errorCode = status.error_code(); errorCode == grpc::StatusCode::UNAVAILABLE ||
                                                                errorCode == grpc::StatusCode::RESOURCE_EXHAUSTED ||
                                                                errorCode == grpc::StatusCode::INTERNAL)
    {
      node->increaseBackoff();
      continue;
    }

    // Successful submission, so decrease backoff for this node.
    node->decreaseBackoff();

    const Status responseStatus = mapResponseStatus(response);
    switch (determineStatus(responseStatus, client, response))
    {
      case ExecutionStatus::SERVER_ERROR:
      {
        continue;
      }
      // Response isn't ready yet from the network
      case ExecutionStatus::RETRY:
      {
        std::this_thread::sleep_for(mCurrentBackoff);
        mCurrentBackoff *= 2;
        break;
      }
      case ExecutionStatus::REQUEST_ERROR:
      {
        throw PrecheckStatusException(gStatusToString.at(responseStatus));
      }
      default:
      {
        return mapResponse(response);
      }
    }
  }
}

//-----
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
SdkRequestType& Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::setNodeAccountIds(
  const std::vector<AccountId>& nodeAccountIds)
{
  mNodeAccountIds = nodeAccountIds;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
SdkRequestType& Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::setMaxAttempts(
  uint32_t attempts)
{
  mMaxAttempts = attempts;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
SdkRequestType& Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::setMinBackoff(
  const std::chrono::duration<double>& backoff)
{
  if ((mMaxBackoff && backoff > *mMaxBackoff) || (!mMaxBackoff && backoff > DEFAULT_MAX_BACKOFF))
  {
    throw std::invalid_argument("Minimum backoff would be larger than maximum backoff");
  }

  mMinBackoff = backoff;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
SdkRequestType& Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::setMaxBackoff(
  const std::chrono::duration<double>& backoff)
{
  if ((mMinBackoff && backoff < *mMinBackoff) || (!mMinBackoff && backoff < DEFAULT_MIN_BACKOFF))
  {
    throw std::invalid_argument("Maximum backoff would be smaller than minimum backoff");
  }

  mMaxBackoff = backoff;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
typename Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::ExecutionStatus
Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::determineStatus(
  Status status,
  const Client&,
  const ProtoResponseType&)
{
  switch (status)
  {
    case Status::PLATFORM_TRANSACTION_NOT_CREATED:
    case Status::PLATFORM_NOT_ACTIVE:
    case Status::BUSY:
      return ExecutionStatus::SERVER_ERROR;
    case Status::OK:
    case Status::SUCCESS:
      return ExecutionStatus::SUCCESS;
      // Let derived class handle this status, assume request error
    default:
      return ExecutionStatus::REQUEST_ERROR;
  }
}

//-----
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
void Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::setExecutionParameters(
  const Client& client)
{
  mCurrentMaxAttempts = mMaxAttempts              ? *mMaxAttempts
                        : client.getMaxAttempts() ? *client.getMaxAttempts()
                                                  : DEFAULT_MAX_ATTEMPTS;
  mCurrentMinBackoff = mMinBackoff              ? *mMinBackoff
                       : client.getMinBackoff() ? *client.getMinBackoff()
                                                : DEFAULT_MIN_BACKOFF;
  mCurrentMaxBackoff = mMaxBackoff              ? *mMaxBackoff
                       : client.getMaxBackoff() ? *client.getMaxBackoff()
                                                : DEFAULT_MAX_BACKOFF;
  mCurrentBackoff = mCurrentMinBackoff;
}

//-----
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
std::vector<std::shared_ptr<internal::Node>>
Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::getNodesFromNodeAccountIds(
  const Client& client) const
{
  std::vector<std::shared_ptr<internal::Node>> nodes;

  // If only a single node is explicitly set, still return all the proxies for that node. It's possible the node itself
  // still works but something could be wrong with the proxy, in which case trying a different proxy would work.
  if (mNodeAccountIds.size() == 1)
  {
    nodes = client.getNetwork()->getNodeProxies(*mNodeAccountIds.cbegin());

    // Still verify the node account ID mapped to a valid Node.
    if (nodes.empty())
    {
      throw IllegalStateException("Node account ID " + mNodeAccountIds.cbegin()->toString() +
                                  " did not map to a valid node in the input Client's network.");
    }

    return nodes;
  }

  // If there are multiple nodes, this Executable should simply try a different node instead of a different proxy on the
  // same node.
  for (const AccountId& accountId : mNodeAccountIds)
  {
    const std::vector<std::shared_ptr<internal::Node>> nodeProxies = client.getNetwork()->getNodeProxies(accountId);

    // Verify the node account ID mapped to a valid Node.
    if (nodes.empty())
    {
      throw IllegalStateException("Node account ID " + accountId.toString() +
                                  " did not map to a valid node in the input Client's network.");
    }

    // Pick a random proxy from the proxy list to add to the node list.
    nodes.push_back(
      nodeProxies.at(internal::Utilities::getRandomNumber(0U, static_cast<unsigned int>(nodeProxies.size()) - 1U)));
  }

  return nodes;
}

//-----
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
unsigned int Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::getNodeIndexForExecute(
  const std::vector<std::shared_ptr<internal::Node>>& nodes,
  unsigned int attempt) const
{
  // Keep track of the best candidate node and its delay (initialize to make compiler happy, but this should never be
  // returned without being provided an actual legitimate value).
  unsigned int candidateNodeIndex = -1U;
  std::chrono::duration<double> candidateDelay(std::numeric_limits<int64_t>::max());

  // Start looking at nodes at the attempt index, but wrap if there's been more attempts than nodes.
  for (unsigned int i = attempt % nodes.size(); i < nodes.size(); ++i)
  {
    const std::shared_ptr<internal::Node>& node = nodes.at(i);
    if (!node->isHealthy())
    {
      // Mark this the candidate node if it has the smaller delay than the previous candidate.
      const std::chrono::duration<double> backoffTime = node->getRemainingTimeForBackoff();
      if (backoffTime < candidateDelay)
      {
        candidateNodeIndex = i;
        candidateDelay = backoffTime;
      }
    }

    // If this node is healthy, then its usable.
    else
    {
      return i;
    }
  }

  // No nodes are healthy, return the index of the one with the smallest delay.
  return candidateNodeIndex;
}

/**
 * Explicit template instantiations.
 */
template class Executable<AccountAllowanceApproveTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<AccountAllowanceDeleteTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<AccountBalanceQuery, proto::Query, proto::Response, AccountBalance>;
template class Executable<AccountCreateTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<AccountDeleteTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<AccountInfoQuery, proto::Query, proto::Response, AccountInfo>;
template class Executable<AccountRecordsQuery, proto::Query, proto::Response, AccountRecords>;
template class Executable<AccountUpdateTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<ContractByteCodeQuery, proto::Query, proto::Response, ContractByteCode>;
template class Executable<ContractCallQuery, proto::Query, proto::Response, ContractFunctionResult>;
template class Executable<ContractCreateTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<ContractDeleteTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<ContractExecuteTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<ContractInfoQuery, proto::Query, proto::Response, ContractInfo>;
template class Executable<ContractUpdateTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<EthereumTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<FileAppendTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<FileContentsQuery, proto::Query, proto::Response, FileContents>;
template class Executable<FileCreateTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<FileDeleteTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<FileInfoQuery, proto::Query, proto::Response, FileInfo>;
template class Executable<FileUpdateTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<FreezeTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<ScheduleCreateTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<ScheduleDeleteTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<ScheduleInfoQuery, proto::Query, proto::Response, ScheduleInfo>;
template class Executable<ScheduleSignTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<SystemDeleteTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<SystemUndeleteTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<TokenAssociateTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<TokenBurnTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<TokenCreateTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<TokenDeleteTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<TokenDissociateTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<TokenFeeScheduleUpdateTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<TokenFreezeTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<TokenGrantKycTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<TokenInfoQuery, proto::Query, proto::Response, TokenInfo>;
template class Executable<TokenMintTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<TokenNftInfoQuery, proto::Query, proto::Response, TokenNftInfo>;
template class Executable<TokenPauseTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<TokenRevokeKycTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<TokenUnfreezeTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<TokenUnpauseTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<TokenUpdateTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<TokenWipeTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<TopicCreateTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<TopicDeleteTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<TopicInfoQuery, proto::Query, proto::Response, TopicInfo>;
template class Executable<TopicMessageSubmitTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<TopicUpdateTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;
template class Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>;
template class Executable<TransactionRecordQuery, proto::Query, proto::Response, TransactionRecord>;
template class Executable<TransferTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;

} // namespace Hedera
