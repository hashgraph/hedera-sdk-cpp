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
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

#include <iostream>

using namespace Hedera;

int main(int argc, char** argv)
{
  if (argc < 5)
  {
    std::cout
      << "Please input operator account ID, operator private key, token ID to transfer, and account ID to transfer to"
      << std::endl;
    return 1;
  }

  // Get a client for the Hedera testnet, and set the operator account ID and key such that all generated transactions
  // will be paid for by this account and be signed by this key.
  Client client = Client::forTestnet();
  const AccountId operatorId = AccountId::fromString(argv[1]);
  client.setOperator(operatorId, ED25519PrivateKey::fromString(argv[2]).get());
  const TokenId tokenId = TokenId::fromString(argv[3]);
  const AccountId recipientId = AccountId::fromString(argv[4]);

  const int64_t amount = 10LL;

  TransactionResponse txResponse = TransferTransaction()
                                     .addTokenTransfer(tokenId, operatorId, -amount)
                                     .addTokenTransfer(tokenId, recipientId, amount)
                                     .execute(client);

  TransactionRecord txRecord = txResponse.getRecord(client);

  std::cout << "List of token transfers received in TransactionRecord:" << std::endl;
  for (const TokenTransfer& transfer : txRecord.getTokenTransferList())
  {
    std::cout << "---TRANSFER---" << std::endl;
    std::cout << " - Token ID: " << transfer.getTokenId().toString() << std::endl;
    std::cout << " - Account ID: " << transfer.getAccountId().toString() << std::endl;
    std::cout << " - Amount: " << transfer.getAmount() << std::endl;
  }

  return 0;
}
