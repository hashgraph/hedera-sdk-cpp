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
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "FeeSchedule.h"
#include "FileContentsQuery.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileId.h"
#include "PrivateKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"

#include <gtest/gtest.h>

using namespace Hedera;

class FeeSchedulesIntegrationTests : public BaseIntegrationTest
{
protected:
  [[nodiscard]] inline const std::vector<std::byte>& getTestFileContents() const { return mTestFileContents; }

private:
  const std::vector<std::byte> mTestFileContents = { std::byte(0x01), std::byte(0x02), std::byte(0x03) };
};

//-----
TEST_F(FeeSchedulesIntegrationTests, FeeSchedulesFetchedAndParsedFromFile)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ operatorKey->getPublicKey() })
                             .setContents(getTestFileContents())
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .mFileId.value());

  // When
  FileContents fileContents;
  EXPECT_NO_THROW(fileContents = FileContentsQuery().setFileId(fileId).execute(getTestClient()));

  FeeSchedule feeSchedule;
  EXPECT_NO_THROW(feeSchedule = FeeSchedule::fromBytes(fileContents));

  // Then
  EXPECT_EQ(fileContents, getTestFileContents());

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}
