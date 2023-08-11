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
#include "BaseIntegrationTest.h"
#include "Defaults.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ED25519PrivateKey.h"
#include "TopicCreateTransaction.h"
#include "TopicDeleteTransaction.h"
#include "TopicInfo.h"
#include "TopicInfoQuery.h"
#include "TopicUpdateTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class TopicUpdateTransactionIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(TopicUpdateTransactionIntegrationTest, ExecuteTopicUpdateTransaction)
{
  // Given
  const std::string newMemo = "new topic create test memo";
  const std::chrono::duration<double> newAutoRenewPeriod = DEFAULT_AUTO_RENEW_PERIOD + std::chrono::hours(10);

  std::shared_ptr<PrivateKey> operatorKey;
  std::shared_ptr<PrivateKey> newKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));
  ASSERT_NO_THROW(newKey = ED25519PrivateKey::generatePrivateKey());

  TopicId topicId;
  ASSERT_NO_THROW(topicId = TopicCreateTransaction()
                              .setAdminKey(operatorKey)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTopicId.value());

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = TopicUpdateTransaction()
                                .setTopicId(topicId)
                                .setMemo(newMemo)
                                .setAdminKey(newKey)
                                .setSubmitKey(newKey)
                                .setAutoRenewPeriod(newAutoRenewPeriod)
                                .setAutoRenewAccountId(AccountId(2ULL))
                                .freezeWith(&getTestClient())
                                .sign(newKey.get())
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  TopicInfo topicInfo;
  ASSERT_NO_THROW(topicInfo = TopicInfoQuery().setTopicId(topicId).execute(getTestClient()));

  EXPECT_EQ(topicInfo.mTopicId, topicId);
  EXPECT_EQ(topicInfo.mMemo, newMemo);
  ASSERT_NE(topicInfo.mAdminKey, nullptr);
  EXPECT_EQ(topicInfo.mAdminKey->toBytes(), newKey->getPublicKey()->toBytes());
  ASSERT_NE(topicInfo.mSubmitKey, nullptr);
  EXPECT_EQ(topicInfo.mSubmitKey->toBytes(), newKey->getPublicKey()->toBytes());
  ASSERT_TRUE(topicInfo.mAutoRenewPeriod.has_value());
  EXPECT_EQ(topicInfo.mAutoRenewPeriod.value(), newAutoRenewPeriod);
  ASSERT_TRUE(topicInfo.mAutoRenewAccountId.has_value());
  EXPECT_EQ(topicInfo.mAutoRenewAccountId.value(), AccountId(2ULL));

  // Clean up
  ASSERT_NO_THROW(txReceipt = TopicDeleteTransaction()
                                .setTopicId(topicId)
                                .freezeWith(&getTestClient())
                                .sign(newKey.get())
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
}
