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
TransactionReceiptQuery& TransactionReceiptQuery::setIncludeChildren(bool children)
{
  mIncludeChildren = children;
  return *this;
}

//-----
TransactionReceiptQuery& TransactionReceiptQuery::setIncludeDuplicates(bool duplicates)
{
  mIncludeDuplicates = duplicates;
  return *this;
}

//-----
TransactionReceipt TransactionReceiptQuery::mapResponse(const proto::Response& response) const
{
  return TransactionReceipt::fromProtobuf(response.transactiongetreceipt(), mTransactionId.value());
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
void TransactionReceiptQuery::validateChecksums(const Client& client) const
{
  if (mTransactionId.has_value())
  {
    mTransactionId->getAccountId().validateChecksum(client);
  }
}

//-----
typename Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>::ExecutionStatus
TransactionReceiptQuery::determineStatus(Status status, const Client& client, const proto::Response& response)
{
  if (const Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>::ExecutionStatus
        baseStatus =
          Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>::determineStatus(
            status, client, response);
      baseStatus == ExecutionStatus::SERVER_ERROR)
  {
    return baseStatus;
  }

  switch (status)
  {
    case Status::BUSY:
    case Status::UNKNOWN:
    case Status::RECEIPT_NOT_FOUND:
    case Status::RECORD_NOT_FOUND:
      return ExecutionStatus::RETRY;

    case Status::OK:
    {
      switch (gProtobufResponseCodeToStatus.at(response.transactiongetreceipt().receipt().status()))
      {
        case Status::BUSY:
        case Status::UNKNOWN:
        case Status::OK:
        case Status::RECEIPT_NOT_FOUND:
        case Status::RECORD_NOT_FOUND:
          return ExecutionStatus::RETRY;
        default:
          return ExecutionStatus::SUCCESS;
      }
    }

    default:
      return ExecutionStatus::REQUEST_ERROR;
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

  transactionGetReceiptQuery->set_includeduplicates(mIncludeDuplicates);
  transactionGetReceiptQuery->set_include_child_receipts(mIncludeChildren);

  proto::Query query;
  query.set_allocated_transactiongetreceipt(transactionGetReceiptQuery.release());
  return query;
}

//-----
proto::ResponseHeader TransactionReceiptQuery::mapResponseHeader(const proto::Response& response) const
{
  saveCostFromHeader(response.transactiongetreceipt().header());
  return response.transactiongetreceipt().header();
}

} // namespace Hedera
