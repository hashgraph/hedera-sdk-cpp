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
#include "TransactionRecordQuery.h"
#include "Status.h"
#include "TransactionRecord.h"
#include "impl/Node.h"

#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>
#include <proto/transaction_get_record.pb.h>

#include <vector>

namespace Hedera
{
//-----
TransactionRecordQuery& TransactionRecordQuery::setTransactionId(const TransactionId& transactionId)
{
  mTransactionId = transactionId;
  return *this;
}

//-----
TransactionRecord TransactionRecordQuery::mapResponse(const proto::Response& response) const
{
  return TransactionRecord::fromProtobuf(response.transactiongetrecord().transactionrecord());
}

//-----
grpc::Status TransactionRecordQuery::submitRequest(const proto::Query& request,
                                                   const std::shared_ptr<internal::Node>& node,
                                                   const std::chrono::system_clock::time_point& deadline,
                                                   proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kTransactionGetRecord, request, deadline, response);
}

//-----
typename Executable<TransactionRecordQuery, proto::Query, proto::Response, TransactionRecord>::ExecutionStatus
TransactionRecordQuery::determineStatus(Status status, const Client& client, const proto::Response& response)
{
  if (const Executable<TransactionRecordQuery, proto::Query, proto::Response, TransactionRecord>::ExecutionStatus
        baseStatus =
          Executable<TransactionRecordQuery, proto::Query, proto::Response, TransactionRecord>::determineStatus(
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
      if (isCostQuery())
      {
        return ExecutionStatus::SUCCESS;
      }

      switch (gProtobufResponseCodeToStatus.at(response.transactiongetrecord().transactionrecord().receipt().status()))
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
proto::Query TransactionRecordQuery::buildRequest(proto::QueryHeader* header) const
{
  auto transactionGetRecordQuery = std::make_unique<proto::TransactionGetRecordQuery>();
  transactionGetRecordQuery->set_allocated_header(header);

  if (mTransactionId.has_value())
  {
    transactionGetRecordQuery->set_allocated_transactionid(mTransactionId->toProtobuf().release());
  }

  proto::Query query;
  query.set_allocated_transactiongetrecord(transactionGetRecordQuery.release());
  return query;
}

//-----
proto::ResponseHeader TransactionRecordQuery::mapResponseHeader(const proto::Response& response) const
{
  Query<TransactionRecordQuery, TransactionRecord>::saveCostFromHeader(response.transactiongetrecord().header());
  return response.transactiongetrecord().header();
}

} // namespace Hedera
