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
#include "TransactionRecord.h"

#include "TransactionReceipt.h"

#include "helper/TimestampConverter.h"

#include <proto/transaction_record.pb.h>

namespace Hedera
{
//-----
TransactionRecord TransactionRecord::fromProtobuf(const proto::TransactionRecord& proto)
{
  TransactionRecord transactionRecord;

  if (proto.has_receipt())
  {
    transactionRecord.mReceipt = TransactionReceipt::fromProtobuf(proto.receipt());
  }

  transactionRecord.mTransactionHash = proto.transactionhash();

  if (proto.has_consensustimestamp())
  {
    transactionRecord.mConsensusTimestamp = TimestampConverter::fromProtobuf(proto.consensustimestamp());
  }

  if (proto.has_transactionid())
  {
    transactionRecord.mTransactionID = TransactionId::fromProtobuf(proto.transactionid());
  }

  transactionRecord.mMemo = proto.memo();
  transactionRecord.mTransactionFee = proto.transactionfee();

  if (proto.has_transferlist())
  {
    for (int i = 0; i < proto.transferlist().accountamounts_size(); ++i)
    {
      transactionRecord.mTransferList->emplace_back(
        AccountId::fromProtobuf(proto.transferlist().accountamounts(i).accountid()),
        Hbar(proto.transferlist().accountamounts(i).amount(), HbarUnit::TINYBAR()));
    }
  }

  return transactionRecord;
}

} // namespace Hedera
