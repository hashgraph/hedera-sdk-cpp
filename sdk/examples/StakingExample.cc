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
#include "AccountCreateTransaction.h"
#include "AccountInfo.h"
#include "AccountInfoQuery.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"

#include <iostream>

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
  client.setOperator(AccountId::fromString(argv[1]), ED25519PrivateKey::fromString(argv[2]));

  // Generate a ED25519 private, public key pair
  const std::unique_ptr<PrivateKey> privateKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> publicKey = privateKey->getPublicKey();

  std::cout << "Generated private key: " << privateKey->toStringRaw() << std::endl;
  std::cout << "Generated public key: " << publicKey->toStringRaw() << std::endl;

  // Create an account and stake to account ID 0.0.3.
  const AccountId newAccountId = *AccountCreateTransaction()
                                    .setKey(publicKey)
                                    .setInitialBalance(Hbar(1LL))
                                    .setStakedAccountId(AccountId(3ULL))
                                    .execute(client)
                                    .getReceipt(client)
                                    .mAccountId;

  std::cout << "Created new account with ID " << newAccountId.toString() << std::endl;

  // Query the account info, it should show the staked account ID to be 0.0.3.
  const AccountInfo accountInfo = AccountInfoQuery().setAccountId(newAccountId).execute(client);
  std::cout << "Account ID " << newAccountId.toString() << " is staked to: "
            << ((accountInfo.mStakingInfo.getStakedAccountId().has_value())
                  ? accountInfo.mStakingInfo.getStakedAccountId()->toString()
                  : "NOT STAKED")
            << std::endl;

  return 0;
}
