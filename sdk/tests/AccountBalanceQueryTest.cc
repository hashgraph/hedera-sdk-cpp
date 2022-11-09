/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
  const AccountId mTestAccountId = AccountId(0ULL, 0ULL, 3ULL);
  const ContractId mTestContractId = ContractId(0ULL, 0ULL, 3ULL);
};

TEST_F(AccountBalanceQueryTest, ConstructAccountBalanceQuery)
{
  AccountBalanceQuery query;
  EXPECT_EQ(query.getAccountId(), nullptr);
  EXPECT_EQ(query.getContractId(), nullptr);
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

  EXPECT_EQ(query.getAccountId(), nullptr);

  query.setAccountId(getTestAccountId());

  EXPECT_EQ(query.getContractId(), nullptr);
}
