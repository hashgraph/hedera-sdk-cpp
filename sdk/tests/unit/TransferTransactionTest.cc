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
#include "TransferTransaction.h"
#include "AccountId.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "Hbar.h"
#include "TokenId.h"
#include "exceptions/IllegalStateException.h"

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
  // Given / When
  TransferTransaction transaction;

  // Then
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
  // Given
  TransferTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.addHbarTransfer(getTestAccountId1(), getTestAmount()));

  // THEN
  EXPECT_FALSE(transaction.getHbarTransfers().empty());
  EXPECT_EQ(transaction.getHbarTransfers().cbegin()->first, getTestAccountId1());
  EXPECT_EQ(transaction.getHbarTransfers().cbegin()->second, getTestAmount());
}

//-----
TEST_F(TransferTransactionTest, AddHbarTransferFrozen)
{
  // Given
  TransferTransaction transaction = TransferTransaction()
                                      .setNodeAccountIds({ AccountId(1ULL) })
                                      .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.addHbarTransfer(getTestAccountId1(), getTestAmount()), IllegalStateException);
}

//-----
TEST_F(TransferTransactionTest, RemoveHbarTransferIfAmountEqualsZero)
{
  // Given
  TransferTransaction transaction;
  ASSERT_NO_THROW(transaction.addHbarTransfer(getTestAccountId1(), getTestAmount()));

  // When
  EXPECT_NO_THROW(transaction.addHbarTransfer(getTestAccountId1(), getTestAmount().negated()));

  // Then
  EXPECT_TRUE(transaction.getHbarTransfers().empty());
}

//-----
TEST_F(TransferTransactionTest, AddTokenTransfer)
{
  // Given
  TransferTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.addTokenTransfer(getTestTokenId(), getTestAccountId2(), getTestAmount().toTinybars()));

  // Then
  ASSERT_FALSE(transaction.getTokenTransfers().empty());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->first, getTestTokenId());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->second.cbegin()->first, getTestAccountId2());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->second.cbegin()->second, getTestAmount().toTinybars());
  EXPECT_TRUE(transaction.getTokenIdDecimals().empty());
}

//-----
TEST_F(TransferTransactionTest, AddTokenTransferFrozen)
{
  // Given
  TransferTransaction transaction = TransferTransaction()
                                      .setNodeAccountIds({ AccountId(1ULL) })
                                      .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.addTokenTransfer(getTestTokenId(), getTestAccountId1(), getTestAmount().toTinybars()),
               IllegalStateException);
}

//-----
TEST_F(TransferTransactionTest, RemoveTokenTransferIfAmountEqualsZero)
{
  // Given
  TransferTransaction transaction;
  ASSERT_NO_THROW(transaction.addTokenTransfer(getTestTokenId(), getTestAccountId1(), getTestAmount().toTinybars()));

  // When
  EXPECT_NO_THROW(
    transaction.addTokenTransfer(getTestTokenId(), getTestAccountId1(), getTestAmount().negated().toTinybars()));

  // Then
  EXPECT_TRUE(transaction.getTokenTransfers().empty());
}

//-----
TEST_F(TransferTransactionTest, AddNftTransfer)
{
  // Given
  TransferTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.addNftTransfer(getTestNftId(), getTestAccountId1(), getTestAccountId2()));

  // Then
  ASSERT_FALSE(transaction.getNftTransfers().empty());
  EXPECT_EQ(transaction.getNftTransfers().cbegin()->first, getTestTokenId());
  EXPECT_EQ(transaction.getNftTransfers().cbegin()->second.cbegin()->getSenderAccountId(), getTestAccountId1());
  EXPECT_EQ(transaction.getNftTransfers().cbegin()->second.cbegin()->getReceiverAccountId(), getTestAccountId2());
  EXPECT_EQ(transaction.getNftTransfers().cbegin()->second.cbegin()->getNftId().getSerialNum(),
            getTestNftId().getSerialNum());
}

//-----
TEST_F(TransferTransactionTest, AddNftTransferFrozen)
{
  // Given
  TransferTransaction transaction = TransferTransaction()
                                      .setNodeAccountIds({ AccountId(1ULL) })
                                      .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.addNftTransfer(getTestNftId(), getTestAccountId1(), getTestAccountId2()),
               IllegalStateException);
}

//-----
TEST_F(TransferTransactionTest, RemoveNftTransferIfAmountEqualsZero)
{
  // Given
  TransferTransaction transaction;
  ASSERT_NO_THROW(transaction.addNftTransfer(getTestNftId(), getTestAccountId1(), getTestAccountId2()));

  // When
  EXPECT_NO_THROW(transaction.addNftTransfer(getTestNftId(), getTestAccountId2(), getTestAccountId1()));

  // Then
  EXPECT_TRUE(transaction.getNftTransfers().empty());
}

//-----
TEST_F(TransferTransactionTest, AddTokenTransferWithDecimals)
{
  // Given
  TransferTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.addTokenTransferWithDecimals(
    getTestTokenId(), getTestAccountId2(), getTestAmount().toTinybars(), getTestExpectedDecimals()));

  // Then
  EXPECT_FALSE(transaction.getTokenTransfers().empty());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->first, getTestTokenId());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->second.cbegin()->first, getTestAccountId2());
  EXPECT_EQ(transaction.getTokenTransfers().cbegin()->second.cbegin()->second, getTestAmount().toTinybars());
  EXPECT_FALSE(transaction.getTokenIdDecimals().empty());
  EXPECT_EQ(transaction.getTokenIdDecimals().cbegin()->first, getTestTokenId());
  EXPECT_EQ(transaction.getTokenIdDecimals().cbegin()->second, getTestExpectedDecimals());
}

//-----
TEST_F(TransferTransactionTest, AddTokenTransferWithDecimalsFrozen)
{
  // Given
  TransferTransaction transaction = TransferTransaction()
                                      .setNodeAccountIds({ AccountId(1ULL) })
                                      .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.addTokenTransferWithDecimals(getTestTokenId(),
                                                        getTestAccountId2(),
                                                        (getTestAmount() + getTestAmount()).negated().toTinybars(),
                                                        getTestExpectedDecimals()),
               IllegalStateException);
}

//-----
TEST_F(TransferTransactionTest, ThrowIfDecimalsDoNotMatch)
{
  // Given
  TransferTransaction transaction;
  ASSERT_NO_THROW(transaction.addTokenTransferWithDecimals(
    getTestTokenId(), getTestAccountId1(), getTestAmount().toTinybars(), getTestExpectedDecimals()));

  // When / Then
  EXPECT_THROW(transaction.addTokenTransferWithDecimals(
                 getTestTokenId(), getTestAccountId1(), getTestAmount().toTinybars(), getTestExpectedDecimals() + 1U),
               std::invalid_argument);
}

//-----
TEST_F(TransferTransactionTest, RemoveTokenWithDecimalsTransferIfAmountEqualsZero)
{
  // Given
  TransferTransaction transaction;
  ASSERT_NO_THROW(transaction.addTokenTransferWithDecimals(
    getTestTokenId(), getTestAccountId1(), getTestAmount().toTinybars(), getTestExpectedDecimals()));

  // When
  EXPECT_NO_THROW(transaction.addTokenTransferWithDecimals(
    getTestTokenId(), getTestAccountId1(), getTestAmount().negated().toTinybars(), getTestExpectedDecimals()));

  // Then
  EXPECT_TRUE(transaction.getTokenTransfers().empty());
  EXPECT_TRUE(transaction.getTokenIdDecimals().empty());
}
