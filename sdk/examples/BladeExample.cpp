/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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

#include "AccountBalanceQuery.h"
#include "AccountCreateTransaction.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "ED25519PublicKey.h"
#include "TransferTransaction.h"

using namespace Hedera;

int main(int argc, char** argv)
{
  Client client = Client::forTestnet();
  client.setOperator(AccountId(argv[0]), std::make_shared<ED25519PrivateKey>(ED25519PrivateKey::fromString(argv[1])));

  ED25519PublicKey publicKey;
  ED25519PrivateKey privateKey;

  AccountCreateTransaction trans;
  AccountBalanceQuery query;
  TransferTransaction transf;

  return 0;
}
