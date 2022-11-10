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
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const Hbar& getTestAmount() const { return mAmount; }

private:
  const AccountId mAccountId = AccountId(10ULL);
  const Hbar mAmount = Hbar(10ULL);
};

TEST_F(TransferTransactionTest, ConstructAccountCreateTransaction)
{
  TransferTransaction transaction;
  EXPECT_TRUE(transaction.getHbarTransfers().empty());
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
