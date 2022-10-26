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

class AccountIdTest : public ::testing::Test {
protected:
  void SetUp() override {}
};

TEST_F(AccountIdTest, AccountNumberConstructor) {
  uint64_t accountNumber = 555;

  std::unique_ptr<Hedera::AccountId> accountId =
      std::make_unique<Hedera::AccountId>(accountNumber);

  ASSERT_NE(accountId, nullptr);
  ASSERT_EQ(accountId->getAccountNum(), accountNumber);
  ASSERT_EQ(accountId->getRealmNum(), 0);
  ASSERT_EQ(accountId->getShardNum(), 0);
}

TEST_F(AccountIdTest, RealmShardConstructor) {
  uint64_t realmNumber = 666;
  uint64_t shardNumber = 777;

  std::unique_ptr<Hedera::AccountId> accountId =
      std::make_unique<Hedera::AccountId>(shardNumber, realmNumber);

  ASSERT_NE(accountId, nullptr);
  ASSERT_FALSE(accountId->getAccountNum().has_value());
  ASSERT_EQ(accountId->getRealmNum(), realmNumber);
  ASSERT_EQ(accountId->getShardNum(), shardNumber);
}

TEST_F(AccountIdTest, AccountRealmShardConstructor) {
  uint64_t accountNumber = 555;
  uint64_t realmNumber = 666;
  uint64_t shardNumber = 777;

  std::unique_ptr<Hedera::AccountId> accountId =
      std::make_unique<Hedera::AccountId>(shardNumber, realmNumber,
                                          accountNumber);

  ASSERT_NE(accountId, nullptr);
  ASSERT_EQ(accountId->getAccountNum(), accountNumber);
  ASSERT_EQ(accountId->getRealmNum(), realmNumber);
  ASSERT_EQ(accountId->getShardNum(), shardNumber);
}

TEST_F(AccountIdTest, SetAccountNum) {
  std::unique_ptr<Hedera::AccountId> accountId =
      std::make_unique<Hedera::AccountId>(333);

  uint64_t newAccountNumber = 555;
  accountId->setAccountNum(newAccountNumber);

  ASSERT_EQ(accountId->getAccountNum(), newAccountNumber);
}

TEST_F(AccountIdTest, SetRealmNum) {
  std::unique_ptr<Hedera::AccountId> accountId =
      std::make_unique<Hedera::AccountId>(22, 33, 44);

  uint64_t newRealmNumber = 55;
  accountId->setRealmNum(newRealmNumber);

  ASSERT_EQ(accountId->getRealmNum(), newRealmNumber);
}

TEST_F(AccountIdTest, SetShardNum) {
  std::unique_ptr<Hedera::AccountId> accountId =
      std::make_unique<Hedera::AccountId>(22, 33, 44);

  uint64_t newShardNum = 66;
  accountId->setShardNum(newShardNum);

  ASSERT_EQ(accountId->getShardNum(), newShardNum);
}

TEST_F(AccountIdTest, Equality) {
  uint64_t accountNumber = 555;
  uint64_t realmNumber = 666;
  uint64_t shardNumber = 777;

  std::unique_ptr<Hedera::AccountId> accountIdOriginal =
      std::make_unique<Hedera::AccountId>(accountNumber, realmNumber,
                                          shardNumber);
  std::unique_ptr<Hedera::AccountId> accountIdDuplicate =
      std::make_unique<Hedera::AccountId>(accountNumber, realmNumber,
                                          shardNumber);

  ASSERT_TRUE(*accountIdOriginal.get() == *accountIdDuplicate.get());

  std::unique_ptr<Hedera::AccountId> accountIdDifferentAccount =
      std::make_unique<Hedera::AccountId>(0, realmNumber, shardNumber);

  ASSERT_FALSE(*accountIdOriginal.get() == *accountIdDifferentAccount.get());

  std::unique_ptr<Hedera::AccountId> accountIdDifferentRealm =
      std::make_unique<Hedera::AccountId>(accountNumber, 0, shardNumber);

  ASSERT_FALSE(*accountIdOriginal.get() == *accountIdDifferentRealm.get());

  std::unique_ptr<Hedera::AccountId> accountIdDifferentShard =
      std::make_unique<Hedera::AccountId>(accountNumber, realmNumber, 0);

  ASSERT_FALSE(*accountIdOriginal.get() == *accountIdDifferentShard.get());
}

TEST_F(AccountIdTest, Protobuf) {
  std::unique_ptr<Hedera::AccountId> accountId =
      std::make_unique<Hedera::AccountId>(22, 33, 44);

  Hedera::AccountId accountIdThroughProtobuf = Hedera::AccountId::fromProtobuf(*accountId->toProtobuf());

  ASSERT_EQ(*accountId.get(), accountIdThroughProtobuf);
}
