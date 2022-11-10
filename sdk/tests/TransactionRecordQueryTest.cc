/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include "TransactionRecordQuery.h"
#include "TransactionId.h"

#include <gtest/gtest.h>

using namespace Hedera;

class TransactionRecordQueryTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }

private:
  const AccountId mTestAccountId = AccountId(0ULL, 0ULL, 10ULL);
};

TEST_F(TransactionRecordQueryTest, SetTransactionId)
{
  TransactionRecordQuery query;
  const TransactionId transactionId = TransactionId::generate(getTestAccountId());
  query.setTransactionId(TransactionId::generate(getTestAccountId()));
  EXPECT_EQ(query.getTransactionId(), transactionId);
}