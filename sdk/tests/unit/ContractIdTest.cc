/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <limits>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class ContractIdTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const uint64_t& getTestShardNum() const { return mShardNum; }
  [[nodiscard]] inline const uint64_t& getTestRealmNum() const { return mRealmNum; }
  [[nodiscard]] inline const uint64_t& getTestContractNum() const { return mContractNum; }
  [[nodiscard]] inline const uint64_t& getTestNumTooBig() const { return mNumTooBig; }
  [[nodiscard]] inline const EvmAddress& getTestEvmAddress() const { return mEvmAddress; }

private:
  const uint64_t mShardNum = 1ULL;
  const uint64_t mRealmNum = 2ULL;
  const uint64_t mContractNum = 3ULL;
  const uint64_t mNumTooBig = static_cast<uint64_t>(std::numeric_limits<int64_t>::max()) + 1ULL;
  const EvmAddress mEvmAddress = EvmAddress::fromString("0x0123456789abcdef0123456789abcdef01234567");
};

//-----
TEST_F(ContractIdTest, DefaultConstructContractId)
{
  // Given / When
  const ContractId contractId;

  // Then
  EXPECT_EQ(contractId.getShardNum(), 0ULL);
  EXPECT_EQ(contractId.getRealmNum(), 0ULL);
  EXPECT_FALSE(contractId.getContractNum().has_value());
  EXPECT_FALSE(contractId.getEvmAddress().has_value());
}

//-----
TEST_F(ContractIdTest, ConstructWithContractNum)
{
  // Given / When
  const ContractId contractId(getTestContractNum());

  // Then
  EXPECT_EQ(contractId.getShardNum(), 0ULL);
  EXPECT_EQ(contractId.getRealmNum(), 0ULL);
  ASSERT_TRUE(contractId.getContractNum().has_value());
  EXPECT_EQ(contractId.getContractNum(), getTestContractNum());
  EXPECT_FALSE(contractId.getEvmAddress().has_value());
}

//-----
TEST_F(ContractIdTest, ConstructWithContractNumTooBig)
{
  // Given / When / Then
  EXPECT_THROW(const ContractId contractId(getTestNumTooBig()), std::invalid_argument);
}

//-----
TEST_F(ContractIdTest, ConstructWithEvmAddress)
{
  // Given / When
  const ContractId contractId(getTestEvmAddress());

  // Then
  EXPECT_EQ(contractId.getShardNum(), 0ULL);
  EXPECT_EQ(contractId.getRealmNum(), 0ULL);
  EXPECT_FALSE(contractId.getContractNum().has_value());
  ASSERT_TRUE(contractId.getEvmAddress().has_value());
  EXPECT_EQ(contractId.getEvmAddress()->toBytes(), getTestEvmAddress().toBytes());
}

//-----
TEST_F(ContractIdTest, ConstructWithShardRealmContractNum)
{
  // Given / When
  const ContractId contractId(getTestShardNum(), getTestRealmNum(), getTestContractNum());

  // Then
  EXPECT_EQ(contractId.getShardNum(), getTestShardNum());
  EXPECT_EQ(contractId.getRealmNum(), getTestRealmNum());
  ASSERT_TRUE(contractId.getContractNum().has_value());
  EXPECT_EQ(contractId.getContractNum(), getTestContractNum());
  EXPECT_FALSE(contractId.getEvmAddress().has_value());
}

//-----
TEST_F(ContractIdTest, ConstructWithShardRealmContractNumTooBig)
{
  // Given / When / Then
  EXPECT_THROW(const ContractId contractId(getTestNumTooBig(), getTestRealmNum(), getTestContractNum()),
               std::invalid_argument);
  EXPECT_THROW(const ContractId contractId(getTestShardNum(), getTestNumTooBig(), getTestContractNum()),
               std::invalid_argument);
  EXPECT_THROW(const ContractId contractId(getTestShardNum(), getTestRealmNum(), getTestNumTooBig()),
               std::invalid_argument);
}

