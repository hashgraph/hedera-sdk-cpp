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
#include "TopicUpdateTransaction.h"
#include "exceptions/IllegalStateException.h"
#include "impl/DurationConverter.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class TopicUpdateTransactionTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const TopicId& getTestTopicId() const { return mTestTopicId; }
  [[nodiscard]] inline const std::string& getTestTopicMemo() const { return mTestTopicMemo; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestExpirationTime() const
  {
    return mTestExpirationTime;
  }
  [[nodiscard]] inline const std::shared_ptr<ED25519PrivateKey>& getTestAdminKey() const { return mTestAdminKey; }
  [[nodiscard]] inline const std::shared_ptr<ED25519PrivateKey>& getTestSubmitKey() const { return mTestSubmitKey; }
  [[nodiscard]] inline const std::chrono::system_clock::duration& getTestAutoRenewPeriod() const
  {
    return mTestAutoRenewPeriod;
  }
  [[nodiscard]] inline const AccountId& getTestAutoRenewAccountId() const { return mTestAutoRenewAccountId; }

private:
  const TopicId mTestTopicId = TopicId(1ULL, 2ULL, 3ULL);
  const std::string mTestTopicMemo = "test topic memo";
  const std::chrono::system_clock::time_point mTestExpirationTime = std::chrono::system_clock::now();
  const std::shared_ptr<ED25519PrivateKey> mTestAdminKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<ED25519PrivateKey> mTestSubmitKey = ED25519PrivateKey::generatePrivateKey();
  const std::chrono::system_clock::duration mTestAutoRenewPeriod = std::chrono::hours(4);
  const AccountId mTestAutoRenewAccountId = AccountId(5ULL, 6ULL, 7ULL);
};

//-----
TEST_F(TopicUpdateTransactionTest, ConstructTopicUpdateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::ConsensusUpdateTopicTransactionBody>();
  body->set_allocated_topicid(getTestTopicId().toProtobuf().release());
  body->mutable_memo()->set_value(getTestTopicMemo());
  body->set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(getTestExpirationTime()));
  body->set_allocated_adminkey(getTestAdminKey()->toProtobufKey().release());
  body->set_allocated_submitkey(getTestSubmitKey()->toProtobufKey().release());
  body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(getTestAutoRenewPeriod()));
  body->set_allocated_autorenewaccount(getTestAutoRenewAccountId().toProtobuf().release());

  proto::TransactionBody txBody;
  txBody.set_allocated_consensusupdatetopic(body.release());

  // When
  const TopicUpdateTransaction topicUpdateTransaction(txBody);

  // Then
  EXPECT_EQ(topicUpdateTransaction.getTopicId(), getTestTopicId());
  EXPECT_EQ(topicUpdateTransaction.getMemo(), getTestTopicMemo());
  EXPECT_EQ(topicUpdateTransaction.getExpirationTime(), getTestExpirationTime());
  EXPECT_EQ(topicUpdateTransaction.getAdminKey()->toBytes(), getTestAdminKey()->getPublicKey()->toBytes());
  EXPECT_EQ(topicUpdateTransaction.getSubmitKey()->toBytes(), getTestSubmitKey()->getPublicKey()->toBytes());
  EXPECT_EQ(topicUpdateTransaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  EXPECT_EQ(topicUpdateTransaction.getAutoRenewAccountId(), getTestAutoRenewAccountId());
}

//-----
TEST_F(TopicUpdateTransactionTest, ConstructTopicUpdateTransactionFromWrongTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::CryptoDeleteTransactionBody>();
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodelete(body.release());

  // When / Then
  EXPECT_THROW(const TopicUpdateTransaction topicCreateTransaction(txBody), std::invalid_argument);
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetTopicId)
{
  // Given
  TopicUpdateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setTopicId(getTestTopicId()));

  // Then
  EXPECT_EQ(transaction.getTopicId(), getTestTopicId());
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetTopicIdFrozen)
{
  // Given
  TopicUpdateTransaction transaction = TopicUpdateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setTopicId(getTestTopicId()), IllegalStateException);
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetMemo)
{
  // Given
  TopicUpdateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setMemo(getTestTopicMemo()));

  // Then
  EXPECT_EQ(transaction.getMemo(), getTestTopicMemo());
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetMemoFrozen)
{
  // Given
  TopicUpdateTransaction transaction = TopicUpdateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setMemo(getTestTopicMemo()), IllegalStateException);
}

