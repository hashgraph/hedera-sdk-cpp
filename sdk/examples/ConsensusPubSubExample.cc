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
#include <iostream>
#include <thread>

using namespace Hedera;

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    std::cout << "Please input account ID and private key" << std::endl;
    return 1;
  }

  // Get a client for the Hedera testnet, and set the operator account ID and key such that all generated transactions
  // will be paid for by this account and be signed by this key.
  Client client = Client::forTestnet();
  client.setOperator(AccountId::fromString(argv[1]), ED25519PrivateKey::fromString(argv[2]).get());

  const TopicId topicId = TopicCreateTransaction().execute(client).getReceipt(client).mTopicId.value();

  TopicMessageQuery query = TopicMessageQuery().setTopicId(topicId);
  SubscriptionHandle handle = query.subscribe(
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
