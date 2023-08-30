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
#include "Client.h"
#include "Status.h"
#include "TransactionRecord.h"
#include "TransferTransaction.h"
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
proto::Query TransactionRecordQuery::makeRequest(const Client& client,
                                                 const std::shared_ptr<internal::Node>& node) const
{
  proto::Query query;
  proto::TransactionGetRecordQuery* getTransactionRecordQuery = query.mutable_transactiongetrecord();

  proto::QueryHeader* header = getTransactionRecordQuery->mutable_header();
  header->set_responsetype(proto::ResponseType::ANSWER_ONLY);

  TransferTransaction tx = TransferTransaction()
                             .setTransactionId(TransactionId::generate(*client.getOperatorAccountId()))
                             .setNodeAccountIds({ node->getAccountId() })
                             .setMaxTransactionFee(Hbar(1LL))
                             .addHbarTransfer(*client.getOperatorAccountId(), Hbar(-1LL))
                             .addHbarTransfer(node->getAccountId(), Hbar(1LL));
  tx.onSelectNode(node);
  header->set_allocated_payment(new proto::Transaction(tx.makeRequest(client, node)));

  getTransactionRecordQuery->set_allocated_transactionid(mTransactionId->toProtobuf().release());

  return query;
}

//-----
TransactionRecord TransactionRecordQuery::mapResponse(const proto::Response& response) const
{
  return TransactionRecord::fromProtobuf(response.transactiongetrecord().transactionrecord());
}

//-----
Status TransactionRecordQuery::mapResponseStatus(const proto::Response& response) const
{
  return gProtobufResponseCodeToStatus.at(response.transactiongetrecord().header().nodetransactionprecheckcode());
}

//-----
typename Executable<TransactionRecordQuery, proto::Query, proto::Response, TransactionRecord>::ExecutionStatus
TransactionRecordQuery::determineStatus(Status status, const Client& client, const proto::Response& response)
{
  if (const Executable<TransactionRecordQuery, proto::Query, proto::Response, TransactionRecord>::ExecutionStatus
        baseStatus =
          Executable<TransactionRecordQuery, proto::Query, proto::Response, TransactionRecord>::determineStatus(
            status, client, response);
      baseStatus == ExecutionStatus::SERVER_ERROR || baseStatus == ExecutionStatus::REQUEST_ERROR)
  {
    if (status == Status::RECORD_NOT_FOUND)
    {
      return ExecutionStatus::RETRY;
    }

    return baseStatus;
  }

  // TransactionRecordQuery should wait until the receipt is actually generated. That status data is contained in the
  // protobuf receipt.
  switch (gProtobufResponseCodeToStatus.at(response.transactiongetrecord().transactionrecord().receipt().status()))
  {
    case Status::BUSY:
    case Status::UNKNOWN:
    case Status::RECORD_NOT_FOUND:
    case Status::OK:
      return ExecutionStatus::RETRY;
    default:
      return ExecutionStatus::SUCCESS;
  }
}

//-----
grpc::Status TransactionRecordQuery::submitRequest(const Client& client,
                                                   const std::chrono::system_clock::time_point& deadline,
                                                   const std::shared_ptr<internal::Node>& node,
                                                   proto::Response* response) const
{
  return node->submitQuery(
    proto::Query::QueryCase::kTransactionGetRecord, makeRequest(client, node), deadline, response);
}

} // namespace Hedera
