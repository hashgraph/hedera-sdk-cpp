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
#include "ContractNonceInfo.h"
#include "impl/Utilities.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <proto/contract_types.pb.h>
#include <vector>

using namespace Hedera;

class ContractNonceInfoTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const ContractId& getTestContractId() const { return mTestContractId; }
  [[nodiscard]] inline const int64_t& getTestNonce() const { return mTestNonce; }

private:
  const ContractId mTestContractId = ContractId(1ULL);
  const int64_t mTestNonce = 2L;
};

//-----
TEST_F(ContractNonceInfoTest, FromProtobuf)
{
  // Given
  proto::ContractNonceInfo protoContractNonceInfo;
  protoContractNonceInfo.set_allocated_contract_id(getTestContractId().toProtobuf().release());
  protoContractNonceInfo.set_nonce(getTestNonce());

  // When
  const ContractNonceInfo contractNonceInfo = ContractNonceInfo::fromProtobuf(protoContractNonceInfo);

  // Then
  EXPECT_EQ(contractNonceInfo.mContractId, getTestContractId());
  EXPECT_EQ(contractNonceInfo.mNonce, getTestNonce());
}

//-----
TEST_F(ContractNonceInfoTest, ToProtobuf)
{
  // Given
  const ContractNonceInfo testContractNonceInfo = ContractNonceInfo(getTestContractId(), getTestNonce());

  // When
  const std::unique_ptr<proto::ContractNonceInfo> protoContractNonceInfo = testContractNonceInfo.toProtobuf();

  // Then
  EXPECT_EQ(protoContractNonceInfo.get()->contract_id().shardnum(),
            static_cast<int64_t>(getTestContractId().getShardNum()));
  EXPECT_EQ(protoContractNonceInfo.get()->contract_id().realmnum(),
            static_cast<int64_t>(getTestContractId().getRealmNum()));
  ASSERT_EQ(protoContractNonceInfo.get()->contract_id().contract_case(), proto::ContractID::ContractCase::kContractNum);
  EXPECT_EQ(protoContractNonceInfo.get()->contract_id().contractnum(),
            static_cast<int64_t>(getTestContractId().getContractNum().value()));
  EXPECT_EQ(protoContractNonceInfo.get()->nonce(), getTestNonce());
}

//-----
TEST_F(ContractNonceInfoTest, ToBytes)
{
  // Given
  const ContractNonceInfo testContractNonceInfo = ContractNonceInfo(getTestContractId(), getTestNonce());

  // When
  const std::vector<std::byte> bytes = testContractNonceInfo.toBytes();

  // Then
  EXPECT_EQ(bytes, internal::Utilities::stringToByteVector(testContractNonceInfo.toProtobuf()->SerializeAsString()));
}

//-----
TEST_F(ContractNonceInfoTest, FromBytes)
{
  // Given
  proto::ContractNonceInfo protoContractNonceInfo;
  protoContractNonceInfo.set_allocated_contract_id(getTestContractId().toProtobuf().release());
  protoContractNonceInfo.set_nonce(getTestNonce());

  // When
  ContractNonceInfo contractNonceInfo =
    ContractNonceInfo::fromBytes(internal::Utilities::stringToByteVector(protoContractNonceInfo.SerializeAsString()));

  // Then
  EXPECT_EQ(contractNonceInfo.mContractId.getShardNum(), getTestContractId().getShardNum());
  EXPECT_EQ(contractNonceInfo.mContractId.getRealmNum(), getTestContractId().getRealmNum());
  EXPECT_EQ(contractNonceInfo.mContractId.getContractNum(), getTestContractId().getContractNum());
  EXPECT_EQ(contractNonceInfo.mNonce, getTestNonce());
}
