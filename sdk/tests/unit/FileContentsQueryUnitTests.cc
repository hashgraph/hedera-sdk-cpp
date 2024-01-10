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
#include "FileContentsQuery.h"
#include "FileId.h"

#include <gtest/gtest.h>

using namespace Hedera;

class FileContentsQueryUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const FileId& getTestFileId() const { return mTestFileId; }

private:
  const FileId mTestFileId = FileId(1ULL);
};

//-----
TEST_F(FileContentsQueryUnitTests, GetSetFileId)
{
  // Given
  FileContentsQuery query;

  // When
  query.setFileId(getTestFileId());

  // Then
  EXPECT_EQ(query.getFileId(), getTestFileId());
}
