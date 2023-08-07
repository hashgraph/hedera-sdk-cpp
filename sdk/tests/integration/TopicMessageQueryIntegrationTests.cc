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
#include "ED25519PrivateKey.h"
#include "SubscriptionHandle.h"
#include "TopicCreateTransaction.h"
#include "TopicDeleteTransaction.h"
#include "TopicId.h"
#include "TopicMessage.h"
#include "TopicMessageQuery.h"
#include "TopicMessageSubmitTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <thread>

using namespace Hedera;

class TopicMessageQueryIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(TopicMessageQueryIntegrationTest, ExecuteTopicMessageQuery)
{
  // Given
  std::cout << "Given" << std::endl;
  const std::string topicMessage = "Hello from HCS!";
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  TopicId topicId;
  ASSERT_NO_THROW(topicId = TopicCreateTransaction()
                              .setAdminKey(operatorKey)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTopicId.value());

  std::cout << "Created topic id: " << topicId.toString() << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(5));

  // When
  std::cout << "When" << std::endl;
  bool finished = false;
  const std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  SubscriptionHandle handle = TopicMessageQuery()
                                .setTopicId(topicId)
                                .setStartTime(start)
                                .setLimit(14ULL)
                                .setCompletionHandler([&finished]() { finished = true; })
                                .subscribe(getTestClient(), [](const TopicMessage&) { return; });

  // Then
  std::cout << "Then" << std::endl;
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = TopicMessageSubmitTransaction()
                                                         .setTopicId(topicId)
                                                         .setMessage(topicMessage)
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));

  while (true)
  {
    if (finished || std::chrono::system_clock::now() > start + std::chrono::seconds(60))
    {
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(2500));
  }

  EXPECT_TRUE(finished);

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    TopicDeleteTransaction().setTopicId(topicId).execute(getTestClient()).getReceipt(getTestClient()));
}
