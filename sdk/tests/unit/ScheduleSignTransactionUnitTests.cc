/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#include "ScheduleSignTransaction.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class ScheduleSignTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ECDSAsecp256k1PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const ScheduleId& getTestScheduleId() const { return mTestScheduleId; }

private:
  Client mClient;
  const ScheduleId mTestScheduleId = ScheduleId(1ULL);
};

//-----
TEST_F(ScheduleSignTransactionTest, ConstructScheduleSignTransaction)
{
  // Given / When
  ScheduleSignTransaction transaction;

  // Then
  EXPECT_FALSE(transaction.getScheduleId().has_value());
}

//-----
TEST_F(ScheduleSignTransactionTest, ConstructScheduleSignTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::ScheduleSignTransactionBody>();
  body->set_allocated_scheduleid(getTestScheduleId().toProtobuf().release());

  proto::TransactionBody txBody;
  txBody.set_allocated_schedulesign(body.release());

  // When
  const ScheduleSignTransaction scheduleSignTransaction(txBody);

  // Then
  EXPECT_EQ(scheduleSignTransaction.getScheduleId(), getTestScheduleId());
}

//-----
TEST_F(ScheduleSignTransactionTest, GetSetScheduleId)
{
  // Given
  ScheduleSignTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setScheduleId(getTestScheduleId()));

  // Then
  EXPECT_EQ(transaction.getScheduleId(), getTestScheduleId());
}

//-----
TEST_F(ScheduleSignTransactionTest, GetSetScheduleIdFrozen)
{
  // Given
  ScheduleSignTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setScheduleId(getTestScheduleId()), IllegalStateException);
}

//-----
TEST_F(ScheduleSignTransactionTest, ClearScheduleId)
{
  // Given
  ScheduleSignTransaction transaction;
  ASSERT_NO_THROW(transaction.setScheduleId(getTestScheduleId()));

  // When
  EXPECT_NO_THROW(transaction.clearScheduleId());

  // Then
  EXPECT_FALSE(transaction.getScheduleId().has_value());
}

//-----
TEST_F(ScheduleSignTransactionTest, ClearScheduleIdFrozen)
{
  // Given
  ScheduleSignTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.clearScheduleId(), IllegalStateException);
}