//-----
TEST_F(ContractIdTest, ConstructWithShardRealmEvmAddress)
{
  // Given / When
  const ContractId contractId(getTestShardNum(), getTestRealmNum(), getTestEvmAddress());

  // Then
  EXPECT_EQ(contractId.getShardNum(), getTestShardNum());
  EXPECT_EQ(contractId.getRealmNum(), getTestRealmNum());
  EXPECT_FALSE(contractId.getContractNum().has_value());
  ASSERT_TRUE(contractId.getEvmAddress().has_value());
  EXPECT_EQ(contractId.getEvmAddress()->toBytes(), getTestEvmAddress().toBytes());
}

//-----
TEST_F(ContractIdTest, ConstructWithShardRealmEvmAddressTooBig)
{
  // Given / When / Then
  EXPECT_THROW(const ContractId contractId(getTestNumTooBig(), getTestRealmNum(), getTestEvmAddress()),
               std::invalid_argument);
  EXPECT_THROW(const ContractId contractId(getTestShardNum(), getTestNumTooBig(), getTestEvmAddress()),
               std::invalid_argument);
}

//-----
TEST_F(ContractIdTest, CompareContractIds)
{
  // Given / When / Then
  EXPECT_TRUE(ContractId() == ContractId());
  EXPECT_TRUE(ContractId(getTestContractNum()) == ContractId(getTestContractNum()));
  EXPECT_TRUE(ContractId(getTestEvmAddress()) == ContractId(getTestEvmAddress()));
  EXPECT_TRUE(ContractId(getTestShardNum(), getTestRealmNum(), getTestContractNum()) ==
              ContractId(getTestShardNum(), getTestRealmNum(), getTestContractNum()));
  EXPECT_FALSE(ContractId(getTestContractNum()) == ContractId(getTestContractNum() - 1ULL));
  EXPECT_FALSE(ContractId(getTestShardNum(), getTestRealmNum(), getTestContractNum()) ==
               ContractId(getTestShardNum() - 1ULL, getTestRealmNum(), getTestContractNum()));
  EXPECT_FALSE(ContractId(getTestShardNum(), getTestRealmNum(), getTestContractNum()) ==
               ContractId(getTestShardNum(), getTestRealmNum() - 1ULL, getTestContractNum()));
}

//-----
TEST_F(ContractIdTest, FromProtobuf)
{
  // Given
  proto::ContractID protoContractIdContractNum;
  protoContractIdContractNum.set_shardnum(static_cast<int64_t>(getTestShardNum()));
  protoContractIdContractNum.set_realmnum(static_cast<int64_t>(getTestRealmNum()));
  protoContractIdContractNum.set_contractnum(static_cast<int64_t>(getTestContractNum()));

  proto::ContractID protoContractIdEvmAddress;
  protoContractIdEvmAddress.set_shardnum(static_cast<int64_t>(getTestShardNum()));
  protoContractIdEvmAddress.set_realmnum(static_cast<int64_t>(getTestRealmNum()));
  protoContractIdEvmAddress.set_evm_address(internal::Utilities::byteVectorToString(getTestEvmAddress().toBytes()));

  // When
  const ContractId contractIdContractNum = ContractId::fromProtobuf(protoContractIdContractNum);
  const ContractId contractIdEvmAddress = ContractId::fromProtobuf(protoContractIdEvmAddress);

  // Then
  EXPECT_EQ(contractIdContractNum.getShardNum(), getTestShardNum());
  EXPECT_EQ(contractIdContractNum.getRealmNum(), getTestRealmNum());
  ASSERT_TRUE(contractIdContractNum.getContractNum().has_value());
  EXPECT_EQ(contractIdContractNum.getContractNum(), getTestContractNum());
  EXPECT_FALSE(contractIdContractNum.getEvmAddress().has_value());

  EXPECT_EQ(contractIdEvmAddress.getShardNum(), getTestShardNum());
  EXPECT_EQ(contractIdEvmAddress.getRealmNum(), getTestRealmNum());
  EXPECT_FALSE(contractIdEvmAddress.getContractNum().has_value());
  ASSERT_TRUE(contractIdEvmAddress.getEvmAddress().has_value());
  EXPECT_EQ(contractIdEvmAddress.getEvmAddress()->toBytes(), getTestEvmAddress().toBytes());
}

