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
#include "AccountDeleteTransaction.h"
#include "BaseIntegrationTest.h"
#include "Defaults.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ED25519PrivateKey.h"
#include "TopicCreateTransaction.h"
#include "TopicDeleteTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class TopicCreateTransactionIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(TopicCreateTransactionIntegrationTest, ExecuteTopicCreateTransaction)
{
  // Given
  const std::string memo = "topic create test memo";
  const std::chrono::duration<double> autoRenewPeriod = DEFAULT_AUTO_RENEW_PERIOD + std::chrono::hours(10);

  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = TopicCreateTransaction()
                                .setMemo(memo)
                                .setAdminKey(operatorKey)
                                .setSubmitKey(operatorKey)
                                .setAutoRenewPeriod(autoRenewPeriod)
                                .setAutoRenewAccountId(AccountId(2ULL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  // TODO: TopicInfoQuery

  // Clean up
  ASSERT_NO_THROW(txReceipt = TopicDeleteTransaction()
                                .setTopicId(txReceipt.mTopicId.value())
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
}

//-----
TEST_F(TopicCreateTransactionIntegrationTest, CanCreateTopicWithNoFieldsSet)
{
  // Given / When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = TopicCreateTransaction().execute(getTestClient()).getReceipt(getTestClient()));

  // Then
  // TODO: TopicId
}
