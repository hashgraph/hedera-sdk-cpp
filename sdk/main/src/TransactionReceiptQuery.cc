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
#include "TransactionReceiptQuery.h"
#include "Status.h"
#include "TransactionReceipt.h"
#include "impl/Node.h"

#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>
#include <proto/transaction_get_receipt.pb.h>

namespace Hedera
{
//-----
TransactionReceiptQuery& TransactionReceiptQuery::setTransactionId(const TransactionId& transactionId)
{
  mTransactionId = transactionId;
  return *this;
}

//-----
TransactionReceipt TransactionReceiptQuery::mapResponse(const proto::Response& response) const
{
  return TransactionReceipt::fromProtobuf(response.transactiongetreceipt().receipt());
}

//-----
grpc::Status TransactionReceiptQuery::submitRequest(const proto::Query& request,
                                                    const std::shared_ptr<internal::Node>& node,
                                                    const std::chrono::system_clock::time_point& deadline,
                                                    proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kTransactionGetReceipt, request, deadline, response);
}

//-----
typename Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>::ExecutionStatus
TransactionReceiptQuery::determineStatus(Status status, const Client& client, const proto::Response& response)
{
  if (const Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>::ExecutionStatus
        baseStatus =
          Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>::determineStatus(
            status, client, response);
      baseStatus == ExecutionStatus::SERVER_ERROR || baseStatus == ExecutionStatus::REQUEST_ERROR)
  {
    if (status == Status::RECEIPT_NOT_FOUND)
    {
      return ExecutionStatus::RETRY;
    }

    return baseStatus;
  }

  // TransactionReceiptQuery should wait until the receipt is actually generated. That status data is contained in the
  // protobuf receipt.
  switch (gProtobufResponseCodeToStatus.at(response.transactiongetreceipt().receipt().status()))
  {
    case Status::BUSY:
    case Status::UNKNOWN:
    case Status::RECEIPT_NOT_FOUND:
    case Status::OK:
      return ExecutionStatus::RETRY;
    default:
      return ExecutionStatus::SUCCESS;
  }
}

//-----
proto::Query TransactionReceiptQuery::buildRequest(proto::QueryHeader* header) const
{
  auto transactionGetReceiptQuery = std::make_unique<proto::TransactionGetReceiptQuery>();
  transactionGetReceiptQuery->set_allocated_header(header);

  if (mTransactionId.has_value())
  {
    transactionGetReceiptQuery->set_allocated_transactionid(mTransactionId->toProtobuf().release());
  }

  proto::Query query;
  query.set_allocated_transactiongetreceipt(transactionGetReceiptQuery.release());
  return query;
}

//-----
proto::ResponseHeader TransactionReceiptQuery::mapResponseHeader(const proto::Response& response) const
{
  return response.transactiongetreceipt().header();
}

} // namespace Hedera