//-----
TEST_F(ContractIdTest, Clone)
{
  // Given
  const ContractId contractIdContractNum(getTestShardNum(), getTestRealmNum(), getTestContractNum());
  const ContractId contractIdEvmAddress(getTestShardNum(), getTestRealmNum(), getTestEvmAddress());

  // When
  const std::unique_ptr<Key> clonedContractIdContractNum = contractIdContractNum.clone();
  const std::unique_ptr<Key> clonedContractIdEvmAddress = contractIdEvmAddress.clone();

  // Then
  EXPECT_EQ(clonedContractIdContractNum->toBytes(), contractIdContractNum.toBytes());
  EXPECT_EQ(clonedContractIdEvmAddress->toBytes(), contractIdEvmAddress.toBytes());
}

//-----
TEST_F(ContractIdTest, ToProtobufKey)
{
  // Given
  const ContractId contractIdContractNum(getTestShardNum(), getTestRealmNum(), getTestContractNum());
  const ContractId contractIdEvmAddress(getTestShardNum(), getTestRealmNum(), getTestEvmAddress());

  // When
  const std::unique_ptr<proto::Key> protoContractIdContractNum = contractIdContractNum.toProtobufKey();
  const std::unique_ptr<proto::Key> protoContractIdEvmAddress = contractIdEvmAddress.toProtobufKey();

  // Then
  ASSERT_TRUE(protoContractIdContractNum->has_contractid());
  ASSERT_TRUE(protoContractIdEvmAddress->has_contractid());

  EXPECT_EQ(protoContractIdContractNum->contractid().shardnum(), static_cast<int64_t>(getTestShardNum()));
  EXPECT_EQ(protoContractIdContractNum->contractid().realmnum(), static_cast<int64_t>(getTestRealmNum()));
  ASSERT_EQ(protoContractIdContractNum->contractid().contract_case(), proto::ContractID::ContractCase::kContractNum);
  EXPECT_EQ(protoContractIdContractNum->contractid().contractnum(), static_cast<int64_t>(getTestContractNum()));

  EXPECT_EQ(protoContractIdEvmAddress->contractid().shardnum(), static_cast<int64_t>(getTestShardNum()));
  EXPECT_EQ(protoContractIdEvmAddress->contractid().realmnum(), static_cast<int64_t>(getTestRealmNum()));
  ASSERT_EQ(protoContractIdEvmAddress->contractid().contract_case(), proto::ContractID::ContractCase::kEvmAddress);
  EXPECT_EQ(protoContractIdEvmAddress->contractid().evm_address(),
            internal::Utilities::byteVectorToString(getTestEvmAddress().toBytes()));
}

//-----
TEST_F(ContractIdTest, ToProtobuf)
{
  // Given
  const ContractId contractIdContractNum(getTestShardNum(), getTestRealmNum(), getTestContractNum());
  const ContractId contractIdEvmAddress(getTestShardNum(), getTestRealmNum(), getTestEvmAddress());

  // When
  const std::unique_ptr<proto::ContractID> protoContractIdContractNum = contractIdContractNum.toProtobuf();
  const std::unique_ptr<proto::ContractID> protoContractIdEvmAddress = contractIdEvmAddress.toProtobuf();

  // Then
  EXPECT_EQ(protoContractIdContractNum->shardnum(), static_cast<int64_t>(getTestShardNum()));
  EXPECT_EQ(protoContractIdContractNum->realmnum(), static_cast<int64_t>(getTestRealmNum()));
  ASSERT_EQ(protoContractIdContractNum->contract_case(), proto::ContractID::ContractCase::kContractNum);
  EXPECT_EQ(protoContractIdContractNum->contractnum(), static_cast<int64_t>(getTestContractNum()));

  EXPECT_EQ(protoContractIdEvmAddress->shardnum(), static_cast<int64_t>(getTestShardNum()));
  EXPECT_EQ(protoContractIdEvmAddress->realmnum(), static_cast<int64_t>(getTestRealmNum()));
  ASSERT_EQ(protoContractIdEvmAddress->contract_case(), proto::ContractID::ContractCase::kEvmAddress);
  EXPECT_EQ(protoContractIdEvmAddress->evm_address(),
            internal::Utilities::byteVectorToString(getTestEvmAddress().toBytes()));
}

