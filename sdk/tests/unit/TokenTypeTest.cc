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
#include "TokenType.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class TokenTypeTest : public ::testing::Test
{
};

//-----
TEST_F(TokenTypeTest, ProtobufTokenTypeToTokenType)
{
  // Given / When / Then
  ASSERT_TRUE(gProtobufTokenTypeToTokenType.find(proto::TokenType::FUNGIBLE_COMMON) !=
              gProtobufTokenTypeToTokenType.end());
  ASSERT_TRUE(gProtobufTokenTypeToTokenType.find(proto::TokenType::NON_FUNGIBLE_UNIQUE) !=
              gProtobufTokenTypeToTokenType.end());
  EXPECT_EQ(TokenType::FUNGIBLE_COMMON, gProtobufTokenTypeToTokenType.at(proto::TokenType::FUNGIBLE_COMMON));
  EXPECT_EQ(TokenType::NON_FUNGIBLE_UNIQUE, gProtobufTokenTypeToTokenType.at(proto::TokenType::NON_FUNGIBLE_UNIQUE));
}

//-----
TEST_F(TokenTypeTest, TokenTypeToString)
{
  // Given / When / Then
  ASSERT_TRUE(gTokenTypeToString.find(TokenType::FUNGIBLE_COMMON) != gTokenTypeToString.end());
  ASSERT_TRUE(gTokenTypeToString.find(TokenType::NON_FUNGIBLE_UNIQUE) != gTokenTypeToString.end());
  EXPECT_STRCASEEQ(gTokenTypeToString.at(TokenType::FUNGIBLE_COMMON), "FUNGIBLE_COMMON");
  EXPECT_STRCASEEQ(gTokenTypeToString.at(TokenType::NON_FUNGIBLE_UNIQUE), "NON_FUNGIBLE_UNIQUE");
}
