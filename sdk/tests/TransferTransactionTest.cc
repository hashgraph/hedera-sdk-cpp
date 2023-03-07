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
#include "TransferTransaction.h"
#include "AccountId.h"
#include "Hbar.h"
#include "TokenId.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class TransferTransactionTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId1() const { return mAccountId1; }
  [[nodiscard]] inline const AccountId& getTestAccountId2() const { return mAccountId2; }
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTokenId; }
  [[nodiscard]] inline const NftId& getTestNftId() const { return mNftId; }
  [[nodiscard]] inline const Hbar& getTestAmount() const { return mAmount; }
  [[nodiscard]] inline uint32_t getTestExpectedDecimals() const { return mExpectedDecimals; }
  [[nodiscard]] inline bool getTestApproval() const { return mApproval; }

private:
  const AccountId mAccountId1 = AccountId(10ULL);
  const AccountId mAccountId2 = AccountId(20ULL);
  const TokenId mTokenId = TokenId(30ULL);
  const NftId mNftId = NftId(mTokenId, 40ULL);
  const Hbar mAmount = Hbar(50ULL);
  const uint32_t mExpectedDecimals = 6U;
  const bool mApproval = true;
};

//-----
TEST_F(TransferTransactionTest, ConstructTransferTransaction)
{
  TransferTransaction transaction;
  EXPECT_TRUE(transaction.getHbarTransfers().empty());
  EXPECT_TRUE(transaction.getTokenTransfers().empty());
  EXPECT_TRUE(transaction.getNftTransfers().empty());
  EXPECT_TRUE(transaction.getTokenIdDecimals().empty());
}

//-----
TEST_F(TransferTransactionTest, ConstructTransferTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::CryptoTransferTransactionBody>();

  proto::AccountAmount* amount = body->mutable_transfers()->add_accountamounts();
  amount->set_allocated_accountid(getTestAccountId1().toProtobuf().release());
  amount->set_amount(getTestAmount().toTinybars());
  amount->set_is_approval(getTestApproval());

  proto::TokenTransferList* list = body->add_tokentransfers();
  list->set_allocated_token(getTestTokenId().toProtobuf().release());
  list->mutable_expected_decimals()->set_value(getTestExpectedDecimals());

  amount = list->add_transfers();
  amount->set_allocated_accountid(getTestAccountId2().toProtobuf().release());
  amount->set_amount(getTestAmount().toTinybars());
  amount->set_is_approval(getTestApproval());

  list = body->add_tokentransfers();
  list->set_allocated_token(getTestNftId().getTokenId().toProtobuf().release());

  proto::NftTransfer* nft = list->add_nfttransfers();
  nft->set_allocated_senderaccountid(getTestAccountId1().toProtobuf().release());
  nft->set_allocated_receiveraccountid(getTestAccountId2().toProtobuf().release());
  nft->set_serialnumber(static_cast<int64_t>(getTestNftId().getSerialNum()));
  nft->set_is_approval(getTestApproval());

  proto::TransactionBody txBody;
  txBody.set_allocated_cryptotransfer(body.release());

  // When
  TransferTransaction transferTransaction(txBody);

  // Then
  const std::unordered_map<AccountId, Hbar> hbarTransfers = transferTransaction.getHbarTransfers();
  const std::unordered_map<TokenId, std::unordered_map<AccountId, int64_t>> tokenTransfers =
    transferTransaction.getTokenTransfers();
  const std::unordered_map<TokenId, std::vector<TokenNftTransfer>> nftTransfers = transferTransaction.getNftTransfers();
  const std::unordered_map<TokenId, uint32_t> tokenDecimals = transferTransaction.getTokenIdDecimals();

  ASSERT_EQ(hbarTransfers.size(), 1);
  EXPECT_EQ(hbarTransfers.cbegin()->first, getTestAccountId1());
  EXPECT_EQ(hbarTransfers.cbegin()->second, getTestAmount());

  ASSERT_EQ(tokenTransfers.size(), 1);
  EXPECT_EQ(tokenTransfers.cbegin()->first, getTestTokenId());
  EXPECT_EQ(tokenTransfers.cbegin()->second.cbegin()->first, getTestAccountId2());
  EXPECT_EQ(tokenTransfers.cbegin()->second.cbegin()->second, getTestAmount().toTinybars());

  ASSERT_EQ(nftTransfers.size(), 1);
  EXPECT_EQ(nftTransfers.cbegin()->first, getTestNftId().getTokenId());
  EXPECT_EQ(nftTransfers.cbegin()->second.size(), 1);
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getNftId(), getTestNftId());
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getSenderAccountId(), getTestAccountId1());
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getReceiverAccountId(), getTestAccountId2());
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getApproval(), getTestApproval());
}

