/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "PublicKey.h"

#include <iostream>

using namespace Hedera;

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "Please input client account ID" << std::endl;
    return 1;
  }

  const AccountId accountId = AccountId::fromString(argv[1]);

  // Get a client for the Hedera testnet
  Client client = Client::forTestnet();

  // Because AccountBalanceQuery is a free query, we can make it without setting an operator on the client.
  Hbar balance = AccountBalanceQuery().setAccountId(accountId).execute(client).mBalance;

  std::cout << "Balance of account " << accountId.toString() << " is " << balance.toString() << std::endl;

  return 0;
}
