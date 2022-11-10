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
#include "TransactionResponse.h"

#include "Client.h"
#include "TransactionReceipt.h"
#include "TransactionReceiptQuery.h"
#include "TransactionRecord.h"
#include "TransactionRecordQuery.h"

#include <proto/transaction_response.pb.h>

namespace Hedera
{
//-----
TransactionResponse TransactionResponse::fromProtobuf(const proto::TransactionResponse& proto)
{
  TransactionResponse response;
  response.mCost = proto.cost();
  response.mValidateStatus = proto.nodetransactionprecheckcode() == proto::OK;
  return response;
}

//-----
TransactionReceipt TransactionResponse::getReceipt(const Client& client) const
{
  return getReceipt(client, client.getRequestTimeout());
}

//-----
TransactionReceipt TransactionResponse::getReceipt(const Client& client,
                                                   const std::chrono::duration<double>& timeout) const
{
  // TODO: implement proper networking so this doesn't have to loop
  TransactionReceipt txReceipt;
  TransactionReceiptQuery query = TransactionReceiptQuery().setTransactionId(mTransactionId);
  while (txReceipt.getStatus() == Status::UNKNOWN)
  {
    txReceipt = query.execute(client, timeout);
  }

  return txReceipt;
}

//-----
TransactionRecord TransactionResponse::getRecord(const Client& client) const
{
  return getRecord(client, client.getRequestTimeout());
}

//-----
TransactionRecord TransactionResponse::getRecord(const Client& client,
                                                 const std::chrono::duration<double>& timeout) const
{
  // TODO: implement proper networking so this doesn't have to loop
  TransactionRecord txRecord;
  TransactionRecordQuery query = TransactionRecordQuery().setTransactionId(mTransactionId);
  while (txRecord.getReceipt().getStatus() == Status::UNKNOWN)
  {
    txRecord = query.execute(client, timeout);
  }

  return txRecord;
}

//-----
TransactionResponse& TransactionResponse::setTransactionId(const Hedera::TransactionId& transactionId)
{
  mTransactionId = transactionId;
  return *this;
}

} // namespace Hedera