//-----
TEST_F(TransferTransactionTest, AddHbarTransfer)
{
  TransferTransaction transaction;
  transaction.addHbarTransfer(getTestAccountId1(), getTestAmount());
  EXPECT_FALSE(transaction.getHbarTransfers().empty());
  EXPECT_EQ(transaction.getHbarTransfers().cbegin()->first, getTestAccountId1());
  EXPECT_EQ(transaction.getHbarTransfers().cbegin()->second, getTestAmount());

  transaction.addHbarTransfer(getTestAccountId1(), getTestAmount());
  EXPECT_FALSE(transaction.getHbarTransfers().empty());
  EXPECT_EQ(transaction.getHbarTransfers().cbegin()->first, getTestAccountId1());
  EXPECT_EQ(transaction.getHbarTransfers().cbegin()->second, getTestAmount() + getTestAmount());

  transaction.addHbarTransfer(getTestAccountId1(), (getTestAmount() + getTestAmount()).negated());
  EXPECT_TRUE(transaction.getHbarTransfers().empty());
}

//-----
TEST_F(TransferTransactionTest, AddTokenTransfer)
{
  TransferTransaction transaction;
  transaction.addTokenTransfer(getTestTokenId(), getTestAccountId2(), getTestAmount().toTinybars());
  EXPECT_FALSE(transaction.getTokenTransfers().empty());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->first, getTestTokenId());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->second.cbegin()->first, getTestAccountId2());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->second.cbegin()->second, getTestAmount().toTinybars());
  EXPECT_TRUE(transaction.getTokenIdDecimals().empty());

  transaction.addTokenTransfer(getTestTokenId(), getTestAccountId2(), getTestAmount().toTinybars());
  EXPECT_FALSE(transaction.getTokenTransfers().empty());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->first, getTestTokenId());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->second.cbegin()->first, getTestAccountId2());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->second.cbegin()->second,
            (getTestAmount() + getTestAmount()).toTinybars());
  EXPECT_TRUE(transaction.getTokenIdDecimals().empty());

  transaction.addTokenTransfer(
    getTestTokenId(), getTestAccountId2(), (getTestAmount() + getTestAmount()).negated().toTinybars());
  EXPECT_TRUE(transaction.getTokenTransfers().empty());
}

//-----
TEST_F(TransferTransactionTest, AddNftTransfer)
{
  TransferTransaction transaction;
  transaction.addNftTransfer(getTestNftId(), getTestAccountId1(), getTestAccountId2());
  EXPECT_FALSE(transaction.getNftTransfers().empty());
  EXPECT_EQ(transaction.getNftTransfers().cbegin()->first, getTestTokenId());
  EXPECT_EQ(transaction.getNftTransfers().cbegin()->second.cbegin()->getSenderAccountId(), getTestAccountId1());
  EXPECT_EQ(transaction.getNftTransfers().cbegin()->second.cbegin()->getReceiverAccountId(), getTestAccountId2());
  EXPECT_EQ(transaction.getNftTransfers().cbegin()->second.cbegin()->getNftId().getSerialNum(),
            getTestNftId().getSerialNum());

  transaction.addNftTransfer(getTestNftId(), getTestAccountId2(), getTestAccountId1());
  EXPECT_TRUE(transaction.getNftTransfers().empty());
}

//-----
TEST_F(TransferTransactionTest, AddTokenTransferWithDecimals)
{
  TransferTransaction transaction;
  transaction.addTokenTransferWithDecimals(
    getTestTokenId(), getTestAccountId2(), getTestAmount().toTinybars(), getTestExpectedDecimals());
  EXPECT_FALSE(transaction.getTokenTransfers().empty());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->first, getTestTokenId());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->second.cbegin()->first, getTestAccountId2());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->second.cbegin()->second, getTestAmount().toTinybars());
  EXPECT_FALSE(transaction.getTokenIdDecimals().empty());
  EXPECT_EQ(transaction.getTokenIdDecimals().cbegin()->first, getTestTokenId());
  EXPECT_EQ(transaction.getTokenIdDecimals().cbegin()->second, getTestExpectedDecimals());

  transaction.addTokenTransferWithDecimals(
    getTestTokenId(), getTestAccountId2(), getTestAmount().toTinybars(), getTestExpectedDecimals());
  EXPECT_FALSE(transaction.getTokenTransfers().empty());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->first, getTestTokenId());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->second.cbegin()->first, getTestAccountId2());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->second.cbegin()->second,
            (getTestAmount() + getTestAmount()).toTinybars());
  EXPECT_FALSE(transaction.getTokenIdDecimals().empty());
  EXPECT_EQ(transaction.getTokenIdDecimals().cbegin()->first, getTestTokenId());
  EXPECT_EQ(transaction.getTokenIdDecimals().cbegin()->second, getTestExpectedDecimals());

  EXPECT_THROW(transaction.addTokenTransferWithDecimals(
                 getTestTokenId(), getTestAccountId2(), getTestAmount().toTinybars(), getTestExpectedDecimals() + 1U),
               std::invalid_argument);

  transaction.addTokenTransferWithDecimals(getTestTokenId(),
                                           getTestAccountId2(),
                                           (getTestAmount() + getTestAmount()).negated().toTinybars(),
                                           getTestExpectedDecimals());
  EXPECT_TRUE(transaction.getTokenTransfers().empty());
  EXPECT_TRUE(transaction.getTokenIdDecimals().empty());
}
