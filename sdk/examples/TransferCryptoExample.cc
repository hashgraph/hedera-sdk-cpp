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
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "PrivateKey.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

#include <iostream>
#include <memory>

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
  const auto operatorId = std::make_shared<AccountId>(argv[1]);
  client.setOperator(operatorId, ED25519PrivateKey::fromString(argv[2]));

  const auto recipientId = std::make_shared<AccountId>("0.0.3");
  const Hbar amount(10000ULL, HbarUnit::TINYBAR());

  const Hbar senderBalanceBefore = AccountBalanceQuery().setAccountId(*operatorId).execute(client).getBalance();
  const Hbar recipientBalanceBefore = AccountBalanceQuery().setAccountId(*recipientId).execute(client).getBalance();

  std::cout << "Sender balance before transfer: " << senderBalanceBefore.toTinybars() << HbarUnit::TINYBAR().getSymbol()
            << std::endl;
  std::cout << "Recipient balance before transfer: " << recipientBalanceBefore.toTinybars()
            << HbarUnit::TINYBAR().getSymbol() << std::endl;

  TransactionResponse txResponse = TransferTransaction()
                                     .addUnapprovedHbarTransfer(operatorId, amount.negated())
                                     .addUnapprovedHbarTransfer(recipientId, amount)
                                     .setTransactionMemo("transfer test")
                                     .execute(client);

  TransactionRecord txRecord = txResponse.getRecord(client);

  std::cout << "Transferred " << amount.toTinybars() << HbarUnit::TINYBAR().getSymbol() << std::endl;

  const Hbar senderBalanceAfter = AccountBalanceQuery().setAccountId(*operatorId).execute(client).getBalance();
  const Hbar recipientBalanceAfter = AccountBalanceQuery().setAccountId(*recipientId).execute(client).getBalance();

  std::cout << "Sender balance after transfer: " << senderBalanceAfter.toTinybars() << HbarUnit::TINYBAR().getSymbol()
            << std::endl;
  std::cout << "Recipient balance after transfer: " << recipientBalanceAfter.toTinybars()
            << HbarUnit::TINYBAR().getSymbol() << std::endl;
  std::cout << "Transfer memo: " << txRecord.getTransactionMemo() << std::endl;

  return 0;
}
