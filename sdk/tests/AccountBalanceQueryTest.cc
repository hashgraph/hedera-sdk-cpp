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
#include "AccountBalanceQuery.h"
#include "AccountId.h"

#include <gtest/gtest.h>

using namespace Hedera;

class AccountBalanceQueryTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const ContractId& getTestContractId() const { return mTestContractId; }

private:
  const AccountId mTestAccountId = AccountId(3ULL);
  const ContractId mTestContractId = ContractId(3ULL);
};

TEST_F(AccountBalanceQueryTest, ConstructAccountBalanceQuery)
{
  AccountBalanceQuery query;
  EXPECT_FALSE(query.getAccountId());
  EXPECT_FALSE(query.getContractId());
}

TEST_F(AccountBalanceQueryTest, CloneAccountBalanceQuery)
{
  AccountBalanceQuery accountBalanceQuery;
  accountBalanceQuery.setNodeAccountIds({ getTestAccountId() });
  accountBalanceQuery.setAccountId(getTestAccountId());

  auto clonedExecutablePtr = accountBalanceQuery.clone();
  EXPECT_EQ(clonedExecutablePtr->getNodeAccountIds().size(), accountBalanceQuery.getNodeAccountIds().size());
  EXPECT_EQ(clonedExecutablePtr->getNodeAccountIds().at(0), getTestAccountId());

  // TODO: get and test Query derived class members when they're added

  auto clonedAccountBalanceQueryPtr = dynamic_cast<AccountBalanceQuery*>(clonedExecutablePtr.get());
  EXPECT_TRUE(clonedAccountBalanceQueryPtr->getAccountId());
  EXPECT_EQ(*clonedAccountBalanceQueryPtr->getAccountId(), getTestAccountId());
}

TEST_F(AccountBalanceQueryTest, SetAccountId)
{
  AccountBalanceQuery query;
  query.setAccountId(getTestAccountId());

  EXPECT_EQ(*query.getAccountId(), getTestAccountId());
}

TEST_F(AccountBalanceQueryTest, SetContractId)
{
  AccountBalanceQuery query;
  query.setContractId(getTestContractId());

  EXPECT_EQ(*query.getContractId(), getTestContractId());
}

TEST_F(AccountBalanceQueryTest, ResetMutuallyExclusiveIds)
{
  AccountBalanceQuery query;
  query.setAccountId(getTestAccountId());
  query.setContractId(getTestContractId());

  EXPECT_FALSE(query.getAccountId());

  query.setAccountId(getTestAccountId());

  EXPECT_FALSE(query.getContractId());
}
