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
  TransactionReceipt txReceipt = TransactionReceiptQuery().setTransactionId(mTransactionId).execute(client, timeout);

  if (mValidateStatus)
  {
    txReceipt.validateStatus();
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
  return TransactionRecordQuery().setTransactionId(mTransactionId).execute(client, timeout);
}

//-----
TransactionResponse& TransactionResponse::setValidateStatus(bool validate)
{
  mValidateStatus = validate;
  return *this;
}

} // namespace Hedera
