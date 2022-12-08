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
  [[nodiscard]] inline const std::shared_ptr<AccountId>& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const TransactionId& getTestTransactionId() const { return mTestTransactionId; }

private:
  const std::shared_ptr<AccountId> mTestAccountId = std::make_shared<AccountId>(10ULL);
  const TransactionId mTestTransactionId = TransactionId::generate(mTestAccountId);
};

TEST_F(TransactionRecordQueryTest, CloneTransactionReceiptQuery)
{
  TransactionRecordQuery transactionRecordQuery;
  transactionRecordQuery.setNodeAccountIds({ getTestAccountId() });
  transactionRecordQuery.setTransactionId(getTestTransactionId());

  auto clonedExecutablePtr = transactionRecordQuery.clone();
  EXPECT_EQ(clonedExecutablePtr->getNodeAccountIds().size(), transactionRecordQuery.getNodeAccountIds().size());
  EXPECT_EQ(*clonedExecutablePtr->getNodeAccountIds().at(0), *getTestAccountId());

  // TODO: get and test Query derived class members when they're added

  auto clonedTransactionReceiptQueryPtr = dynamic_cast<TransactionRecordQuery*>(clonedExecutablePtr.get());
  EXPECT_TRUE(clonedTransactionReceiptQueryPtr->getTransactionId());
  EXPECT_EQ(*clonedTransactionReceiptQueryPtr->getTransactionId(), getTestTransactionId());
}

TEST_F(TransactionRecordQueryTest, SetTransactionId)
{
  TransactionRecordQuery query;
  const TransactionId transactionId = TransactionId::generate(getTestAccountId());
  query.setTransactionId(transactionId);
  EXPECT_EQ(query.getTransactionId(), transactionId);
}