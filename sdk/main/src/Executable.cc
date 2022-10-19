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
#include "Executable.h"

#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "AccountCreateTransaction.h"
#include "Client.h"
#include "Node.h"
#include "TransactionReceipt.h"
#include "TransactionReceiptQuery.h"
#include "TransactionResponse.h"

#include <proto/query.pb.h>
#include <proto/response.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_response.pb.h>

#include <grpcpp/impl/codegen/status.h>

#include <utility>

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
  const std::chrono::duration<double>& duration)
{
  for (std::shared_ptr<Node> node : client.getNetwork()->getNodesWithAccountIds(mNodeAccountIds))
  {
    std::pair<ProtoResponseType, grpc::Status> response = node->submitRequest(makeRequest(), duration);
    if (response.second.ok())
    {
      return mapResponse(response.first);
    }
  }

  // TODO: throw?
  return SdkResponseType();
}

//-----
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
SdkRequestType& Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::setNodeAccountIds(
  const std::vector<AccountId>& nodeAccountIds)
{
  mNodeAccountIds = nodeAccountIds;
  return static_cast<SdkRequestType&>(*this);
}

/**
 * Explicit template instantiation
 */
template class Executable<AccountBalanceQuery, proto::Query, proto::Response, AccountBalance>;
template class Executable<AccountCreateTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>;

} // namespace Hedera