//-----
TEST_F(ContractIdTest, ToString)
{
  // Given
  const ContractId contractIdDefault;
  const ContractId contractIdContractNum(getTestContractNum());
  const ContractId contractIdEvmAddress(getTestEvmAddress());
  const ContractId contractIdShardRealmContractNum(getTestShardNum(), getTestRealmNum(), getTestContractNum());
  const ContractId contractIdShardRealmEvmAddress(getTestShardNum(), getTestRealmNum(), getTestEvmAddress());

  // When
  const std::string strContractIdDefault = contractIdDefault.toString();
  const std::string strContractIdContractNum = contractIdContractNum.toString();
  const std::string strContractIdEvmAddress = contractIdEvmAddress.toString();
  const std::string strContractIdShardRealmContractNum = contractIdShardRealmContractNum.toString();
  const std::string strContractIdShardRealmEvmAddress = contractIdShardRealmEvmAddress.toString();

  // Then
  EXPECT_EQ(strContractIdDefault, "0.0.0");
  EXPECT_EQ(strContractIdContractNum, "0.0." + std::to_string(getTestContractNum()));
  EXPECT_EQ(strContractIdEvmAddress, "0.0." + getTestEvmAddress().toString());
  EXPECT_EQ(strContractIdShardRealmContractNum,
            std::to_string(getTestShardNum()) + '.' + std::to_string(getTestRealmNum()) + '.' +
              std::to_string(getTestContractNum()));
  EXPECT_EQ(strContractIdShardRealmEvmAddress,
            std::to_string(getTestShardNum()) + '.' + std::to_string(getTestRealmNum()) + '.' +
              getTestEvmAddress().toString());
}

//-----
TEST_F(ContractIdTest, GetSetShardNum)
{
  // Given
  ContractId contractId;

  // When
  EXPECT_THROW(contractId.setShardNum(getTestNumTooBig()), std::invalid_argument);
  contractId.setShardNum(getTestShardNum());

  // Then
  EXPECT_EQ(contractId.getShardNum(), getTestShardNum());
}

//-----
TEST_F(ContractIdTest, GetSetRealmNum)
{
  // Given
  ContractId contractId;

  // When
  EXPECT_THROW(contractId.setRealmNum(getTestNumTooBig()), std::invalid_argument);
  contractId.setRealmNum(getTestRealmNum());

  // Then
  EXPECT_EQ(contractId.getRealmNum(), getTestRealmNum());
}

//-----
TEST_F(ContractIdTest, GetSetContractNum)
{
  // Given
  ContractId contractId;

  // When
  EXPECT_THROW(contractId.setContractNum(getTestNumTooBig()), std::invalid_argument);
  contractId.setContractNum(getTestContractNum());

  // Then
  ASSERT_TRUE(contractId.getContractNum().has_value());
  EXPECT_EQ(contractId.getContractNum(), getTestContractNum());
}

//-----
TEST_F(ContractIdTest, GetSetEvmAddress)
{
  // Given
  ContractId contractId;

  // When
  contractId.setEvmAddress(getTestEvmAddress());

  // Then
  ASSERT_TRUE(contractId.getEvmAddress().has_value());
  EXPECT_EQ(contractId.getEvmAddress()->toBytes(), getTestEvmAddress().toBytes());
}

//-----
TEST_F(ContractIdTest, ResetMutuallyExclusiveIds)
{
  // Given
  ContractId contractIdContractNum(getTestContractNum());
  ContractId contractIdEvmAddress(getTestEvmAddress());

  // When
  contractIdContractNum.setEvmAddress(getTestEvmAddress());
  contractIdEvmAddress.setContractNum(getTestContractNum());

  // Then
  EXPECT_FALSE(contractIdContractNum.getContractNum().has_value());
  EXPECT_FALSE(contractIdEvmAddress.getEvmAddress().has_value());
}
