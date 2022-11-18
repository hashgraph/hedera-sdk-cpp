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
#include "AccountId.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class AccountIdTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const uint64_t& getTestShardNum() const { return mShardNum; }
  [[nodiscard]] inline const uint64_t& getTestRealmNum() const { return mRealmNum; }
  [[nodiscard]] inline const uint64_t& getTestAccountNum() const { return mAccountNum; }

private:
  const uint64_t mShardNum = 8ULL;
  const uint64_t mRealmNum = 9ULL;
  const uint64_t mAccountNum = 10ULL;
};

TEST_F(AccountIdTest, DefaultConstructaccountId)
{
  AccountId accountId;
  EXPECT_EQ(accountId.getShardNum(), 0ULL);
  EXPECT_EQ(accountId.getRealmNum(), 0ULL);
  EXPECT_EQ(accountId.getAccountNum(), 0ULL);
}

TEST_F(AccountIdTest, ConstructWithaccountnum)
{
  AccountId accountId(getTestAccountNum());
  EXPECT_EQ(accountId.getShardNum(), 0ULL);
  EXPECT_EQ(accountId.getRealmNum(), 0ULL);
  EXPECT_EQ(accountId.getAccountNum(), getTestAccountNum());
}

TEST_F(AccountIdTest, ConstructWithShardRealmaccountnum)
{
  AccountId accountId(getTestShardNum(), getTestRealmNum(), getTestAccountNum());
  EXPECT_EQ(accountId.getShardNum(), getTestShardNum());
  EXPECT_EQ(accountId.getRealmNum(), getTestRealmNum());
  EXPECT_EQ(accountId.getAccountNum(), getTestAccountNum());
}

TEST_F(AccountIdTest, ConstructFromString)
{
  const std::string testShardNumStr = std::to_string(getTestShardNum());
  const std::string testRealmNumStr = std::to_string(getTestRealmNum());
  const std::string testAccountNumStr = std::to_string(getTestAccountNum());

  AccountId accountId(testShardNumStr + '.' + testRealmNumStr + '.' + testAccountNumStr);
  EXPECT_EQ(accountId.getShardNum(), getTestShardNum());
  EXPECT_EQ(accountId.getRealmNum(), getTestRealmNum());
  EXPECT_EQ(accountId.getAccountNum(), getTestAccountNum());

  EXPECT_THROW(AccountId(".." + testShardNumStr + testRealmNumStr + testAccountNumStr), std::invalid_argument);
  EXPECT_THROW(AccountId('.' + testShardNumStr + '.' + testRealmNumStr + testAccountNumStr), std::invalid_argument);
  EXPECT_THROW(AccountId('.' + testShardNumStr + testRealmNumStr + '.' + testAccountNumStr), std::invalid_argument);
  EXPECT_THROW(AccountId('.' + testShardNumStr + testRealmNumStr + testAccountNumStr + '.'), std::invalid_argument);
  EXPECT_THROW(AccountId(testShardNumStr + ".." + testRealmNumStr + testAccountNumStr), std::invalid_argument);
  EXPECT_THROW(AccountId(testShardNumStr + '.' + testRealmNumStr + testAccountNumStr + '.'), std::invalid_argument);
  EXPECT_THROW(AccountId(testShardNumStr + testRealmNumStr + ".." + testAccountNumStr), std::invalid_argument);
  EXPECT_THROW(AccountId(testShardNumStr + testRealmNumStr + '.' + testAccountNumStr + '.'), std::invalid_argument);
  EXPECT_THROW(AccountId('.' + testShardNumStr + '.' + testRealmNumStr + '.' + testAccountNumStr + '.'),
               std::invalid_argument);

  EXPECT_THROW(AccountId("abc"), std::invalid_argument);
  EXPECT_THROW(AccountId("o.o.e"), std::invalid_argument);
  EXPECT_THROW(AccountId("0.0.1!"), std::invalid_argument);
}

TEST_F(AccountIdTest, SetShardRealmaccountnum)
{
  AccountId accountId;
  accountId.setShardNum(getTestShardNum());
  accountId.setRealmNum(getTestRealmNum());
  accountId.setAccountNum(getTestAccountNum());

  EXPECT_EQ(accountId.getShardNum(), getTestShardNum());
  EXPECT_EQ(accountId.getRealmNum(), getTestRealmNum());
  EXPECT_EQ(accountId.getAccountNum(), getTestAccountNum());
}

TEST_F(AccountIdTest, ProtobufaccountId)
{
  AccountId accountId;
  accountId.setShardNum(getTestShardNum());
  accountId.setRealmNum(getTestRealmNum());
  accountId.setAccountNum(getTestAccountNum());

  auto protoAccountId = std::unique_ptr<proto::AccountID>(accountId.toProtobuf());
  EXPECT_EQ(protoAccountId->shardnum(), getTestShardNum());
  EXPECT_EQ(protoAccountId->realmnum(), getTestRealmNum());
  EXPECT_EQ(protoAccountId->accountnum(), getTestAccountNum());

  const uint64_t adjustment = 3ULL;
  const uint64_t newShard = getTestShardNum() + adjustment;
  const uint64_t newRealm = getTestRealmNum() - adjustment;
  const uint64_t newContract = getTestAccountNum() * adjustment;

  protoAccountId->set_shardnum(static_cast<int64_t>(newShard));
  protoAccountId->set_realmnum(static_cast<int64_t>(newRealm));
  protoAccountId->set_accountnum(static_cast<int64_t>(newContract));

  accountId = AccountId::fromProtobuf(*protoAccountId);
  EXPECT_EQ(accountId.getShardNum(), newShard);
  EXPECT_EQ(accountId.getRealmNum(), newRealm);
  EXPECT_EQ(accountId.getAccountNum(), newContract);
}