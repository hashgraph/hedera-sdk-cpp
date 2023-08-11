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
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "Status.h"
#include "TokenAssociateTransaction.h"
#include "TokenCreateTransaction.h"
#include "TokenDeleteTransaction.h"
#include "TokenGrantKycTransaction.h"
#include "TokenId.h"
#include "TokenWipeTransaction.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

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
  const AccountId operatorAccountId = AccountId::fromString(argv[1]);
  const std::shared_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(argv[2]);

  Client client = Client::forTestnet();
  client.setOperator(operatorAccountId, operatorKey.get());

  // Generate two accounts.
  const std::unique_ptr<PrivateKey> privateKey1 = ED25519PrivateKey::generatePrivateKey();
  const std::unique_ptr<PrivateKey> privateKey2 = ED25519PrivateKey::generatePrivateKey();

  const AccountId accountId1 = AccountCreateTransaction()
                                 .setKey(privateKey1.get())
                                 .setInitialBalance(Hbar(1LL))
                                 .execute(client)
                                 .getReceipt(client)
                                 .mAccountId.value();
  std::cout << "Generated account with account ID " << accountId1.toString() << std::endl;

  const AccountId accountId2 = AccountCreateTransaction()
                                 .setKey(privateKey2.get())
                                 .setInitialBalance(Hbar(1LL))
                                 .execute(client)
                                 .getReceipt(client)
                                 .mAccountId.value();
  std::cout << "Generated account with account ID " << accountId2.toString() << std::endl;

  // Create a token to transfer.
  const TokenId tokenId = TokenCreateTransaction()
                            .setTokenName("ffff")
                            .setTokenSymbol("F")
                            .setTreasuryAccountId(operatorAccountId)
                            .setInitialSupply(100000ULL)
                            .setAdminKey(operatorKey)
                            .setKycKey(operatorKey)
                            .setWipeKey(operatorKey)
                            .execute(client)
                            .getReceipt(client)
                            .mTokenId.value();
  std::cout << "Generated token with token ID " << tokenId.toString() << std::endl;

  // Associate the token with the two accounts.
  std::cout << "Associate the token with account 1: "
            << gStatusToString.at(TokenAssociateTransaction()
                                    .setAccountId(accountId1)
                                    .setTokenIds({ tokenId })
                                    .freezeWith(&client)
                                    .sign(privateKey1.get())
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;

  std::cout << "Associate the token with account 2: "
            << gStatusToString.at(TokenAssociateTransaction()
                                    .setAccountId(accountId2)
                                    .setTokenIds({ tokenId })
                                    .freezeWith(&client)
                                    .sign(privateKey2.get())
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;

  // Grant KYC to these accounts for this token.
  std::cout << "Grant KYC to account 1 for the token: "
            << gStatusToString.at(TokenGrantKycTransaction()
                                    .setAccountId(accountId1)
                                    .setTokenId(tokenId)
                                    .freezeWith(&client)
                                    .sign(privateKey1.get())
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;

  std::cout << "Grant KYC to account 2 for the token: "
            << gStatusToString.at(TokenGrantKycTransaction()
                                    .setAccountId(accountId2)
                                    .setTokenId(tokenId)
                                    .freezeWith(&client)
                                    .sign(privateKey2.get())
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;

  // Send ten tokens to account 1.
  std::cout << "Send 10 tokens from the treasury to account 1: "
            << gStatusToString.at(TransferTransaction()
                                    .addTokenTransfer(tokenId, operatorAccountId, -10LL)
                                    .addTokenTransfer(tokenId, accountId1, 10LL)
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;

  // Send the ten tokens from account 1 to account 2.
  std::cout << "Send 10 tokens from account 1 to account 2: "
            << gStatusToString.at(TransferTransaction()
                                    .addTokenTransfer(tokenId, accountId1, -10LL)
                                    .addTokenTransfer(tokenId, accountId2, 10LL)
                                    .freezeWith(&client)
                                    .sign(privateKey1.get())
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;

  // Send the ten tokens from account 2 back to account 1.
  std::cout << "Send 10 tokens from account 2 back to account 1: "
            << gStatusToString.at(TransferTransaction()
                                    .addTokenTransfer(tokenId, accountId1, 10LL)
                                    .addTokenTransfer(tokenId, accountId2, -10LL)
                                    .freezeWith(&client)
                                    .sign(privateKey2.get())
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;

  // Wipe the account of the 10 tokens.
  std::cout << "Wipe the 10 tokens from account 1: "
            << gStatusToString.at(TokenWipeTransaction()
                                    .setTokenId(tokenId)
                                    .setAccountId(accountId1)
                                    .setAmount(10LL)
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;

  // Delete the tokens and the accounts.
  std::cout << "Delete the token: "
            << gStatusToString.at(
                 TokenDeleteTransaction().setTokenId(tokenId).execute(client).getReceipt(client).mStatus)
            << std::endl;
  std::cout << "Delete account 1: "
            << gStatusToString.at(AccountDeleteTransaction()
                                    .setTransferAccountId(operatorAccountId)
                                    .setDeleteAccountId(accountId1)
                                    .freezeWith(&client)
                                    .sign(privateKey1.get())
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;
  std::cout << "Delete account 2: "
            << gStatusToString.at(AccountDeleteTransaction()
                                    .setTransferAccountId(operatorAccountId)
                                    .setDeleteAccountId(accountId2)
                                    .freezeWith(&client)
                                    .sign(privateKey2.get())
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;

  return 0;
}
