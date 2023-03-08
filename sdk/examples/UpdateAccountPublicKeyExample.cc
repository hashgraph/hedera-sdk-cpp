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
#include "AccountCreateTransaction.h"
#include "AccountUpdateTransaction.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
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

  // Generate an ECDSAsecp256k1 public key to use for the new account
  const std::shared_ptr<PrivateKey> privateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> publicKey = privateKey->getPublicKey();

  // Create a new account with an initial balance of 1000 tinybars. The only required field here is the key.
  TransactionResponse txResp =
    AccountCreateTransaction().setKey(publicKey).setInitialBalance(Hbar(1000ULL, HbarUnit::TINYBAR())).execute(client);

  // Get the receipt when it becomes available
  TransactionReceipt txReceipt = txResp.getReceipt(client);

  const AccountId newAccountId = txReceipt.getAccountId().value();
  std::cout << "Created new account with ID " << newAccountId.toString() << " and public key " << publicKey->toString()
            << std::endl;

  // Generate a new ED25519PrivateKey public key with which to update the account.
  const std::unique_ptr<PrivateKey> newPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> newPublicKey = newPrivateKey->getPublicKey();

  // Update the account
  std::cout << "Updating account to use new public key: " << newPublicKey->toString() << std::endl;
  txResp = AccountUpdateTransaction()
             .setAccountId(newAccountId)
             .setKey(newPublicKey)
             .freezeWith(client)
             .sign(privateKey.get())
             .sign(newPrivateKey.get())
             .execute(client);

  txReceipt = txResp.getReceipt(client);
  std::cout << "Transaction response: " << gStatusToString.at(txReceipt.getStatus()) << std::endl;

  return 0;
}
