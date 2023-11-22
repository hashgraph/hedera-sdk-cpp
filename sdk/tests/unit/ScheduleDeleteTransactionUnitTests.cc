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
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ScheduleDeleteTransaction.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class ScheduleDeleteTransactionUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const ScheduleId& getTestScheduleId() const { return mTestScheduleId; }

private:
  const ScheduleId mTestScheduleId = ScheduleId(1ULL);
};

//-----
TEST_F(ScheduleDeleteTransactionUnitTests, ConstructScheduleDeleteTransaction)
{
  // Given / When
  ScheduleDeleteTransaction transaction;

  // Then
  EXPECT_FALSE(transaction.getScheduleId().has_value());
}

//-----
TEST_F(ScheduleDeleteTransactionUnitTests, ConstructScheduleDeleteTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::ScheduleDeleteTransactionBody>();
  body->set_allocated_scheduleid(getTestScheduleId().toProtobuf().release());

  proto::TransactionBody txBody;
  txBody.set_allocated_scheduledelete(body.release());

  // When
  const ScheduleDeleteTransaction scheduleDeleteTransaction(txBody);

  // Then
  EXPECT_EQ(scheduleDeleteTransaction.getScheduleId(), getTestScheduleId());
}

//-----
TEST_F(ScheduleDeleteTransactionUnitTests, GetSetScheduleId)
{
  // Given
  ScheduleDeleteTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setScheduleId(getTestScheduleId()));

  // Then
  EXPECT_EQ(transaction.getScheduleId(), getTestScheduleId());
}

//-----
TEST_F(ScheduleDeleteTransactionUnitTests, GetSetScheduleIdFrozen)
{
  // Given
  ScheduleDeleteTransaction transaction = ScheduleDeleteTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setScheduleId(getTestScheduleId()), IllegalStateException);
}
