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
#include "ContractId.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class ContractIdTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const uint64_t& getTestShardNum() const { return mShardNum; }
  [[nodiscard]] inline const uint64_t& getTestRealmNum() const { return mRealmNum; }
  [[nodiscard]] inline const uint64_t& getTestContractNum() const { return mContractNum; }

private:
  const uint64_t mShardNum = 8ULL;
  const uint64_t mRealmNum = 9ULL;
  const uint64_t mContractNum = 10ULL;
};

TEST_F(ContractIdTest, DefaultConstructContractId)
{
  ContractId contractId;
  EXPECT_EQ(contractId.getShardNum(), 0ULL);
  EXPECT_EQ(contractId.getRealmNum(), 0ULL);
  EXPECT_EQ(contractId.getContractNum(), 0ULL);
}

TEST_F(ContractIdTest, ConstructWithContractNum)
{
  ContractId contractId(getTestContractNum());
  EXPECT_EQ(contractId.getShardNum(), 0ULL);
  EXPECT_EQ(contractId.getRealmNum(), 0ULL);
  EXPECT_EQ(contractId.getContractNum(), getTestContractNum());
}

TEST_F(ContractIdTest, ConstructWithShardRealmContractNum)
{
  ContractId contractId(getTestShardNum(), getTestRealmNum(), getTestContractNum());
  EXPECT_EQ(contractId.getShardNum(), getTestShardNum());
  EXPECT_EQ(contractId.getRealmNum(), getTestRealmNum());
  EXPECT_EQ(contractId.getContractNum(), getTestContractNum());
}

TEST_F(ContractIdTest, SetShardRealmContractNum)
{
  ContractId contractId;
  contractId.setShardNum(getTestShardNum());
  contractId.setRealmNum(getTestRealmNum());
  contractId.setContractNum(getTestContractNum());

  EXPECT_EQ(contractId.getShardNum(), getTestShardNum());
  EXPECT_EQ(contractId.getRealmNum(), getTestRealmNum());
  EXPECT_EQ(contractId.getContractNum(), getTestContractNum());
}

TEST_F(ContractIdTest, ProtobufContractId)
{
  ContractId contractId;
  contractId.setShardNum(getTestShardNum());
  contractId.setRealmNum(getTestRealmNum());
  contractId.setContractNum(getTestContractNum());

  auto protoContractId = std::unique_ptr<proto::ContractID>(contractId.toProtobuf());
  EXPECT_EQ(protoContractId->shardnum(), getTestShardNum());
  EXPECT_EQ(protoContractId->realmnum(), getTestRealmNum());
  EXPECT_EQ(protoContractId->contractnum(), getTestContractNum());

  const uint64_t adjustment = 3ULL;
  const uint64_t newShard = getTestShardNum() + adjustment;
  const uint64_t newRealm = getTestRealmNum() - adjustment;
  const uint64_t newContract = getTestContractNum() * adjustment;

  protoContractId->set_shardnum(static_cast<int64_t>(newShard));
  protoContractId->set_realmnum(static_cast<int64_t>(newRealm));
  protoContractId->set_contractnum(static_cast<int64_t>(newContract));

  contractId = ContractId::fromProtobuf(*protoContractId);
  EXPECT_EQ(contractId.getShardNum(), newShard);
  EXPECT_EQ(contractId.getRealmNum(), newRealm);
  EXPECT_EQ(contractId.getContractNum(), newContract);
}