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
#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "AccountCreateTransaction.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

#include <iostream>

using namespace Hedera;

int main(int argc, char** argv)
{
  Client client = Client::forTestnet();
  client.setOperator(AccountId(argv[0]), ED25519PrivateKey::fromString(argv[1]));

  const std::shared_ptr<PrivateKey> privateKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> publicKey = privateKey->getPublicKey();

  // Create a new account with an initial balance of 1 Hbar
  TransactionResponse txResp =
    AccountCreateTransaction().setKey(publicKey).setInitialBalance(Hbar(1ULL)).execute(client);

  // Get the account ID of the newly created account
  const AccountId newAccountId = txResp.getReceipt(client).getAccountId().value();

  std::cout << "Created new account with ID " << newAccountId.toString() << std::endl;

  // Query the account totals
  Hbar operatorAccountTotal =
    AccountBalanceQuery().setAccountId(client.getOperatorAccountId().value()).execute(client).getBalance();
  Hbar newAccountTotal = AccountBalanceQuery().setAccountId(newAccountId).execute(client).getBalance();

  std::cout << "New account balance: " << newAccountTotal.toTinybars() << HbarUnit::TINYBAR().getSymbol() << std::endl;
  std::cout << "Operator account balance: " << operatorAccountTotal.toTinybars() << HbarUnit::TINYBAR().getSymbol()
            << std::endl;

  // Create a transaction to transfer 1 Hbar
  const Hbar amountToTransfer(1ULL);
  txResp = TransferTransaction()
             .addUnapprovedHbarTransfer(client.getOperatorAccountId().value(), amountToTransfer.negated())
             .addUnapprovedHbarTransfer(newAccountId, amountToTransfer)
             .execute(client);

  // Print off the transfers
  if (const TransactionRecord txRecord = txResp.getRecord(client); txRecord.getTransferList().has_value())
  {
    std::cout << "Transaction record shows:" << std::endl;
    const auto transferList = txRecord.getTransferList().value();
    for (const auto& [accountId, amount] : transferList)
    {
      std::cout << " - Account " << accountId.toString() << " transferred " << amount.toTinybars()
                << HbarUnit::TINYBAR().getSymbol() << std::endl;
    }
  }

  // Get the new account totals
  operatorAccountTotal =
    AccountBalanceQuery().setAccountId(client.getOperatorAccountId().value()).execute(client).getBalance();
  newAccountTotal = AccountBalanceQuery().setAccountId(newAccountId).execute(client).getBalance();

  std::cout << "New account balance: " << newAccountTotal.toTinybars() << HbarUnit::TINYBAR().getSymbol() << std::endl;
  std::cout << "Operator account balance: " << operatorAccountTotal.toTinybars() << HbarUnit::TINYBAR().getSymbol()
            << std::endl;

  return 0;
}
