/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "AddressBookQuery.h"

#include <gtest/gtest.h>

using namespace Hedera;

class AddressBookQueryUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const FileId& getTestFileId() const { return mTestFileId; }
  [[nodiscard]] inline unsigned int getTestLimit() const { return mTestLimit; }
  [[nodiscard]] inline unsigned int getTestMaxAttempts() const { return mTestMaxAttempts; }
  [[nodiscard]] inline const std::chrono::system_clock::duration& getTestMaxBackoff() const { return mTestMaxBackoff; }

private:
  const FileId mTestFileId = FileId(1ULL, 2ULL, 3ULL);
  const unsigned int mTestLimit = 4ULL;
  const unsigned int mTestMaxAttempts = 5ULL;
  const std::chrono::system_clock::duration mTestMaxBackoff = std::chrono::seconds(6ULL);
};

//-----
TEST_F(AddressBookQueryUnitTests, GetSetAccountId)
{
  // Given
  AddressBookQuery query;

  // When
  query.setFileId(getTestFileId());

  // Then
  EXPECT_EQ(query.getFileId(), getTestFileId());
}

//-----
TEST_F(AddressBookQueryUnitTests, GetSetLimit)
{
  // Given
  AddressBookQuery query;

  // When
  query.setLimit(getTestLimit());

  // Then
  EXPECT_EQ(query.getLimit(), getTestLimit());
}

//-----
TEST_F(AddressBookQueryUnitTests, GetSetMaxAttempts)
{
  // Given
  AddressBookQuery query;

  // When
  query.setMaxAttempts(getTestMaxAttempts());

  // Then
  EXPECT_EQ(query.getMaxAttempts(), getTestMaxAttempts());
}

//-----
TEST_F(AddressBookQueryUnitTests, GetSetMaxBackoff)
{
  // Given
  AddressBookQuery query;

  // When
  query.setMaxBackoff(getTestMaxBackoff());

  // Then
  EXPECT_EQ(query.getMaxBackoff(), getTestMaxBackoff());
}
