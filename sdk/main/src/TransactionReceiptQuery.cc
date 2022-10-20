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

#include "Client.h"
#include "TransactionReceipt.h"

#include <proto/query.pb.h>
#include <proto/response.pb.h>
#include <proto/transaction_get_receipt.pb.h>

namespace Hedera
{
//-----
proto::Query TransactionReceiptQuery::makeRequest(const Client&) const
{
  proto::Query query;
  proto::TransactionGetReceiptQuery* getTransactionReceiptQuery = query.mutable_transactiongetreceipt();

  if (mTransactionId.has_value())
  {
    getTransactionReceiptQuery->set_allocated_transactionid(mTransactionId->toProtobuf().get());
  }

  return query;
}

//-----
TransactionReceipt TransactionReceiptQuery::mapResponse(const proto::Response& response) const
{
  return TransactionReceipt::fromProtobuf(response.transactiongetreceipt().receipt());
}

//-----
TransactionReceiptQuery& TransactionReceiptQuery::setTransactionId(const Hedera::TransactionId& transactionId)
{
  mTransactionId = transactionId;
  return *this;
}

} // namespace Hedera