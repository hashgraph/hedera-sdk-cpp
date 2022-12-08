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
std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)>
TransactionReceiptQuery::getGrpcMethod(const std::shared_ptr<internal::Node>& node) const
{
  return node->getGrpcQueryMethod(proto::Query::QueryCase::kTransactionGetReceipt);
}

} // namespace Hedera
