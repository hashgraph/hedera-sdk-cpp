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
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <proto/transaction_get_receipt.pb.h>
#include <proto/transaction_receipt.pb.h>

namespace Hedera
{
//-----
TransactionReceipt TransactionReceipt::fromProtobuf(const proto::TransactionGetReceiptResponse& proto,
                                                    const TransactionId& transactionId)
{
  TransactionReceipt receipt = TransactionReceipt::fromProtobuf(proto.receipt());
  receipt.mTransactionId = transactionId;

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
TransactionReceipt TransactionReceipt::fromProtobuf(const proto::TransactionReceipt& proto,
                                                    const TransactionId& transactionId)
{
  TransactionReceipt receipt;
  receipt.mTransactionId = transactionId;
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
    receipt.mExchangeRates = ExchangeRates::fromProtobuf(proto.exchangerate());
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
TransactionReceipt TransactionReceipt::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::TransactionReceipt proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
std::unique_ptr<proto::TransactionReceipt> TransactionReceipt::toProtobuf() const
{
  auto proto = std::make_unique<proto::TransactionReceipt>();
  proto->set_status(gStatusToProtobufResponseCode.at(mStatus));

  if (mAccountId.has_value())
  {
    proto->set_allocated_accountid(mAccountId->toProtobuf().release());
  }

  if (mFileId.has_value())
  {
    proto->set_allocated_fileid(mFileId->toProtobuf().release());
  }

  if (mContractId.has_value())
  {
    proto->set_allocated_contractid(mContractId->toProtobuf().release());
  }

  proto->mutable_exchangerate()->mutable_currentrate()->set_hbarequiv(mExchangeRates.mCurrentRate.mHbars);
  proto->mutable_exchangerate()->mutable_currentrate()->set_centequiv(mExchangeRates.mCurrentRate.mCents);
  proto->mutable_exchangerate()->mutable_currentrate()->set_allocated_expirationtime(
    internal::TimestampConverter::toSecondsProtobuf(mExchangeRates.mCurrentRate.mExpirationTime));

  if (mTopicId.has_value())
  {
    proto->set_allocated_topicid(mTopicId->toProtobuf().release());
  }

  if (mTopicSequenceNumber.has_value())
  {
    proto->set_topicsequencenumber(mTopicSequenceNumber.value());
  }

  if (mTopicRunningHash.has_value())
  {
    proto->set_topicrunninghash(internal::Utilities::byteVectorToString(mTopicRunningHash.value()));
  }

  if (mTopicRunningHashVersion.has_value())
  {
    proto->set_topicrunninghashversion(mTopicRunningHashVersion.value());
  }

  if (mNewTotalSupply.has_value())
  {
    proto->set_newtotalsupply(mNewTotalSupply.value());
  }

  if (mScheduleId.has_value())
  {
    proto->set_allocated_scheduleid(mScheduleId->toProtobuf().release());
  }

  if (mScheduledTransactionId.has_value())
  {
    proto->set_allocated_scheduledtransactionid(mScheduledTransactionId->toProtobuf().release());
  }

  std::for_each(mSerialNumbers.cbegin(),
                mSerialNumbers.cend(),
                [&proto](uint64_t num) { proto->add_serialnumbers(static_cast<int64_t>(num)); });

  return proto;
}

//-----
std::vector<std::byte> TransactionReceipt::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
void TransactionReceipt::validateStatus() const
{
  if (mStatus != Status::SUCCESS)
  {
    throw ReceiptStatusException(mTransactionId, mStatus);
  }
}

} // namespace Hedera
