/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "TokenSupplyType.h"

#include <basic_types.pb.h>
#include <gtest/gtest.h>

using namespace Hiero;

class TokenSupplyTypeUnitTests : public ::testing::Test
{
};

//-----
TEST_F(TokenSupplyTypeUnitTests, ProtobufTokenSupplyTypeToTokenSupplyType)
{
  // Given / When / Then
  ASSERT_TRUE(gProtobufTokenSupplyTypeToTokenSupplyType.find(proto::TokenSupplyType::INFINITE) !=
              gProtobufTokenSupplyTypeToTokenSupplyType.end());
  ASSERT_TRUE(gProtobufTokenSupplyTypeToTokenSupplyType.find(proto::TokenSupplyType::FINITE) !=
              gProtobufTokenSupplyTypeToTokenSupplyType.end());
  EXPECT_EQ(TokenSupplyType::INFINITE, gProtobufTokenSupplyTypeToTokenSupplyType.at(proto::TokenSupplyType::INFINITE));
  EXPECT_EQ(TokenSupplyType::FINITE, gProtobufTokenSupplyTypeToTokenSupplyType.at(proto::TokenSupplyType::FINITE));
}

//-----
TEST_F(TokenSupplyTypeUnitTests, TokenSupplyTypeToString)
{
  // Given / When / Then
  ASSERT_TRUE(gTokenSupplyTypeToString.find(TokenSupplyType::INFINITE) != gTokenSupplyTypeToString.end());
  ASSERT_TRUE(gTokenSupplyTypeToString.find(TokenSupplyType::FINITE) != gTokenSupplyTypeToString.end());
  EXPECT_STRCASEEQ(gTokenSupplyTypeToString.at(TokenSupplyType::INFINITE), "INFINITE");
  EXPECT_STRCASEEQ(gTokenSupplyTypeToString.at(TokenSupplyType::FINITE), "FINITE");
}
