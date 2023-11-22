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
#include "TransactionId.h"
#include "TransactionRecordQuery.h"

#include <gtest/gtest.h>

using namespace Hedera;

class TransactionRecordQueryUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const TransactionId& getTestTransactionId() const { return mTestTransactionId; }

private:
  const AccountId mTestAccountId = AccountId(10ULL);
  const TransactionId mTestTransactionId = TransactionId::generate(mTestAccountId);
};

TEST_F(TransactionRecordQueryUnitTests, SetTransactionId)
{
  TransactionRecordQuery query;
  const TransactionId transactionId = TransactionId::generate(getTestAccountId());
  query.setTransactionId(transactionId);
  EXPECT_EQ(query.getTransactionId(), transactionId);
}