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
#include "TokenNftInfoQuery.h"

#include <gtest/gtest.h>

using namespace Hiero;

class TokenNftInfoQueryUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const NftId& getTestNftId() const { return mTestNftId; }

private:
  const NftId mTestNftId = NftId(TokenId(1ULL, 2ULL, 3ULL), 4ULL);
};

//-----
TEST_F(TokenNftInfoQueryUnitTests, GetSetNftId)
{
  // Given
  TokenNftInfoQuery query;

  // When
  query.setNftId(getTestNftId());

  // Then
  EXPECT_EQ(query.getNftId(), getTestNftId());
}
