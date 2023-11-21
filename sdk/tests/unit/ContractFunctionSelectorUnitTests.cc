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
#include "ContractFunctionSelector.h"
#include "exceptions/IllegalStateException.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <vector>

using namespace Hedera;

class ContractFunctionSelectorUnitTests : public ::testing::Test
{
};

//-----
TEST_F(ContractFunctionSelectorUnitTests, AddAllTypes)
{
  // Given
  ContractFunctionSelector selector = ContractFunctionSelector()
                                        .addAddress()
                                        .addAddressArray()
                                        .addBool()
                                        .addBytes()
                                        .addBytes32()
                                        .addBytes32Array()
                                        .addBytesArray()
                                        .addFunction()
                                        .addInt8()
                                        .addInt8Array()
                                        .addInt32()
                                        .addInt32Array()
                                        .addInt64()
                                        .addInt64Array()
                                        .addInt256()
                                        .addInt256Array()
                                        .addUint8()
                                        .addUint8Array()
                                        .addUint32()
                                        .addUint32Array()
                                        .addUint64()
                                        .addUint64Array()
                                        .addUint256()
                                        .addUint256Array()
                                        .addString()
                                        .addStringArray();

  // When
  const std::vector<std::byte> bytes = selector.finish("testFunction");

  // Then
  const std::vector<std::byte> expectedBytes = { std::byte(0x44), std::byte(0x38), std::byte(0xE4), std::byte(0xCE) };
  EXPECT_EQ(bytes, expectedBytes);
}
