/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

#include <dotenv.h>
#include <iostream>

using namespace Hiero;

int main(int argc, char** argv)
{
  dotenv::init();
  const AccountId operatorAccountId = AccountId::fromString(std::getenv("OPERATOR_ID"));
  const std::shared_ptr<PrivateKey> operatorPrivateKey = ED25519PrivateKey::fromString(std::getenv("OPERATOR_KEY"));

  // Get a client for the Hiero testnet, and set the operator account ID and key such that all generated transactions
  // will be paid for by this account and be signed by this key.
  Client client = Client::forTestnet();
  client.setOperator(operatorAccountId, operatorPrivateKey);

  const auto recipientId = AccountId(3ULL);
  const Hbar amount(10000ULL, HbarUnit::TINYBAR());

  const Hbar senderBalanceBefore = AccountBalanceQuery().setAccountId(operatorAccountId).execute(client).mBalance;
  const Hbar recipientBalanceBefore = AccountBalanceQuery().setAccountId(recipientId).execute(client).mBalance;

  std::cout << "Sender balance before transfer: " << senderBalanceBefore.toString() << std::endl;
  std::cout << "Recipient balance before transfer: " << recipientBalanceBefore.toString() << std::endl;

  TransactionResponse txResponse = TransferTransaction()
                                     .addHbarTransfer(operatorAccountId, amount.negated())
                                     .addHbarTransfer(recipientId, amount)
                                     .setTransactionMemo("transfer test")
                                     .execute(client);

  TransactionRecord txRecord = txResponse.getRecord(client);

  std::cout << "Transferred " << amount.toString() << std::endl;

  const Hbar senderBalanceAfter = AccountBalanceQuery().setAccountId(operatorAccountId).execute(client).mBalance;
  const Hbar recipientBalanceAfter = AccountBalanceQuery().setAccountId(recipientId).execute(client).mBalance;

  std::cout << "Sender balance after transfer: " << senderBalanceAfter.toString() << std::endl;
  std::cout << "Recipient balance after transfer: " << recipientBalanceAfter.toString() << std::endl;
  std::cout << "HbarTransfer memo: " << txRecord.mMemo << std::endl;

  return 0;
}
