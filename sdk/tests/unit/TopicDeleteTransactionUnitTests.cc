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
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "TopicDeleteTransaction.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class TopicDeleteTransactionUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const TopicId& getTestTopicId() const { return mTestTopicId; }

private:
  const TopicId mTestTopicId = TopicId(1ULL);
};

//-----
TEST_F(TopicDeleteTransactionUnitTests, ConstructTopicDeleteTransaction)
{
  // Given / When
  TopicDeleteTransaction transaction;

  // Then
  EXPECT_FALSE(transaction.getTopicId().has_value());
}

//-----
TEST_F(TopicDeleteTransactionUnitTests, ConstructTopicDeleteTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::ConsensusDeleteTopicTransactionBody>();
  body->set_allocated_topicid(getTestTopicId().toProtobuf().release());

  proto::TransactionBody txBody;
  txBody.set_allocated_consensusdeletetopic(body.release());

  // When
  const TopicDeleteTransaction topicDeleteTransaction(txBody);

  // Then
  EXPECT_EQ(topicDeleteTransaction.getTopicId(), getTestTopicId());
}

//-----
TEST_F(TopicDeleteTransactionUnitTests, GetSetTopicId)
{
  // Given
  TopicDeleteTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setTopicId(getTestTopicId()));

  // Then
  EXPECT_EQ(transaction.getTopicId(), getTestTopicId());
}

//-----
TEST_F(TopicDeleteTransactionUnitTests, GetSetTopicIdFrozen)
{
  // Given
  TopicDeleteTransaction transaction = TopicDeleteTransaction()
                                         .setNodeAccountIds({ AccountId(1ULL) })
                                         .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setTopicId(getTestTopicId()), IllegalStateException);
}
