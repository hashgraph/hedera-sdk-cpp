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

using namespace Hedera;

class TransferTransactionTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId1() const { return mAccountId1; }
  [[nodiscard]] inline const AccountId& getTestAccountId2() const { return mAccountId2; }
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTokenId; }
  [[nodiscard]] inline const uint64_t& getTestNftSerialNumber() const { return mSerialNumber; }
  [[nodiscard]] inline const NftId& getTestNftId() const { return mNftId; }
  [[nodiscard]] inline const Hbar& getTestAmount() const { return mAmount; }
  [[nodiscard]] inline uint32_t getTestExpectedDecimals() const { return mExpectedDecimals; }

private:
  const AccountId mAccountId1 = AccountId(10ULL);
  const AccountId mAccountId2 = AccountId(20ULL);
  const TokenId mTokenId = TokenId(30ULL);
  const uint64_t mSerialNumber = 40ULL;
  const NftId mNftId = NftId(mTokenId, mSerialNumber);
  const Hbar mAmount = Hbar(50ULL);
  const uint32_t mExpectedDecimals = 6U;
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
TEST_F(TransferTransactionTest, CloneTransferTransaction)
{
  TransferTransaction transaction;
  const std::string memo = "this is a test memo";
  transaction.setNodeAccountIds({ getTestAccountId1() });
  transaction.setTransactionMemo(memo);
  transaction.addHbarTransfer(getTestAccountId1(), getTestAmount());

  auto clonedExecutableTransactionPtr = transaction.clone();
  EXPECT_EQ(clonedExecutableTransactionPtr->getNodeAccountIds().size(), transaction.getNodeAccountIds().size());
  EXPECT_EQ(clonedExecutableTransactionPtr->getNodeAccountIds().at(0), getTestAccountId1());

  auto clonedTransactionPtr = dynamic_cast<Transaction<TransferTransaction>*>(clonedExecutableTransactionPtr.get());
  EXPECT_NE(clonedTransactionPtr, nullptr);
  EXPECT_EQ(clonedTransactionPtr->getTransactionMemo(), memo);

  auto clonedAccountCreateTransactionPtr = dynamic_cast<TransferTransaction*>(clonedTransactionPtr);
  EXPECT_NE(clonedAccountCreateTransactionPtr, nullptr);
  EXPECT_FALSE(clonedAccountCreateTransactionPtr->getHbarTransfers().empty());
  EXPECT_EQ(clonedAccountCreateTransactionPtr->getHbarTransfers().begin()->first, getTestAccountId1());
  EXPECT_EQ(clonedAccountCreateTransactionPtr->getHbarTransfers().begin()->second, getTestAmount());
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
  EXPECT_EQ(transaction.getNftTransfers().cbegin()->second.cbegin()->getNftSerialNumber(), getTestNftSerialNumber());

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
