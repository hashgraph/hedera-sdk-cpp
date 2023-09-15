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
#include "ED25519PrivateKey.h"
#include "TopicCreateTransaction.h"
#include "exceptions/IllegalStateException.h"
#include "impl/DurationConverter.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class TopicCreateTransactionTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::string& getTestTopicMemo() const { return mTestTopicMemo; }
  [[nodiscard]] inline const std::shared_ptr<ED25519PrivateKey>& getTestAdminKey() const { return mTestAdminKey; }
  [[nodiscard]] inline const std::shared_ptr<ED25519PrivateKey>& getTestSubmitKey() const { return mTestSubmitKey; }
  [[nodiscard]] inline const std::chrono::duration<double>& getTestAutoRenewPeriod() const
  {
    return mTestAutoRenewPeriod;
  }
  [[nodiscard]] inline const AccountId& getTestAutoRenewAccountId() const { return mTestAutoRenewAccountId; }

private:
  const std::string mTestTopicMemo = "test topic memo";
  const std::shared_ptr<ED25519PrivateKey> mTestAdminKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<ED25519PrivateKey> mTestSubmitKey = ED25519PrivateKey::generatePrivateKey();
  const std::chrono::duration<double> mTestAutoRenewPeriod = std::chrono::hours(1);
  const AccountId mTestAutoRenewAccountId = AccountId(2ULL, 3ULL, 4ULL);
};

//-----
TEST_F(TopicCreateTransactionTest, ConstructTopicCreateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::ConsensusCreateTopicTransactionBody>();
  body->set_memo(getTestTopicMemo());
  body->set_allocated_adminkey(getTestAdminKey()->toProtobufKey().release());
  body->set_allocated_submitkey(getTestSubmitKey()->toProtobufKey().release());
  body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(getTestAutoRenewPeriod()));
  body->set_allocated_autorenewaccount(getTestAutoRenewAccountId().toProtobuf().release());

  proto::TransactionBody txBody;
  txBody.set_allocated_consensuscreatetopic(body.release());

  // When
  const TopicCreateTransaction topicCreateTransaction(txBody);

  // Then
  EXPECT_EQ(topicCreateTransaction.getMemo(), getTestTopicMemo());
  EXPECT_EQ(topicCreateTransaction.getAdminKey()->toBytes(), getTestAdminKey()->getPublicKey()->toBytes());
  EXPECT_EQ(topicCreateTransaction.getSubmitKey()->toBytes(), getTestSubmitKey()->getPublicKey()->toBytes());
  EXPECT_EQ(topicCreateTransaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  EXPECT_EQ(topicCreateTransaction.getAutoRenewAccountId(), getTestAutoRenewAccountId());
}

//-----
TEST_F(TopicCreateTransactionTest, ConstructTopicCreateTransactionFromWrongTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::CryptoDeleteTransactionBody>();
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodelete(body.release());

  // When / Then
  EXPECT_THROW(const TopicCreateTransaction topicCreateTransaction(txBody), std::invalid_argument);
}

//-----
TEST_F(TopicCreateTransactionTest, GetSetMemo)
{
  // Given
  TopicCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setMemo(getTestTopicMemo()));

  // Then
  EXPECT_EQ(transaction.getMemo(), getTestTopicMemo());
}

//-----
TEST_F(TopicCreateTransactionTest, GetSetMemoFrozen)
{
  // Given
  TopicCreateTransaction transaction = TopicCreateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setMemo(getTestTopicMemo()), IllegalStateException);
}

//-----
TEST_F(TopicCreateTransactionTest, GetSetAdminKey)
{
  // Given
  TopicCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAdminKey(getTestAdminKey()));

  // Then
  EXPECT_EQ(transaction.getAdminKey()->toBytes(), getTestAdminKey()->toBytes());
}

//-----
TEST_F(TopicCreateTransactionTest, GetSetAdminKeyFrozen)
{
  // Given
  TopicCreateTransaction transaction = TopicCreateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAdminKey(getTestAdminKey()), IllegalStateException);
}

//-----
TEST_F(TopicCreateTransactionTest, GetSetSubmitKey)
{
  // Given
  TopicCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setSubmitKey(getTestSubmitKey()));

  // Then
  EXPECT_EQ(transaction.getSubmitKey()->toBytes(), getTestSubmitKey()->toBytes());
}

//-----
TEST_F(TopicCreateTransactionTest, GetSetSubmitKeyFrozen)
{
  // Given
  TopicCreateTransaction transaction = TopicCreateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setSubmitKey(getTestSubmitKey()), IllegalStateException);
}

//-----
TEST_F(TopicCreateTransactionTest, GetSetAutoRenewPeriod)
{
  // Given
  TopicCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAutoRenewPeriod(getTestAutoRenewPeriod()));

  // Then
  EXPECT_EQ(transaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
}

//-----
TEST_F(TopicCreateTransactionTest, GetSetAutoRenewPeriodFrozen)
{
  // Given
  TopicCreateTransaction transaction = TopicCreateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAutoRenewPeriod(getTestAutoRenewPeriod()), IllegalStateException);
}

//-----
TEST_F(TopicCreateTransactionTest, GetSetAutoRenewAccountId)
{
  // Given
  TopicCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAutoRenewAccountId(getTestAutoRenewAccountId()));

  // Then
  EXPECT_EQ(transaction.getAutoRenewAccountId(), getTestAutoRenewAccountId());
}

//-----
TEST_F(TopicCreateTransactionTest, GetSetAutoRenewAccountIdFrozen)
{
  // Given
  TopicCreateTransaction transaction = TopicCreateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAutoRenewAccountId(getTestAutoRenewAccountId()), IllegalStateException);
}
