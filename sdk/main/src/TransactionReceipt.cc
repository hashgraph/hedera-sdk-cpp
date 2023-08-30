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
#include "TransactionReceipt.h"
#include "exceptions/ReceiptStatusException.h"
#include "impl/Utilities.h"

#include <proto/transaction_get_receipt.pb.h>
#include <proto/transaction_receipt.pb.h>

namespace Hedera
{
//-----
TransactionReceipt TransactionReceipt::fromProtobuf(const proto::TransactionGetReceiptResponse& proto)
{
  TransactionReceipt receipt = TransactionReceipt::fromProtobuf(proto.receipt());

  for (int i = 0; i < proto.duplicatetransactionreceipts_size(); ++i)
  {
    receipt.mDuplicates.push_back(TransactionReceipt::fromProtobuf(proto.duplicatetransactionreceipts(i)));
  }

  for (int i = 0; i < proto.child_transaction_receipts_size(); ++i)
  {
    receipt.mChildren.push_back(TransactionReceipt::fromProtobuf(proto.child_transaction_receipts(i)));
  }

  return receipt;
}

//-----
TransactionReceipt TransactionReceipt::fromProtobuf(const proto::TransactionReceipt& proto)
{
  TransactionReceipt receipt;
  receipt.mStatus = gProtobufResponseCodeToStatus.at(proto.status());

  if (proto.has_accountid())
  {
    receipt.mAccountId = AccountId::fromProtobuf(proto.accountid());
  }

  if (proto.has_fileid())
  {
    receipt.mFileId = FileId::fromProtobuf(proto.fileid());
  }

  if (proto.has_contractid())
  {
    receipt.mContractId = ContractId::fromProtobuf(proto.contractid());
  }

  if (proto.has_exchangerate())
  {
    receipt.mExchangeRates = ExchangeRateSet::fromProtobuf(proto.exchangerate());
  }

  if (proto.has_topicid())
  {
    receipt.mTopicId = TopicId::fromProtobuf(proto.topicid());
  }

  if (proto.topicsequencenumber() != 0ULL)
  {
    receipt.mTopicSequenceNumber = proto.topicsequencenumber();
  }

  if (!proto.topicrunninghash().empty())
  {
    receipt.mTopicRunningHash = internal::Utilities::stringToByteVector(proto.topicrunninghash());
  }

  if (proto.topicrunninghashversion() != 0ULL)
  {
    receipt.mTopicRunningHashVersion = proto.topicrunninghashversion();
  }

  if (proto.has_tokenid())
  {
    receipt.mTokenId = TokenId::fromProtobuf(proto.tokenid());
  }

  receipt.mNewTotalSupply = proto.newtotalsupply();

  if (proto.has_scheduleid())
  {
    receipt.mScheduleId = ScheduleId::fromProtobuf(proto.scheduleid());
  }

  if (proto.has_scheduledtransactionid())
  {
    receipt.mScheduledTransactionId = TransactionId::fromProtobuf(proto.scheduledtransactionid());
  }

  for (int i = 0; i < proto.serialnumbers_size(); ++i)
  {
    receipt.mSerialNumbers.push_back(static_cast<uint64_t>(proto.serialnumbers(i)));
  }

  return receipt;
}

//-----
void TransactionReceipt::validateStatus() const
{
  if (mStatus != Status::SUCCESS)
  {
    throw ReceiptStatusException(gStatusToString.at(mStatus));
  }
}

} // namespace Hedera
