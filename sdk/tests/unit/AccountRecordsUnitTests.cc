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
#include "AccountRecords.h"

#include <gtest/gtest.h>
#include <proto/crypto_get_account_records.pb.h>

using namespace Hedera;

class AccountRecordsUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }

private:
  const AccountId mTestAccountId = AccountId(3ULL);
};

//-----
TEST_F(AccountRecordsUnitTests, DeserializeAccountRecordsFromProtobuf)
{
  // Given
  proto::CryptoGetAccountRecordsResponse accountRecordsResponse;
  accountRecordsResponse.set_allocated_accountid(getTestAccountId().toProtobuf().release());
  accountRecordsResponse.add_records();
  accountRecordsResponse.add_records();

  // When
  const AccountRecords accountRecords = AccountRecords::fromProtobuf(accountRecordsResponse);

  // Then
  EXPECT_EQ(accountRecords.mAccountId, getTestAccountId());
  // Don't bother testing records data as that's already tested in TransactionRecordTest
  EXPECT_EQ(accountRecords.mRecords.size(), 2);
}