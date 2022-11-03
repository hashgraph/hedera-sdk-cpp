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
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

#include <iostream>

using namespace Hedera;

int main(int argc, char** argv)
{
  std::unique_ptr<PrivateKey> operatorPrivateKey = ED25519PrivateKey::fromString(argv[2]);

  Client client = Client::forTestnet();
  client.setOperator(AccountId(argv[1]), operatorPrivateKey);

  const std::unique_ptr<PrivateKey> privateKey = ED25519PrivateKey::generatePrivateKey();
  const std::shared_ptr<PublicKey> publicKey = privateKey->getPublicKey();

  // Query the initial operator account total
  Hbar operatorAccountTotal =
    AccountBalanceQuery().setAccountId(client.getOperatorAccountId().value()).execute(client).getBalance();
  std::cout << "Initial operator account balance: " << operatorAccountTotal.toTinybars()
            << HbarUnit::TINYBAR().getSymbol() << std::endl;

  // Create a new account with an initial balance of 1000 tinybars
  TransactionResponse txResp =
    AccountCreateTransaction().setKey(publicKey).setInitialBalance(Hbar(1000ULL, HbarUnit::TINYBAR())).execute(client);

  // Get the account ID of the newly created account
  TransactionReceipt txReceipt;
  while (!txReceipt.getAccountId().has_value())
  {
    txReceipt = txResp.getReceipt(client);
  }

  const AccountId newAccountId = txReceipt.getAccountId().value();
  std::cout << "Created new account with ID " << newAccountId.toString() << std::endl;

  // Query the account totals
  operatorAccountTotal =
    AccountBalanceQuery().setAccountId(client.getOperatorAccountId().value()).execute(client).getBalance();
  Hbar newAccountTotal = AccountBalanceQuery().setAccountId(newAccountId).execute(client).getBalance();

  std::cout << "New account balance: " << newAccountTotal.toTinybars() << HbarUnit::TINYBAR().getSymbol() << std::endl;
  std::cout << "Operator account balance: " << operatorAccountTotal.toTinybars() << HbarUnit::TINYBAR().getSymbol()
            << std::endl;

  // Create a transaction to transfer 100 tinybars
  const Hbar amountToTransfer(100ULL, HbarUnit::TINYBAR());
  txResp = TransferTransaction()
             .addUnapprovedHbarTransfer(client.getOperatorAccountId().value(), amountToTransfer.negated())
             .addUnapprovedHbarTransfer(newAccountId, amountToTransfer)
             .execute(client);

  std::cout << "Transferred " << amountToTransfer.toTinybars() << HbarUnit::TINYBAR().getSymbol()
            << " from the operator account " << client.getOperatorAccountId().value().toString()
            << " to the new account " << newAccountId.toString() << std::endl;

  // Get the transaction record
  TransactionRecord txRecord;
  while (txRecord.getTransferList().empty())
  {
    txRecord = txResp.getRecord(client);
  }

  std::cout << "Transaction record shows:" << std::endl;
  const auto transferList = txRecord.getTransferList();
  for (const auto& [accountId, amount] : transferList)
  {
    std::cout << " - Account " << accountId.toString() << " transferred " << amount.toTinybars()
              << HbarUnit::TINYBAR().getSymbol() << std::endl;
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
