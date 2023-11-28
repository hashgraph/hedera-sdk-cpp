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
#include "AccountId.h"
#include "ContractCallQuery.h"
#include "ContractId.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <vector>

using namespace Hedera;

class ContractCallQueryUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const ContractId& getTestContractId() const { return mTestContractId; }
  [[nodiscard]] inline const uint64_t& getTestGas() const { return mTestGas; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestFunctionParameters() const
  {
    return mTestFunctionParameters;
  }
  [[nodiscard]] inline const AccountId& getTestSenderAccountId() const { return mTestSenderAccountId; }

private:
  const ContractId mTestContractId = ContractId(1ULL);
  const uint64_t mTestGas = 2ULL;
  const std::vector<std::byte> mTestFunctionParameters = { std::byte(0x03), std::byte(0x04), std::byte(0x05) };
  const AccountId mTestSenderAccountId = AccountId(6ULL);
};

//-----
TEST_F(ContractCallQueryUnitTests, GetSetContractId)
{
  // Given
  ContractCallQuery query;

  // When
  query.setContractId(getTestContractId());

  // Then
  EXPECT_EQ(query.getContractId(), getTestContractId());
}

//-----
TEST_F(ContractCallQueryUnitTests, GetSetGas)
{
  // Given
  ContractCallQuery query;

  // When
  query.setGas(getTestGas());

  // Then
  EXPECT_EQ(query.getGas(), getTestGas());
}

//-----
TEST_F(ContractCallQueryUnitTests, GetSetFunctionParameters)
{
  // Given
  ContractCallQuery query;

  // When
  query.setFunctionParameters(getTestFunctionParameters());

  // Then
  EXPECT_EQ(query.getFunctionParameters(), getTestFunctionParameters());
}

//-----
TEST_F(ContractCallQueryUnitTests, GetSetSenderAccountId)
{
  // Given
  ContractCallQuery query;

  // When
  query.setSenderAccountId(getTestSenderAccountId());

  // Then
  EXPECT_EQ(query.getSenderAccountId(), getTestSenderAccountId());
}
