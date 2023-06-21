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
#include "AccountId.h"
#include "Client.h"
#include "CustomFractionalFee.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "Status.h"
#include "TokenCreateTransaction.h"
#include "TokenType.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

#include <iostream>
#include <memory>

using namespace Hedera;

/*
 * Example for HIP-573: Blanket exemptions for custom fee collectors:
 *   1. Create accounts A, B, and C.
 *   2. Create a fungible token that has three fractional fees.
 *    - Fee #1 sends 1/100 of the transferred value to collector 0.0.A.
 *    - Fee #2 sends 2/100 of the transferred value to collector 0.0.B.
 *    - Fee #3 sends 3/100 of the transferred value to collector 0.0.C.
 *   3. Collector 0.0.B sends 10,000 units of the token to 0.0.A.
 *   4. Show that the fee collector accounts in the custom fee list of the token that was created was not charged a
 *      custom fee in the transfer.
 */
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
  client.setOperator(AccountId::fromString(argv[1]), ED25519PrivateKey::fromString(argv[2]).get());

  /**
   * Step 1: Create accounts A, B, and C.
   */
  const std::shared_ptr<ED25519PrivateKey> firstAccountPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const AccountId firstAccountId = AccountCreateTransaction()
                                     .setInitialBalance(Hbar(10LL))
                                     .setKey(firstAccountPrivateKey.get())
                                     .freezeWith(client)
                                     .sign(firstAccountPrivateKey.get())
                                     .execute(client)
                                     .getReceipt(client)
                                     .getAccountId()
                                     .value();
  std::cout << "Generated account with ID: " << firstAccountId.toString() << std::endl;

  const std::shared_ptr<ED25519PrivateKey> secondAccountPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const AccountId secondAccountId = AccountCreateTransaction()
                                      .setInitialBalance(Hbar(10LL))
                                      .setKey(secondAccountPrivateKey.get())
                                      .freezeWith(client)
                                      .sign(secondAccountPrivateKey.get())
                                      .execute(client)
                                      .getReceipt(client)
                                      .getAccountId()
                                      .value();
  std::cout << "Generated account with ID: " << secondAccountId.toString() << std::endl;

  const std::shared_ptr<ED25519PrivateKey> thirdAccountPrivateKey = ED25519PrivateKey::generatePrivateKey();
  const AccountId thirdAccountId = AccountCreateTransaction()
                                     .setInitialBalance(Hbar(10LL))
                                     .setKey(thirdAccountPrivateKey.get())
                                     .freezeWith(client)
                                     .sign(thirdAccountPrivateKey.get())
                                     .execute(client)
                                     .getReceipt(client)
                                     .getAccountId()
                                     .value();
  std::cout << "Generated account with ID: " << thirdAccountId.toString() << std::endl;

  /**
   * Step 2: Create a fungible token that has three fractional fees.
   *    - Fee #1 sends 1/100 of the transferred value to collector 0.0.A.
   *    - Fee #2 sends 2/100 of the transferred value to collector 0.0.B.
   *    - Fee #3 sends 3/100 of the transferred value to collector 0.0.C.
   */
  const TokenId createdTokenId =
    TokenCreateTransaction()
      .setTokenName("HIP-573 Token")
      .setTokenSymbol("H573")
      .setInitialSupply(100000000ULL)
      .setTreasuryAccountId(client.getOperatorAccountId().value())
      .setAdminKey(client.getOperatorPublicKey())
      .setFreezeKey(client.getOperatorPublicKey())
      .setWipeKey(client.getOperatorPublicKey())
      .setAutoRenewAccountId(client.getOperatorAccountId().value())
      .setTokenType(TokenType::FUNGIBLE_COMMON)
      .setCustomFees({ std::make_shared<CustomFractionalFee>(CustomFractionalFee()
                                                               .setFeeCollectorAccountId(firstAccountId)
                                                               .setNumerator(1LL)
                                                               .setDenominator(10LL)
                                                               .setAllCollectorsAreExempt(true)),
                       std::make_shared<CustomFractionalFee>(CustomFractionalFee()
                                                               .setFeeCollectorAccountId(secondAccountId)
                                                               .setNumerator(2LL)
                                                               .setDenominator(10LL)
                                                               .setAllCollectorsAreExempt(true)),
                       std::make_shared<CustomFractionalFee>(CustomFractionalFee()
                                                               .setFeeCollectorAccountId(thirdAccountId)
                                                               .setNumerator(3LL)
                                                               .setDenominator(10LL)
                                                               .setAllCollectorsAreExempt(true)) })
      .freezeWith(client)
      .sign(firstAccountPrivateKey.get())
      .sign(secondAccountPrivateKey.get())
      .sign(thirdAccountPrivateKey.get())
      .execute(client)
      .getReceipt(client)
      .getTokenId()
      .value();
  std::cout << "Created token with ID: " << createdTokenId.toString() << std::endl;

  /**
   * Step 3: Collector 0.0.B sends 10,000 units of the token to 0.0.A.
   */

  // First send 10,000 units to account B from the operator.
  std::cout << "Transfer 10,000 units to account B: ";
  TransactionReceipt txReceipt = TransferTransaction()
                                   .addTokenTransfer(createdTokenId, client.getOperatorAccountId().value(), -10000LL)
                                   .addTokenTransfer(createdTokenId, secondAccountId, 10000LL)
                                   .execute(client)
                                   .getReceipt(client);
  std::cout << gStatusToString.at(txReceipt.getStatus()) << std::endl;

  std::cout << "Transfer 10,000 units from account B to account A: ";
  TransactionRecord txRecord = TransferTransaction()
                                 .addTokenTransfer(createdTokenId, secondAccountId, -10000LL)
                                 .addTokenTransfer(createdTokenId, firstAccountId, 10000LL)
                                 .freezeWith(client)
                                 .sign(secondAccountPrivateKey.get())
                                 .execute(client)
                                 .getRecord(client);
  std::cout << gStatusToString.at(txRecord.mReceipt->getStatus()) << std::endl;

  /**
   * Step 4: Show that the fee collector accounts in the custom fee list of the token that was created was not charged a
   *         custom fee in the transfer. The `else` block here should not run.
   */
  if (txRecord.mAssessedCustomFees.empty())
  {
    std::cout << "No fees were collected!" << std::endl;
  }
  else
  {
    std::cout << "Fees were collected: " << std::endl;
    for (const auto& assessedCustomFee : txRecord.mAssessedCustomFees)
    {
      std::cout << " - Account " << assessedCustomFee.mFeeCollectorAccountId.toString() << " collected "
                << assessedCustomFee.mAmount << " of "
                << (assessedCustomFee.mTokenId.has_value() ? ("token " + assessedCustomFee.mTokenId->toString())
                                                           : "Hbar")
                << "from ";
      for (const auto& account : assessedCustomFee.mPayerAccountIdList)
      {
        std::cout << account.toString() << ' ';
      }
      std::cout << std::endl;
    }
  }

  return 0;
}
