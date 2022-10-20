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
#include "TransactionRecordQuery.h"

#include "Client.h"
#include "TransactionRecord.h"

#include <proto/query.pb.h>
#include <proto/response.pb.h>
#include <proto/transaction_get_record.pb.h>

namespace Hedera
{
//-----
TransactionRecordQuery& TransactionRecordQuery::setTransactionId(const TransactionId& transactionId)
{
  mTransactionId = transactionId;
  return *this;
}

//-----
proto::Query TransactionRecordQuery::makeRequest(const Client&) const
{
  proto::Query query;
  proto::TransactionGetRecordQuery* getTransactionRecordQuery = query.mutable_transactiongetrecord();

  if (mTransactionId.has_value())
  {
    getTransactionRecordQuery->set_allocated_transactionid(mTransactionId.value().toProtobuf().get());
  }

  return query;
}

//-----
TransactionRecord TransactionRecordQuery::mapResponse(const proto::Response& response) const
{
  return TransactionRecord::fromProtobuf(response.transactiongetrecord().transactionrecord());
}

} // namespace Hedera
