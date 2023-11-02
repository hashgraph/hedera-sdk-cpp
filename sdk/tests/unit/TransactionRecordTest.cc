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
#include "TransactionRecord.h"
#include "AccountId.h"
#include "EvmAddress.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <chrono>
#include <gtest/gtest.h>
#include <proto/transaction_record.pb.h>

using namespace Hedera;

class TransactionRecordTest : public ::testing::Test
{
};

//-----
TEST_F(TransactionRecordTest, FromProtobuf)
{
  // Given
  const auto accountIdTo = AccountId(3ULL);
  const auto accountIdFrom = AccountId(4ULL);
  const int64_t amount = 10LL;
  const std::string txHash = "txHash";
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  const std::string txMemo = "txMemo";
  const uint64_t txFee = 10ULL;
  const auto tokenId = TokenId(10ULL);
  const auto nftId = NftId(TokenId(20ULL), 1000ULL);
  const std::vector<std::byte> testEvmAddressBytes = { std::byte('0'), std::byte('1'), std::byte('2'), std::byte('3'),
                                                       std::byte('4'), std::byte('5'), std::byte('6'), std::byte('7'),
                                                       std::byte('8'), std::byte('9'), std::byte('a'), std::byte('b'),
                                                       std::byte('c'), std::byte('d'), std::byte('e'), std::byte('f'),
                                                       std::byte('g'), std::byte('h'), std::byte('i'), std::byte('j') };
  const auto contractId = ContractId(5ULL);
  const std::vector<std::byte> contractCallResult = { std::byte(0x06), std::byte(0x07), std::byte(0x08) };
  const auto scheduleId = ScheduleId(9ULL);

  proto::TransactionRecord protoTransactionRecord;
  protoTransactionRecord.mutable_receipt()->set_allocated_accountid(accountIdFrom.toProtobuf().release());
  protoTransactionRecord.set_transactionhash(txHash);
  protoTransactionRecord.set_allocated_consensustimestamp(internal::TimestampConverter::toProtobuf(now));
  protoTransactionRecord.set_allocated_transactionid(TransactionId::generate(accountIdFrom).toProtobuf().release());
  protoTransactionRecord.set_memo(txMemo);
  protoTransactionRecord.set_transactionfee(txFee);
  protoTransactionRecord.set_evm_address(internal::Utilities::byteVectorToString(testEvmAddressBytes));

  proto::ContractFunctionResult* contractFunctionResult = protoTransactionRecord.mutable_contractcallresult();
  contractFunctionResult->set_allocated_contractid(contractId.toProtobuf().release());
  contractFunctionResult->set_contractcallresult(internal::Utilities::byteVectorToString(contractCallResult));

  proto::AccountAmount* aa = protoTransactionRecord.mutable_transferlist()->add_accountamounts();
  aa->set_allocated_accountid(accountIdFrom.toProtobuf().release());
  aa->set_amount(-amount);

  aa = protoTransactionRecord.mutable_transferlist()->add_accountamounts();
  aa->set_allocated_accountid(accountIdTo.toProtobuf().release());
  aa->set_amount(amount);

  proto::TokenTransferList* list = protoTransactionRecord.add_tokentransferlists();
  list->set_allocated_token(tokenId.toProtobuf().release());

  aa = list->add_transfers();
  aa->set_allocated_accountid(accountIdTo.toProtobuf().release());
  aa->set_amount(amount);

  aa = list->add_transfers();
  aa->set_allocated_accountid(accountIdFrom.toProtobuf().release());
  aa->set_amount(-amount);

  list = protoTransactionRecord.add_tokentransferlists();
  list->set_allocated_token(nftId.mTokenId.toProtobuf().release());

  proto::NftTransfer* nft = list->add_nfttransfers();
  nft->set_serialnumber(static_cast<int64_t>(nftId.mSerialNum));
  nft->set_allocated_senderaccountid(accountIdFrom.toProtobuf().release());
  nft->set_allocated_receiveraccountid(accountIdTo.toProtobuf().release());

  protoTransactionRecord.set_allocated_scheduleref(scheduleId.toProtobuf().release());

  proto::AssessedCustomFee* assessedCustomFee = protoTransactionRecord.add_assessed_custom_fees();
  assessedCustomFee->set_amount(amount);
  assessedCustomFee->set_allocated_token_id(tokenId.toProtobuf().release());
  assessedCustomFee->set_allocated_fee_collector_account_id(accountIdFrom.toProtobuf().release());
  *assessedCustomFee->add_effective_payer_account_id() = *accountIdFrom.toProtobuf();
  *assessedCustomFee->add_effective_payer_account_id() = *accountIdTo.toProtobuf();

  proto::TokenAssociation* tokenAssociation = protoTransactionRecord.add_automatic_token_associations();
  tokenAssociation->set_allocated_account_id(accountIdFrom.toProtobuf().release());
  tokenAssociation->set_allocated_token_id(tokenId.toProtobuf().release());

  // When
  const TransactionRecord txRecord = TransactionRecord::fromProtobuf(protoTransactionRecord);

  // Then
  ASSERT_TRUE(txRecord.mReceipt.has_value());
  ASSERT_TRUE(txRecord.mReceipt->mAccountId.has_value());
  EXPECT_EQ(txRecord.mReceipt->mAccountId, accountIdFrom);

  EXPECT_EQ(txRecord.mTransactionHash, txHash);

  ASSERT_TRUE(txRecord.mConsensusTimestamp.has_value());
  EXPECT_EQ(txRecord.mConsensusTimestamp->time_since_epoch().count(), now.time_since_epoch().count());

  ASSERT_TRUE(txRecord.mTransactionID.has_value());
  EXPECT_EQ(txRecord.mTransactionID->mAccountId, accountIdFrom);
  EXPECT_GE(txRecord.mTransactionID->mValidTransactionTime, now);

  EXPECT_EQ(txRecord.mMemo, txMemo);

  EXPECT_EQ(txRecord.mTransactionFee, txFee);

  ASSERT_TRUE(txRecord.mContractFunctionResult.has_value());
  EXPECT_EQ(txRecord.mContractFunctionResult->mContractId, contractId);
  EXPECT_EQ(txRecord.mContractFunctionResult->mContractCallResult, contractCallResult);

  ASSERT_EQ(txRecord.mHbarTransferList.size(), 2);
  EXPECT_EQ(txRecord.mHbarTransferList.at(0).mAccountId, accountIdFrom);
  EXPECT_EQ(txRecord.mHbarTransferList.at(0).mAmount.toTinybars(), -amount);
  EXPECT_EQ(txRecord.mHbarTransferList.at(1).mAccountId, accountIdTo);
  EXPECT_EQ(txRecord.mHbarTransferList.at(1).mAmount.toTinybars(), amount);

  ASSERT_EQ(txRecord.mTokenTransferList.size(), 2);
  EXPECT_EQ(txRecord.mTokenTransferList.at(0).mTokenId, tokenId);
  EXPECT_EQ(txRecord.mTokenTransferList.at(0).mAccountId, accountIdTo);
  EXPECT_EQ(txRecord.mTokenTransferList.at(0).mAmount, amount);
  EXPECT_EQ(txRecord.mTokenTransferList.at(1).mTokenId, tokenId);
  EXPECT_EQ(txRecord.mTokenTransferList.at(1).mAccountId, accountIdFrom);
  EXPECT_EQ(txRecord.mTokenTransferList.at(1).mAmount, -amount);

  ASSERT_EQ(txRecord.mNftTransferList.size(), 1);
  EXPECT_EQ(txRecord.mNftTransferList.at(0).mNftId, nftId);
  EXPECT_EQ(txRecord.mNftTransferList.at(0).mSenderAccountId, accountIdFrom);
  EXPECT_EQ(txRecord.mNftTransferList.at(0).mReceiverAccountId, accountIdTo);

  EXPECT_EQ(txRecord.mScheduleRef, scheduleId);

  ASSERT_EQ(txRecord.mAssessedCustomFees.size(), 1);
  EXPECT_EQ(txRecord.mAssessedCustomFees.at(0).mAmount, amount);
  EXPECT_EQ(txRecord.mAssessedCustomFees.at(0).mTokenId, tokenId);
  EXPECT_EQ(txRecord.mAssessedCustomFees.at(0).mFeeCollectorAccountId, accountIdFrom);
  ASSERT_EQ(txRecord.mAssessedCustomFees.at(0).mPayerAccountIdList.size(), 2);
  EXPECT_EQ(txRecord.mAssessedCustomFees.at(0).mPayerAccountIdList.at(0), accountIdFrom);
  EXPECT_EQ(txRecord.mAssessedCustomFees.at(0).mPayerAccountIdList.at(1), accountIdTo);

  ASSERT_EQ(txRecord.mAutomaticTokenAssociations.size(), 1);
  EXPECT_EQ(txRecord.mAutomaticTokenAssociations.at(0).mAccountId, accountIdFrom);
  EXPECT_EQ(txRecord.mAutomaticTokenAssociations.at(0).mTokenId, tokenId);

  ASSERT_TRUE(txRecord.mEvmAddress.has_value());
  EXPECT_EQ(txRecord.mEvmAddress->toBytes(), testEvmAddressBytes);
}
