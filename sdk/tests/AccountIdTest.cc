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

#include <gtest/gtest.h>
#include <memory>

#include "AccountId.h"

using namespace Hedera;

class AccountIdTest : public ::testing::Test {
protected:
  uint64_t realmNumber = 555;
  uint64_t shardNumber = 666;
  uint64_t accountNumber = 777;

  void SetUp() override {}
};

TEST_F(AccountIdTest, AccountNumberConstructor) {
  std::unique_ptr<AccountId> accountId =
      std::make_unique<AccountId>(accountNumber);

  ASSERT_NE(accountId, nullptr);

  ASSERT_EQ(accountId->getShardNum(), 0);
  ASSERT_EQ(accountId->getRealmNum(), 0);
  ASSERT_EQ(accountId->getAccountNum(), accountNumber);
}

TEST_F(AccountIdTest, RealmShardConstructor) {
  std::unique_ptr<AccountId> accountId =
      std::make_unique<AccountId>(shardNumber, realmNumber);

  ASSERT_NE(accountId, nullptr);

  ASSERT_EQ(accountId->getShardNum(), shardNumber);
  ASSERT_EQ(accountId->getRealmNum(), realmNumber);
  ASSERT_FALSE(accountId->getAccountNum().has_value());
}

TEST_F(AccountIdTest, AccountRealmShardConstructor) {
  std::unique_ptr<AccountId> accountId =
      std::make_unique<AccountId>(shardNumber, realmNumber, accountNumber);

  ASSERT_NE(accountId, nullptr);
  ASSERT_EQ(accountId->getShardNum(), shardNumber);
  ASSERT_EQ(accountId->getRealmNum(), realmNumber);
  ASSERT_EQ(accountId->getAccountNum(), accountNumber);
}

TEST_F(AccountIdTest, SetAccountNum) {
  std::unique_ptr<AccountId> accountId =
      std::make_unique<AccountId>(accountNumber);

  uint64_t newAccountNumber = 11;
  accountId->setAccountNum(newAccountNumber);

  ASSERT_EQ(accountId->getAccountNum(), newAccountNumber);
}

TEST_F(AccountIdTest, SetRealmNum) {
  std::unique_ptr<AccountId> accountId =
      std::make_unique<AccountId>(shardNumber, realmNumber, accountNumber);

  uint64_t newRealmNumber = 22;
  accountId->setRealmNum(newRealmNumber);

  ASSERT_EQ(accountId->getRealmNum(), newRealmNumber);
}

TEST_F(AccountIdTest, SetShardNum) {
  std::unique_ptr<AccountId> accountId =
      std::make_unique<AccountId>(shardNumber, realmNumber, accountNumber);

  uint64_t newShardNum = 33;
  accountId->setShardNum(newShardNum);

  ASSERT_EQ(accountId->getShardNum(), newShardNum);
}

TEST_F(AccountIdTest, Equality) {
  std::unique_ptr<AccountId> accountIdOriginal =
      std::make_unique<AccountId>(shardNumber, realmNumber, accountNumber);
  std::unique_ptr<AccountId> accountIdDuplicate =
      std::make_unique<AccountId>(shardNumber, realmNumber, accountNumber);

  ASSERT_TRUE(*accountIdOriginal.get() == *accountIdDuplicate.get());

  std::unique_ptr<AccountId> accountIdDifferentShard =
      std::make_unique<AccountId>(0, realmNumber, accountNumber);

  ASSERT_FALSE(*accountIdOriginal.get() == *accountIdDifferentShard.get());

  std::unique_ptr<AccountId> accountIdDifferentRealm =
      std::make_unique<AccountId>(shardNumber, 0, accountNumber);

  ASSERT_FALSE(*accountIdOriginal.get() == *accountIdDifferentRealm.get());

  std::unique_ptr<AccountId> accountIdDifferentAccount =
      std::make_unique<AccountId>(shardNumber, realmNumber, 0);

  ASSERT_FALSE(*accountIdOriginal.get() == *accountIdDifferentAccount.get());
}

TEST_F(AccountIdTest, Protobuf) {
  std::unique_ptr<AccountId> accountId =
      std::make_unique<AccountId>(shardNumber, realmNumber, accountNumber);

  AccountId accountIdThroughProtobuf =
      AccountId::fromProtobuf(*accountId->toProtobuf());

  ASSERT_EQ(*accountId.get(), accountIdThroughProtobuf);
}
