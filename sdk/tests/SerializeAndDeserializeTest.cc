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
#include "PrivateKey.h"
#include "AccountId.h"
#include "ContractId.h"

#include <gtest/gtest.h>
#include <iostream>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class SerializeAndDeserializeTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const uint64_t getTestShardNum() const { return mShardNum; }
  [[nodiscard]] inline const uint64_t getTestRealmNum() const { return mRealmNum; }
  [[nodiscard]] inline const uint64_t getTestContractNum() const { return mContractNum; }
  [[nodiscard]] inline const AccountId& getAccountId() const { return mAccountId; }
  [[nodiscard]] inline const AccountId& getNodeId() const { return mNodeId; }
  [[nodiscard]] inline const AccountId& getOperatorId() const { return mOperatorId; }
  [[nodiscard]] inline const AccountId& getSenderId() const { return mSenderId; }
  [[nodiscard]] inline const AccountId& getRecipientId() const { return mRecipientId; }

private:
  const uint64_t mShardNum = 1;
  const uint64_t mRealmNum = 2;
  const uint64_t mContractNum = 3;
  const AccountId mAccountId = AccountId(10ULL);
  const AccountId mNodeId = AccountId("0.0.7");
  const AccountId mOperatorId = AccountId("0.0.666");
  const AccountId mSenderId = AccountId("0.0.123456");
  const AccountId mRecipientId = AccountId("0.0.654321");
  const std::chrono::system_clock::time_point mValidStart = std::chrono::system_clock::time_point();
};

TEST_F(SerializeAndDeserializeTest, SerializeAccountIdTest)
{
  const std::string accountIdStr = "111.222.333";
  AccountId accountId(accountIdStr);

  auto protoAccountId = std::unique_ptr<proto::AccountID>(accountId.toProtobuf());

  EXPECT_EQ(protoAccountId->shardnum(), accountId.getShardNum());
  EXPECT_EQ(protoAccountId->realmnum(), accountId.getRealmNum());
  EXPECT_EQ(protoAccountId->accountnum(), accountId.getAccountNum());
}

TEST_F(SerializeAndDeserializeTest, DeserializeAccountIdTest)
{
  proto::AccountID protoAccountId = proto::AccountID();

  protoAccountId.set_shardnum(123);
  protoAccountId.set_realmnum(456);
  protoAccountId.set_accountnum(789);

  AccountId accountId = AccountId::fromProtobuf(protoAccountId);
  
  EXPECT_EQ(accountId.toString(), "123.456.789");
}

TEST_F(SerializeAndDeserializeTest, SerializeContractIdTest)
{
  // Given
  uint64_t testShardNum = getTestShardNum();
  uint64_t testRealmNum = getTestRealmNum();
  uint64_t testContractNum = getTestContractNum();

  ContractId testContractId = ContractId(testShardNum, testRealmNum, testContractNum);

  // When
  auto protoContractId = std::unique_ptr<proto::ContractID>(testContractId.toProtobuf());

  // Then
  EXPECT_EQ(protoContractId->shardnum(), testShardNum);
  EXPECT_EQ(protoContractId->realmnum(), testRealmNum);
  EXPECT_EQ(protoContractId->contractnum(), testContractNum);
}

TEST_F(SerializeAndDeserializeTest, DeserializeContractIdTest)
{
  // Given
  uint64_t testShardNum = getTestShardNum();
  uint64_t testRealmNum = getTestRealmNum();
  uint64_t testContractNum = getTestContractNum();

  proto::ContractID testProtoContractId = proto::ContractID();

  testProtoContractId.set_shardnum(testShardNum);
  testProtoContractId.set_realmnum(testRealmNum);
  testProtoContractId.set_contractnum(testContractNum);

  // When
  ContractId contractId = ContractId::fromProtobuf(testProtoContractId);
  
  // Then
  EXPECT_EQ(contractId.getShardNum(), testShardNum);
  EXPECT_EQ(contractId.getRealmNum(), testRealmNum);
  EXPECT_EQ(contractId.getContractNum(), testContractNum);
}