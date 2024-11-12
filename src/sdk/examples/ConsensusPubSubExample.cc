/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "ED25519PrivateKey.h"
#include "SubscriptionHandle.h"
#include "TopicCreateTransaction.h"
#include "TopicId.h"
#include "TopicMessage.h"
#include "TopicMessageQuery.h"
#include "TopicMessageSubmitTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "impl/Utilities.h"

#include <chrono>
#include <dotenv.h>
#include <iostream>
#include <thread>

using namespace Hiero;

int main(int argc, char** argv)
{
  dotenv::init();
  const AccountId operatorAccountId = AccountId::fromString(std::getenv("OPERATOR_ID"));
  const std::shared_ptr<PrivateKey> operatorPrivateKey = ED25519PrivateKey::fromString(std::getenv("OPERATOR_KEY"));

  // Get a client for the Hiero testnet, and set the operator account ID and key such that all generated transactions
  // will be paid for by this account and be signed by this key.
  Client client = Client::forTestnet();
  client.setOperator(operatorAccountId, operatorPrivateKey);

  const TopicId topicId = TopicCreateTransaction().execute(client).getReceipt(client).mTopicId.value();

  TopicMessageQuery query = TopicMessageQuery().setTopicId(topicId);
  std::shared_ptr<SubscriptionHandle> handle = query.subscribe(
    client,
    [](const TopicMessage& message)
    { std::cout << "Received message: " << internal::Utilities::byteVectorToString(message.mContents) << std::endl; });

  for (int i = 0; true; ++i)
  {
    const TransactionReceipt txReceipt = TopicMessageSubmitTransaction()
                                           .setTopicId(topicId)
                                           .setMessage("Hello from HCS " + std::to_string(i))
                                           .execute(client)
                                           .getReceipt(client);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  return 0;
}
