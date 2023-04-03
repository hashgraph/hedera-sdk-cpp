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

// Tests deserialization of proto::TransactionRecord -> Hedera::TransactionRecord.
TEST_F(TransactionRecordTest, ProtobufTransactionRecord)
{
  // Given
  const auto accountIdTo = AccountId(3ULL);
  const auto accountIdFrom = AccountId(4ULL);
  const int64_t transferAmount = 10LL;
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

  proto::TransactionRecord protoTransactionRecord;
  protoTransactionRecord.mutable_receipt()->set_allocated_accountid(accountIdFrom.toProtobuf().release());
  protoTransactionRecord.set_allocated_transactionhash(new std::string(txHash));
  protoTransactionRecord.set_allocated_consensustimestamp(internal::TimestampConverter::toProtobuf(now));
  protoTransactionRecord.set_allocated_transactionid(TransactionId::generate(accountIdFrom).toProtobuf().release());
  protoTransactionRecord.set_allocated_memo(new std::string(txMemo));
  protoTransactionRecord.set_transactionfee(txFee);
  protoTransactionRecord.set_allocated_evm_address(
    new std::string(internal::Utilities::byteVectorToString(testEvmAddressBytes)));

  proto::AccountAmount* aa = protoTransactionRecord.mutable_transferlist()->add_accountamounts();
  aa->set_allocated_accountid(accountIdFrom.toProtobuf().release());
  aa->set_amount(-transferAmount);

  aa = protoTransactionRecord.mutable_transferlist()->add_accountamounts();
  aa->set_allocated_accountid(accountIdTo.toProtobuf().release());
  aa->set_amount(transferAmount);

  proto::TokenTransferList* list = protoTransactionRecord.add_tokentransferlists();
  list->set_allocated_token(tokenId.toProtobuf().release());

  aa = list->add_transfers();
  aa->set_allocated_accountid(accountIdTo.toProtobuf().release());
  aa->set_amount(transferAmount);

  aa = list->add_transfers();
  aa->set_allocated_accountid(accountIdFrom.toProtobuf().release());
  aa->set_amount(-transferAmount);

  list = protoTransactionRecord.add_tokentransferlists();
  list->set_allocated_token(nftId.getTokenId().toProtobuf().release());

  proto::NftTransfer* nft = list->add_nfttransfers();
  nft->set_serialnumber(static_cast<int64_t>(nftId.getSerialNum()));
  nft->set_allocated_senderaccountid(accountIdFrom.toProtobuf().release());
  nft->set_allocated_receiveraccountid(accountIdTo.toProtobuf().release());

  // When
  TransactionRecord txRecord = TransactionRecord::fromProtobuf(protoTransactionRecord);

  // Then
  EXPECT_TRUE(txRecord.getReceipt().has_value());
  EXPECT_TRUE(txRecord.getReceipt()->getAccountId());
  EXPECT_EQ(*txRecord.getReceipt()->getAccountId(), accountIdFrom);

  EXPECT_EQ(txRecord.getTransactionHash(), txHash);

  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());
  EXPECT_EQ(txRecord.getConsensusTimestamp()->time_since_epoch().count(), now.time_since_epoch().count());

  EXPECT_TRUE(txRecord.getTransactionId().has_value());
  EXPECT_EQ(txRecord.getTransactionId()->getAccountId(), accountIdFrom);
  EXPECT_GE(txRecord.getTransactionId()->getValidTransactionTime(), now);

  EXPECT_EQ(txRecord.getTransactionMemo(), txMemo);

  EXPECT_EQ(txRecord.getTransactionFee(), txFee);

  EXPECT_EQ(txRecord.getHbarTransferList().size(), 2);
  EXPECT_EQ(txRecord.getHbarTransferList().at(0).getAccountId(), accountIdFrom);
  EXPECT_EQ(txRecord.getHbarTransferList().at(0).getAmount().toTinybars(), -transferAmount);
  EXPECT_EQ(txRecord.getHbarTransferList().at(1).getAccountId(), accountIdTo);
  EXPECT_EQ(txRecord.getHbarTransferList().at(1).getAmount().toTinybars(), transferAmount);

  EXPECT_EQ(txRecord.getTokenTransferList().size(), 2);
  EXPECT_EQ(txRecord.getTokenTransferList().at(0).getTokenId(), tokenId);
  EXPECT_EQ(txRecord.getTokenTransferList().at(0).getAccountId(), accountIdTo);
  EXPECT_EQ(txRecord.getTokenTransferList().at(0).getAmount(), transferAmount);
  EXPECT_EQ(txRecord.getTokenTransferList().at(1).getTokenId(), tokenId);
  EXPECT_EQ(txRecord.getTokenTransferList().at(1).getAccountId(), accountIdFrom);
  EXPECT_EQ(txRecord.getTokenTransferList().at(1).getAmount(), -transferAmount);

  EXPECT_EQ(txRecord.getNftTransferList().size(), 1);
  EXPECT_EQ(txRecord.getNftTransferList().at(0).getNftId(), nftId);
  EXPECT_EQ(txRecord.getNftTransferList().at(0).getSenderAccountId(), accountIdFrom);
  EXPECT_EQ(txRecord.getNftTransferList().at(0).getReceiverAccountId(), accountIdTo);

  EXPECT_TRUE(txRecord.getEvmAddress().has_value());
  EXPECT_EQ(txRecord.getEvmAddress()->toBytes(), testEvmAddressBytes);
}