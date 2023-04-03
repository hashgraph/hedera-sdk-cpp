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
#include "ContractLogInfo.h"
#include "ContractId.h"
#include "impl/Utilities.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <proto/contract_call_local.pb.h>
#include <vector>

using namespace Hedera;

class ContractLogInfoTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const ContractId& getTestContractId() const { return mTestContractId; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestBloom() const { return mTestBloom; }
  [[nodiscard]] inline const std::vector<std::vector<std::byte>>& getTestTopics() const { return mTestTopics; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestData() const { return mTestBloom; }

private:
  const ContractId mTestContractId = ContractId(1ULL);
  const std::vector<std::byte> mTestBloom = { std::byte(0x02), std::byte(0x03), std::byte(0x04) };
  const std::vector<std::vector<std::byte>> mTestTopics = {
    {std::byte(0x05),  std::byte(0x06), std::byte(0x07)},
    { std::byte(0x08), std::byte(0x09), std::byte(0x0A)},
    { std::byte(0x0B), std::byte(0x0C), std::byte(0x0D)}
  };
  const std::vector<std::byte> mTestData = { std::byte(0x0E), std::byte(0x0F) };
};

//-----
TEST_F(ContractLogInfoTest, FromProtobuf)
{
  // Given
  proto::ContractLoginfo protoContractLogInfo;
  protoContractLogInfo.set_allocated_contractid(getTestContractId().toProtobuf().release());
  protoContractLogInfo.set_bloom(internal::Utilities::byteVectorToString(getTestBloom()));

  for (const auto& topic : getTestTopics())
  {
    protoContractLogInfo.add_topic(internal::Utilities::byteVectorToString(topic));
  }

  protoContractLogInfo.set_data(internal::Utilities::byteVectorToString(getTestData()));

  // When
  const ContractLogInfo contractLogInfo = ContractLogInfo::fromProtobuf(protoContractLogInfo);

  // Then
  EXPECT_EQ(contractLogInfo.getContractId(), getTestContractId());

  ASSERT_EQ(contractLogInfo.getBloom().size(), getTestBloom().size());
  for (int i = 0; i < contractLogInfo.getBloom().size(); ++i)
  {
    EXPECT_EQ(contractLogInfo.getBloom().at(i), getTestBloom().at(i));
  }

  ASSERT_EQ(contractLogInfo.getTopics().size(), getTestTopics().size());
  for (int i = 0; i < contractLogInfo.getTopics().size(); ++i)
  {
    ASSERT_EQ(contractLogInfo.getTopics().at(i).size(), getTestTopics().at(i).size());
    for (int j = 0; j < contractLogInfo.getTopics().at(i).size(); ++j)
    {
      EXPECT_EQ(contractLogInfo.getTopics().at(i).at(j), getTestTopics().at(i).at(j));
    }
  }

  ASSERT_EQ(contractLogInfo.getData().size(), getTestData().size());
  for (int i = 0; i < contractLogInfo.getData().size(); ++i)
  {
    EXPECT_EQ(contractLogInfo.getData().at(i), getTestData().at(i));
  }
}
