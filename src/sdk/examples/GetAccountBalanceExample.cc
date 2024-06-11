/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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

#include <dotenv.h>
#include <iostream>

using namespace Hedera;

int main(int argc, char** argv)
{
  dotenv::init();
  const AccountId operatorAccountId = AccountId::fromString("0.0.2202375");

  // Get a client for the Hedera testnet
  Client client = Client::forTestnet();

  // Because AccountBalanceQuery is a free query, we can make it without setting an operator on the client.
  AccountBalance accountBalance = AccountBalanceQuery().setAccountId(operatorAccountId).execute(client);

  std::cout << "Balance of account " << operatorAccountId.toString() << " is " << accountBalance.mBalance.toString()
            << std::endl;

  std::cout << "Token balances of account" << std::endl;
  for (auto tokenBalance : accountBalance.mTokens)
  {
    std::cout << tokenBalance.first.toString() << ": " << tokenBalance.second << std::endl;
  }

  return 0;
}
