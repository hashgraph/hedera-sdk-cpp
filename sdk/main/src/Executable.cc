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
#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "AccountCreateTransaction.h"
#include "Client.h"
#include "TransactionReceipt.h"
#include "TransactionReceiptQuery.h"
#include "TransactionRecord.h"
#include "TransactionRecordQuery.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

#include <grpcpp/client_context.h>
#include <grpcpp/impl/codegen/status.h>
#include <proto/query.pb.h>
#include <proto/response.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_response.pb.h>
#include <stdexcept>
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
  const std::chrono::duration<int64_t>& duration)
{
  // Perform any operations needed when executing.
  onExecute(client);

  // Create the context from the timeout duration
  grpc::ClientContext context;
  context.set_deadline(std::chrono::system_clock::now() +
                       std::chrono::duration_cast<std::chrono::milliseconds>(duration));

  // The response object to fill
  ProtoResponseType response;

  for (const auto& node : client.getNodesWithAccountIds(mNodeAccountIds))
  {
    // Do node specific tasks
    onSelectNode(node);

    auto grpcFunc = getGrpcMethod(node);

    // Create the request
    const ProtoRequestType request = makeRequest(client, node);

    // Call the gRPC method
    grpc::Status status = grpcFunc(&context, request, &response);

    if (status.error_code() == 0)
    {
      return mapResponse(response);
    }
  }

  throw std::runtime_error("Unable to communicate with any node");
}

//-----
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
SdkRequestType& Executable<SdkRequestType, ProtoRequestType, ProtoResponseType, SdkResponseType>::setNodeAccountIds(
  const std::vector<std::shared_ptr<AccountId>>& nodeAccountIds)
{
  mNodeAccountIds = nodeAccountIds;
  return static_cast<SdkRequestType&>(*this);
}

/**
 * Explicit template instantiations.
 */
template class Executable<AccountBalanceQuery, proto::Query, proto::Response, AccountBalance>;
template class Executable<AccountCreateTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;
template class Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>;
template class Executable<TransactionRecordQuery, proto::Query, proto::Response, TransactionRecord>;
template class Executable<TransferTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>;

} // namespace Hedera
