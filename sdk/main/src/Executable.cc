/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#include "ContractFunctionResult.h"
#include "TransactionReceipt.h"
#include "TransactionReceiptQuery.h"
#include "TransactionRecord.h"
#include "TransactionRecordQuery.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "exceptions/MaxAttemptsExceededException.h"
#include "exceptions/PrecheckStatusException.h"
#include "impl/Node.h"

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

  // The time to timeout
  const std::chrono::system_clock::time_point timeoutTime =
    std::chrono::system_clock::now() + std::chrono::duration_cast<std::chrono::system_clock::duration>(timeout);

  // The response object to fill
  ProtoResponseType response;

  // List of nodes to which this Executable may be sent
  const std::vector<std::shared_ptr<internal::Node>> nodes = client.getNodesWithAccountIds(mNodeAccountIds);

  for (uint32_t attempt = 1;; ++attempt)
  {
    if (attempt > mCurrentMaxAttempts)
    {
      throw MaxAttemptsExceededException("Max number of attempts made (max attempts allowed: " +
                                         std::to_string(mCurrentMaxAttempts));
    }

    std::shared_ptr<internal::Node> node = getNodeForExecute(nodes);

    // If the returned node is not healthy, then no nodes are healthy and the returned node has the shortest remaining
    // delay. Sleep for the delay period.
    if (!node->isHealthy())
    {
      std::this_thread::sleep_for(node->getRemainingTimeForBackoff());
    }

    // Make sure the Node is connected. If it can't connect, mark this Node as unhealthy and try another Node.
    if (!node->connect(timeoutTime))
    {
      node->increaseBackoff();
      continue;
    }

    // Do node specific tasks
    onSelectNode(node);

    const grpc::Status status = submitRequest(client, timeoutTime, node, &response);

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
void Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::onSelectNode(
  const std::shared_ptr<internal::Node>& node)
{
  node->setMinBackoff(mCurrentMinBackoff);
  node->setMaxBackoff(mCurrentMaxBackoff);
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
    // Let derived class handle this status
    default:
      return ExecutionStatus::UNKNOWN;
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
std::shared_ptr<internal::Node>
Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::getNodeForExecute(
  const std::vector<std::shared_ptr<internal::Node>>& nodes) const
{
  // Keep track of the best candidate node and its delay.
  std::shared_ptr<internal::Node> candidateNode = nullptr;
  std::chrono::duration<double> candidateDelay(std::numeric_limits<int64_t>::max());

  for (const auto& node : nodes)
  {
    if (!node->isHealthy())
    {
      // Mark this the candidate node if it has the smaller delay than the previous candidate.
      const std::chrono::duration<double> backoffTime = node->getRemainingTimeForBackoff();
      if (backoffTime < candidateDelay)
      {
        candidateNode = node;
        candidateDelay = backoffTime;
      }
    }

    // If this node is healthy, then return it.
    else
    {
      return node;
    }
  }

  // No nodes are healthy, return the one with the smallest delay.
  return candidateNode;
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
template class Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>;
template class Executable<TransactionRecordQuery, proto::Query, proto::Response, TransactionRecord>;
template class Executable<TransferTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;

} // namespace Hedera
