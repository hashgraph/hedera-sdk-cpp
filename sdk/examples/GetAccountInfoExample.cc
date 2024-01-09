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
#include "AccountInfo.h"
#include "AccountInfoQuery.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "PublicKey.h"

#include <dotenv.h>
#include <iostream>

using namespace Hedera;

int main(int argc, char** argv)
{
  dotenv::init();
  const AccountId operatorAccountId = AccountId::fromString(std::getenv("OPERATOR_ID"));
  const std::shared_ptr<PrivateKey> operatorPrivateKey = ED25519PrivateKey::fromString(std::getenv("OPERATOR_KEY"));

  // Get a client for the Hedera testnet, and set the operator account ID and key such that all generated transactions
  // will be paid for by this account and be signed by this key.
  Client client = Client::forTestnet();
  client.setOperator(operatorAccountId, operatorPrivateKey);

  const AccountInfo accountInfo =
    AccountInfoQuery().setAccountId(client.getOperatorAccountId().value()).execute(client);

  std::cout << "Account key:                         "
            << std::dynamic_pointer_cast<PublicKey>(accountInfo.mKey)->toStringRaw() << std::endl;
  std::cout << "Account receiver signature required: " << (accountInfo.mReceiverSignatureRequired ? "true" : "false")
            << std::endl;
  std::cout << "Account expiration time:             " << accountInfo.mExpirationTime.time_since_epoch().count()
            << std::endl;

  return 0;
}
