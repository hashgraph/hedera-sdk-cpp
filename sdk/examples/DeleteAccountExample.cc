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
#include "AccountDeleteTransaction.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "PublicKey.h"
#include "Status.h"
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
  const AccountId operatorAccountId = AccountId::fromString(argv[1]);
  client.setOperator(operatorAccountId, ED25519PrivateKey::fromString(argv[2]).get());

  // Generate a ED25519 private, public key pair
  const std::unique_ptr<PrivateKey> privateKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> publicKey = privateKey->getPublicKey();

  std::cout << "Generated private key: " << privateKey->toStringRaw() << std::endl;
  std::cout << "Generated public key: " << publicKey->toStringRaw() << std::endl;

  // Create a new account with an initial balance of 2 Hbars. The only required field here is the key.
  TransactionResponse txResp =
    AccountCreateTransaction().setKey(publicKey.get()).setInitialBalance(Hbar(2LL)).execute(client);

  // Get the receipt when it becomes available
  TransactionReceipt txReceipt = txResp.getReceipt(client);

  const AccountId newAccountId = txReceipt.mAccountId.value();
  std::cout << "Created new account with ID " << newAccountId.toString() << std::endl;

  // Delete the newly-created account
  txReceipt = AccountDeleteTransaction()
                .setDeleteAccountId(newAccountId)
                .setTransferAccountId(operatorAccountId)
                .freezeWith(client)
                .sign(privateKey.get())
                .execute(client)
                .getReceipt(client);

  std::cout << "Deleted account with response code: " << gStatusToString.at(txReceipt.mStatus) << std::endl;

  return 0;
}
