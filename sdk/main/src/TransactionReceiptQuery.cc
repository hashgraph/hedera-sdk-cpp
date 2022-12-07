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
std::unique_ptr<Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>>
TransactionReceiptQuery::clone() const
{
  return std::make_unique<TransactionReceiptQuery>(*this);
}

//-----
TransactionReceiptQuery& TransactionReceiptQuery::setTransactionId(const TransactionId& transactionId)
{
  mTransactionId = transactionId;
  return *this;
}

//-----
proto::Query TransactionReceiptQuery::makeRequest(const Client&, const std::shared_ptr<internal::Node>&) const
{
  proto::Query query;
  proto::TransactionGetReceiptQuery* getTransactionReceiptQuery = query.mutable_transactiongetreceipt();

  proto::QueryHeader* header = getTransactionReceiptQuery->mutable_header();
  header->set_responsetype(proto::ANSWER_ONLY);

  // This is a free query, so no payment required
  getTransactionReceiptQuery->set_allocated_transactionid(mTransactionId->toProtobuf().release());

  return query;
}

//-----
TransactionReceipt TransactionReceiptQuery::mapResponse(const proto::Response& response) const
{
  return TransactionReceipt::fromProtobuf(response.transactiongetreceipt().receipt());
}

//-----
Status TransactionReceiptQuery::mapResponseStatus(const proto::Response& response) const
{
  return STATUS_MAP.at(response.transactiongetreceipt().header().nodetransactionprecheckcode());
}

//-----
typename Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>::ExecutionStatus
TransactionReceiptQuery::shouldRetry(Status status, const Client& client, const proto::Response& response)
{
  if (const Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>::ExecutionStatus
        baseStatus =
          Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>::shouldRetry(
            status, client, response);
      baseStatus != ExecutionStatus::UNKNOWN)
  {
    return baseStatus;
  }

  switch (status)
  {
    case Status::UNKNOWN:
    case Status::RECEIPT_NOT_FOUND:
      return ExecutionStatus::RETRY;
    case Status::OK:
      break;
    default:
      return ExecutionStatus::REQUEST_ERROR;
  }

  // Check the actual receipt status value to ensure the receipt actually holds correct data.
  switch (STATUS_MAP.at(response.transactiongetreceipt().receipt().status()))
  {
    case Status::BUSY:
    case Status::UNKNOWN:
    case Status::RECEIPT_NOT_FOUND:
    case Status::RECORD_NOT_FOUND:
    case Status::OK:
      return ExecutionStatus::RETRY;
    default:
      return ExecutionStatus::SUCCESS;
  }
}

//-----
grpc::Status TransactionReceiptQuery::submitRequest(const Client& client,
                                                    const std::chrono::system_clock::time_point& deadline,
                                                    const std::shared_ptr<internal::Node>& node,
                                                    proto::Response* response) const
{
  return node->submitQuery(
    proto::Query::QueryCase::kTransactionGetReceipt, makeRequest(client, node), deadline, response);
}

} // namespace Hedera