//-----
TEST_F(TopicUpdateTransactionTest, ClearMemo)
{
  // Given
  TopicUpdateTransaction transaction;
  ASSERT_NO_THROW(transaction.setMemo(getTestTopicMemo()));

  // When
  EXPECT_NO_THROW(transaction.clearTopicMemo());

  // Then
  EXPECT_TRUE(transaction.getMemo()->empty());
}

//-----
TEST_F(TopicUpdateTransactionTest, ClearMemoFrozen)
{
  // Given
  TopicUpdateTransaction transaction = TopicUpdateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.clearTopicMemo(), IllegalStateException);
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetExpirationTime)
{
  // Given
  TopicUpdateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setExpirationTime(getTestExpirationTime()));

  // Then
  EXPECT_EQ(transaction.getExpirationTime(), getTestExpirationTime());
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetExpirationTimeFrozen)
{
  // Given
  TopicUpdateTransaction transaction = TopicUpdateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setExpirationTime(getTestExpirationTime()), IllegalStateException);
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetAdminKey)
{
  // Given
  TopicUpdateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAdminKey(getTestAdminKey()));

  // Then
  EXPECT_EQ(transaction.getAdminKey()->toBytes(), getTestAdminKey()->toBytes());
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetAdminKeyFrozen)
{
  // Given
  TopicUpdateTransaction transaction = TopicUpdateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAdminKey(getTestAdminKey()), IllegalStateException);
}

//-----
TEST_F(TopicUpdateTransactionTest, ClearAdminKey)
{
  // Given
  TopicUpdateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.clearAdminKey());

  // Then
  EXPECT_NE(transaction.getAdminKey(), nullptr);
}

//-----
TEST_F(TopicUpdateTransactionTest, ClearAdminKeyFrozen)
{
  // Given
  TopicUpdateTransaction transaction = TopicUpdateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.clearAdminKey(), IllegalStateException);
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetSubmitKey)
{
  // Given
  TopicUpdateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setSubmitKey(getTestSubmitKey()));

  // Then
  EXPECT_EQ(transaction.getSubmitKey()->toBytes(), getTestSubmitKey()->toBytes());
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetSubmitKeyFrozen)
{
  // Given
  TopicUpdateTransaction transaction = TopicUpdateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setSubmitKey(getTestSubmitKey()), IllegalStateException);
}

//-----
TEST_F(TopicUpdateTransactionTest, ClearSubmitKey)
{
  // Given
  TopicUpdateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.clearSubmitKey());

  // Then
  EXPECT_NE(transaction.getSubmitKey(), nullptr);
}

//-----
TEST_F(TopicUpdateTransactionTest, ClearSubmitKeyFrozen)
{
  // Given
  TopicUpdateTransaction transaction = TopicUpdateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.clearAdminKey(), IllegalStateException);
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetAutoRenewPeriod)
{
  // Given
  TopicUpdateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAutoRenewPeriod(getTestAutoRenewPeriod()));

  // Then
  EXPECT_EQ(transaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetAutoRenewPeriodFrozen)
{
  // Given
  TopicUpdateTransaction transaction = TopicUpdateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAutoRenewPeriod(getTestAutoRenewPeriod()), IllegalStateException);
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetAutoRenewAccountId)
{
  // Given
  TopicUpdateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAutoRenewAccountId(getTestAutoRenewAccountId()));

  // Then
  EXPECT_EQ(transaction.getAutoRenewAccountId(), getTestAutoRenewAccountId());
}

//-----
TEST_F(TopicUpdateTransactionTest, GetSetAutoRenewAccountIdFrozen)
{
  // Given
  TopicUpdateTransaction transaction = TopicUpdateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAutoRenewAccountId(getTestAutoRenewAccountId()), IllegalStateException);
}

//-----
TEST_F(TopicUpdateTransactionTest, ClearAutoRenewAccountId)
{
  // Given
  TopicUpdateTransaction transaction;
  ASSERT_NO_THROW(transaction.setAutoRenewAccountId(getTestAutoRenewAccountId()));

  // When
  EXPECT_NO_THROW(transaction.clearAutoRenewAccountId());

  // Then
  EXPECT_EQ(transaction.getAutoRenewAccountId(), AccountId());
}

//-----
TEST_F(TopicUpdateTransactionTest, ClearAutoRenewAccountIdFrozen)
{
  // Given
  TopicUpdateTransaction transaction = TopicUpdateTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.clearAutoRenewAccountId(), IllegalStateException);
}
