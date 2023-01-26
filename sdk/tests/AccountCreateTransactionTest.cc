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
#include "AccountCreateTransaction.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "PublicKey.h"

#include <gtest/gtest.h>

using namespace Hedera;

class AccountCreateTransactionTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKey() const { return mPublicKey; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const uint64_t& getTestNodeId() const { return mNodeId; }

private:
  const std::shared_ptr<PublicKey> mPublicKey = ED25519PrivateKey::generatePrivateKey()->getPublicKey();
  const AccountId mAccountId = AccountId(10ULL);
  const uint64_t mNodeId = 10ULL;
};

TEST_F(AccountCreateTransactionTest, ConstructAccountCreateTransaction)
{
  AccountCreateTransaction transaction;
  EXPECT_EQ(transaction.getKey(), nullptr);
  EXPECT_EQ(transaction.getInitialBalance(), Hbar(0ULL));
  EXPECT_FALSE(transaction.getReceiverSignatureRequired());
  EXPECT_EQ(transaction.getAutoRenewPeriod(), std::chrono::hours(2160)); // 90 days
  EXPECT_EQ(transaction.getTransactionMemo(), std::string());
  EXPECT_EQ(transaction.getMaxAutomaticTokenAssociations(), 0U);
  EXPECT_FALSE(transaction.getStakedAccountId());
  EXPECT_FALSE(transaction.getStakedNodeId());
  EXPECT_FALSE(transaction.getDeclineStakingReward());
  EXPECT_EQ(transaction.getAlias(), nullptr);
}

TEST_F(AccountCreateTransactionTest, CloneAccountCreateTransaction)
{
  AccountCreateTransaction transaction;
  const std::string memo = "this is a test memo";
  transaction.setNodeAccountIds({ getTestAccountId() });
  transaction.setTransactionMemo(memo);
  transaction.setStakedAccountId(getTestAccountId());

  auto clonedExecutableTransactionPtr = transaction.clone();
  EXPECT_EQ(clonedExecutableTransactionPtr->getNodeAccountIds().size(), transaction.getNodeAccountIds().size());
  EXPECT_EQ(clonedExecutableTransactionPtr->getNodeAccountIds().at(0), getTestAccountId());

  auto clonedTransactionPtr =
    dynamic_cast<Transaction<AccountCreateTransaction>*>(clonedExecutableTransactionPtr.get());
  EXPECT_NE(clonedTransactionPtr, nullptr);
  EXPECT_EQ(clonedTransactionPtr->getTransactionMemo(), memo);

  auto clonedAccountCreateTransactionPtr = dynamic_cast<AccountCreateTransaction*>(clonedTransactionPtr);
  EXPECT_NE(clonedAccountCreateTransactionPtr, nullptr);
  EXPECT_TRUE(clonedAccountCreateTransactionPtr->getStakedAccountId());
  EXPECT_EQ(*clonedAccountCreateTransactionPtr->getStakedAccountId(), getTestAccountId());
}

TEST_F(AccountCreateTransactionTest, SetKey)
{
  AccountCreateTransaction transaction;
  transaction.setKey(getTestPublicKey());
  EXPECT_EQ(transaction.getKey()->toString(), getTestPublicKey()->toString());
}

TEST_F(AccountCreateTransactionTest, SetInitialBalance)
{
  AccountCreateTransaction transaction;
  const auto initialBalance = Hbar(3ULL);
  transaction.setInitialBalance(initialBalance);

  EXPECT_EQ(transaction.getInitialBalance(), initialBalance);
}

TEST_F(AccountCreateTransactionTest, SetReceiverSignatureRequired)
{
  AccountCreateTransaction transaction;
  transaction.setReceiverSignatureRequired(true);
  EXPECT_TRUE(transaction.getReceiverSignatureRequired());
}

TEST_F(AccountCreateTransactionTest, SetAutoRenewPeriod)
{
  AccountCreateTransaction transaction;
  const auto duration = std::chrono::hours(240); // 10 days
  transaction.setAutoRenewPeriod(duration);

  EXPECT_EQ(transaction.getAutoRenewPeriod(), duration);
}

TEST_F(AccountCreateTransactionTest, SetAccountMemo)
{
  AccountCreateTransaction transaction;
  const std::string memo = "transaction test";
  transaction.setAccountMemo(memo);

  EXPECT_EQ(transaction.getAccountMemo(), memo);
  // Throw if account memo is larger than 100 characters
  EXPECT_THROW(transaction.setAccountMemo(std::string(101, 'a')), std::invalid_argument);
}

TEST_F(AccountCreateTransactionTest, SetMaxAutomaticTokenAssociations)
{
  AccountCreateTransaction transaction;
  const uint32_t associations = 5U;
  transaction.setMaxAutomaticTokenAssociations(associations);

  EXPECT_EQ(transaction.getMaxAutomaticTokenAssociations(), associations);

  // Throw if over 5000
  EXPECT_NO_THROW(transaction.setMaxAutomaticTokenAssociations(5000U));
  EXPECT_THROW(transaction.setMaxAutomaticTokenAssociations(5001U), std::invalid_argument);
  EXPECT_THROW(transaction.setMaxAutomaticTokenAssociations(std::numeric_limits<uint32_t>::max()),
               std::invalid_argument);
}

TEST_F(AccountCreateTransactionTest, SetStakedAccountId)
{
  AccountCreateTransaction transaction;
  transaction.setStakedAccountId(getTestAccountId());
  EXPECT_EQ(*transaction.getStakedAccountId(), getTestAccountId());
}

TEST_F(AccountCreateTransactionTest, SetStakedNodeId)
{
  AccountCreateTransaction transaction;
  transaction.setStakedNodeId(getTestNodeId());
  EXPECT_EQ(*transaction.getStakedNodeId(), getTestNodeId());
}

TEST_F(AccountCreateTransactionTest, SetStakingRewardPolicy)
{
  AccountCreateTransaction transaction;
  transaction.setDeclineStakingReward(true);
  EXPECT_TRUE(transaction.getDeclineStakingReward());
}

TEST_F(AccountCreateTransactionTest, SetAlias)
{
  AccountCreateTransaction transaction;
  transaction.setAlias(getTestPublicKey());
  EXPECT_EQ(transaction.getAlias()->toString(), getTestPublicKey()->toString());
}

TEST_F(AccountCreateTransactionTest, ResetMutuallyExclusiveIds)
{
  AccountCreateTransaction transaction;
  transaction.setStakedAccountId(getTestAccountId());
  transaction.setStakedNodeId(getTestNodeId());

  EXPECT_FALSE(transaction.getStakedAccountId());
  EXPECT_TRUE(transaction.getStakedNodeId());

  transaction.setStakedAccountId(getTestAccountId());
  EXPECT_TRUE(transaction.getStakedAccountId());
  EXPECT_FALSE(transaction.getStakedNodeId());
}
