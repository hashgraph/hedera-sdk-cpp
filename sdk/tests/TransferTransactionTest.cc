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

#include <gtest/gtest.h>

using namespace Hedera;

class TransferTransactionTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::shared_ptr<AccountId>& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const Hbar& getTestAmount() const { return mAmount; }

private:
  const std::shared_ptr<AccountId> mAccountId = std::make_shared<AccountId>(10ULL);
  const Hbar mAmount = Hbar(10ULL);
};

TEST_F(TransferTransactionTest, ConstructTransferTransaction)
{
  TransferTransaction transaction;
  EXPECT_TRUE(transaction.getHbarTransfers().empty());
}

TEST_F(TransferTransactionTest, CloneTransferTransaction)
{
  TransferTransaction transaction;
  const std::string memo = "this is a test memo";
  transaction.setNodeAccountIds({ getTestAccountId() });
  transaction.setTransactionMemo(memo);
  transaction.addUnapprovedHbarTransfer(getTestAccountId(), getTestAmount());

  auto clonedExecutableTransactionPtr = transaction.clone();
  EXPECT_EQ(clonedExecutableTransactionPtr->getNodeAccountIds().size(), transaction.getNodeAccountIds().size());
  EXPECT_EQ(*clonedExecutableTransactionPtr->getNodeAccountIds().at(0), *getTestAccountId());

  auto clonedTransactionPtr = dynamic_cast<Transaction<TransferTransaction>*>(clonedExecutableTransactionPtr.get());
  EXPECT_NE(clonedTransactionPtr, nullptr);
  EXPECT_EQ(clonedTransactionPtr->getTransactionMemo(), memo);

  auto clonedAccountCreateTransactionPtr = dynamic_cast<TransferTransaction*>(clonedTransactionPtr);
  EXPECT_NE(clonedAccountCreateTransactionPtr, nullptr);
  EXPECT_FALSE(clonedAccountCreateTransactionPtr->getHbarTransfers().empty());
  EXPECT_EQ(*clonedAccountCreateTransactionPtr->getHbarTransfers().at(0).getAccountId(), *getTestAccountId());
  EXPECT_EQ(clonedAccountCreateTransactionPtr->getHbarTransfers().at(0).getAmount(), getTestAmount());
}

TEST_F(TransferTransactionTest, AddApproved)
{
  TransferTransaction transaction;
  transaction.addApprovedHbarTransfer(getTestAccountId(), getTestAmount());
  EXPECT_TRUE(transaction.getHbarTransfers().at(0).getApproval());
}

TEST_F(TransferTransactionTest, AddUnapproved)
{
  TransferTransaction transaction;
  transaction.addUnapprovedHbarTransfer(getTestAccountId(), getTestAmount());
  EXPECT_FALSE(transaction.getHbarTransfers().at(0).getApproval());
}
