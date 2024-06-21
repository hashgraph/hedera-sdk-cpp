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
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "TokenAssociateTransaction.h"
#include "TokenCreateTransaction.h"
#include "TokenDeleteTransaction.h"
#include "TokenFreezeTransaction.h"
#include "TokenInfo.h"
#include "TokenInfoQuery.h"
#include "TokenUpdateTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

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

  std::shared_ptr<PrivateKey> accountKey = ED25519PrivateKey::generatePrivateKey();

  AccountId accountId = AccountCreateTransaction()
                          .setKey(accountKey)
                          .setInitialBalance(Hbar(1LL))
                          .execute(client)
                          .getReceipt(client)
                          .mAccountId.value();

  std::cout << "1" << std::endl;

  TokenId tokenId = TokenCreateTransaction()
                      .setTokenName("ffff")
                      .setTokenSymbol("F")
                      .setInitialSupply(100000ULL)
                      .setTreasuryAccountId(AccountId(2ULL))
                      .setAdminKey(operatorPrivateKey)
                      .setFreezeKey(operatorPrivateKey)
                      .execute(client)
                      .getReceipt(client)
                      .mTokenId.value();

  std::cout << "2" << std::endl;

  TransactionReceipt txReceipt = TokenAssociateTransaction()
                                   .setAccountId(accountId)
                                   .setTokenIds({ tokenId })
                                   .freezeWith(&client)
                                   .sign(accountKey)
                                   .execute(client)
                                   .getReceipt(client);

  std::cout << "3" << std::endl;

  txReceipt = TokenFreezeTransaction()
                .setAccountId(accountId)
                .setTokenId(tokenId)
                .freezeWith(&client)
                .sign(accountKey)
                .execute(client)
                .getReceipt(client);

  txReceipt = TransferTransaction()
                .addTokenTransfer(tokenId, AccountId(2ULL), -10LL)
                .addTokenTransfer(tokenId, accountId, 10LL)
                .execute(client)
                .getReceipt(client); // ACCOUNT_FROZEN_FOR_TOKEN

  txReceipt = AccountDeleteTransaction()
                .setDeleteAccountId(accountId)
                .setTransferAccountId(AccountId(2ULL))
                .freezeWith(&client)
                .sign(accountKey)
                .execute(client)
                .getReceipt(client);

  txReceipt = TokenDeleteTransaction().setTokenId(tokenId).execute(client).getReceipt(client);

  return 0;
}