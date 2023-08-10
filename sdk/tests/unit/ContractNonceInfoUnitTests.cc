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
  const int64_t mTestNonce = 10L;
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
  // ContractLogInfo contractLogInfo;
  // contractLogInfo.mContractId = getTestContractId();
  // contractLogInfo.mBloom = getTestBloom();
  // contractLogInfo.mTopics = getTestTopics();
  // contractLogInfo.mData = getTestData();

  // // When
  // const std::unique_ptr<proto::ContractLoginfo> protoContractLogInfo = contractLogInfo.toProtobuf();

  // // Then
  // EXPECT_EQ(ContractId::fromProtobuf(protoContractLogInfo->contractid()), getTestContractId());
  // EXPECT_EQ(protoContractLogInfo->bloom(), internal::Utilities::byteVectorToString(getTestBloom()));

  // ASSERT_EQ(protoContractLogInfo->topic_size(), getTestTopics().size());
  // for (int i = 0; i < protoContractLogInfo->topic_size(); ++i)
  // {
  //   EXPECT_EQ(protoContractLogInfo->topic(i), internal::Utilities::byteVectorToString(getTestTopics().at(i)));
  // }

  // EXPECT_EQ(protoContractLogInfo->data(), internal::Utilities::byteVectorToString(getTestData()));
}
