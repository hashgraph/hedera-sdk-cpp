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
  [[nodiscard]] inline const uint64_t& getTestNum() const { return mTestNum; }

private:
  const uint64_t mTestNum = 10;
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
  ContractId contractId(getTestNum());
  EXPECT_EQ(contractId.getShardNum(), 0ULL);
  EXPECT_EQ(contractId.getRealmNum(), 0ULL);
  EXPECT_EQ(contractId.getContractNum(), getTestNum());
}

TEST_F(ContractIdTest, ConstructWithShardRealmContractNum)
{
  ContractId contractId(getTestNum(), getTestNum(), getTestNum());
  EXPECT_EQ(contractId.getShardNum(), getTestNum());
  EXPECT_EQ(contractId.getRealmNum(), getTestNum());
  EXPECT_EQ(contractId.getContractNum(), getTestNum());
}

TEST_F(ContractIdTest, SetShardRealmContractNum)
{
  ContractId contractId;
  contractId.setShardNum(getTestNum());
  contractId.setRealmNum(getTestNum());
  contractId.setContractNum(getTestNum());

  EXPECT_EQ(contractId.getShardNum(), getTestNum());
  EXPECT_EQ(contractId.getRealmNum(), getTestNum());
  EXPECT_EQ(contractId.getContractNum(), getTestNum());
}

TEST_F(ContractIdTest, ProtobufContractId)
{
  ContractId contractId;
  contractId.setShardNum(getTestNum());
  contractId.setRealmNum(getTestNum());
  contractId.setContractNum(getTestNum());

  auto protoContractId = std::unique_ptr<proto::ContractID>(contractId.toProtobuf());
  EXPECT_EQ(protoContractId->shardnum(), getTestNum());
  EXPECT_EQ(protoContractId->realmnum(), getTestNum());
  EXPECT_EQ(protoContractId->contractnum(), getTestNum());

  const uint64_t adjustment = 3ULL;
  const uint64_t newShard = getTestNum() + adjustment;
  const uint64_t newRealm = getTestNum() - adjustment;
  const uint64_t newContract = getTestNum() * adjustment;

  protoContractId->set_shardnum(static_cast<int64_t>(newShard));
  protoContractId->set_realmnum(static_cast<int64_t>(newRealm));
  protoContractId->set_contractnum(static_cast<int64_t>(newContract));

  contractId = ContractId::fromProtobuf(*protoContractId);
  EXPECT_EQ(contractId.getShardNum(), newShard);
  EXPECT_EQ(contractId.getRealmNum(), newRealm);
  EXPECT_EQ(contractId.getContractNum(), newContract);
}