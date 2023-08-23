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
#include "ED25519PrivateKey.h"
#include "KeyList.h"
#include "ScheduleCreateTransaction.h"
#include "ScheduleInfo.h"
#include "ScheduleInfoQuery.h"
#include "ScheduleSignTransaction.h"
#include "Status.h"
#include "TransactionReceipt.h"
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

  const AccountId operatorAccountId = AccountId::fromString(argv[1]);
  const std::shared_ptr<PrivateKey> operatorPrivateKey = ED25519PrivateKey::fromString(argv[2]);

  // Get a client for the Hedera testnet, and set the operator account ID and key such that all generated transactions
  // will be paid for by this account and be signed by this key.
  Client client = Client::forTestnet();
  client.setOperator(operatorAccountId, operatorPrivateKey.get());

  // Create an account with a Keylist of three keys.
  const std::shared_ptr<PrivateKey> key1 = ED25519PrivateKey::generatePrivateKey();
  std::cout << "Generated key: " << key1->toStringRaw() << std::endl;

  const std::shared_ptr<PrivateKey> key2 = ED25519PrivateKey::generatePrivateKey();
  std::cout << "Generated key: " << key2->toStringRaw() << std::endl;

  const std::shared_ptr<PrivateKey> key3 = ED25519PrivateKey::generatePrivateKey();
  std::cout << "Generated key: " << key3->toStringRaw() << std::endl;

  // Put the three generated keys in a keylist.
  const KeyList keyList = KeyList::of({ key1.get(), key2.get(), key3.get() });

  // Create an account with the three keys.
  std::cout << "Creating account with generated keys: ";
  TransactionReceipt txReceipt =
    AccountCreateTransaction().setKey(&keyList).setInitialBalance(Hbar(10LL)).execute(client).getReceipt(client);
  std::cout << gStatusToString.at(txReceipt.mStatus) << std::endl;

  // Schedule a transfer out of the created account with 2/3 needed signatures.
  const AccountId accountId = txReceipt.mAccountId.value();
  const ScheduleId scheduleId = TransferTransaction()
                                  .addHbarTransfer(accountId, Hbar(-1LL))
                                  .addHbarTransfer(operatorAccountId, Hbar(1LL))
                                  .schedule()
                                  .setPayerAccountId(operatorAccountId)
                                  .setAdminKey(operatorPrivateKey)
                                  .freezeWith(&client)
                                  .sign(key1.get())
                                  .sign(key2.get())
                                  .execute(client)
                                  .getReceipt(client)
                                  .mScheduleId.value();
  std::cout << "Scheduled transaction with schedule ID: " << scheduleId.toString() << std::endl;

  // Query the schedule entity to verify the scheduled transaction hasn't executed and is a transfer.
  if (const ScheduleInfo scheduleInfo = ScheduleInfoQuery().setScheduleId(scheduleId).execute(client);
      scheduleInfo.mExecutionTime.has_value())
  {
    std::cout << "Transaction has executed before providing the last signature!" << std::endl;
    return 1;
  }

  else if (const auto* scheduledTransaction = scheduleInfo.mScheduledTransaction.getTransaction<TransferTransaction>();
           !scheduledTransaction)
  {
    std::cout << "Scheduled transaction is not a transfer!" << std::endl;
    return 1;
  }

  else if (scheduledTransaction->getHbarTransfers().size() != 2)
  {
    std::cout << "Scheduled transaction doesn't contain the correct amount of transfers!" << std::endl;
    return 1;
  }

  // Send the last signature. This should cause the transfer to execute.
  std::cout << "Sending last signature: "
            << gStatusToString.at(ScheduleSignTransaction()
                                    .setScheduleId(scheduleId)
                                    .freezeWith(&client)
                                    .sign(key3.get())
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;

  // Verify the transfer executed.
  if (ScheduleInfoQuery().setScheduleId(scheduleId).execute(client).mExecutionTime.has_value())
  {
    std::cout << "Transfer executed!" << std::endl;
  }
  else
  {
    std::cout << "Transfer didn't execute!" << std::endl;
    return 1;
  }

  // Delete the created account.
  std::cout << "Delete created account: "
            << gStatusToString.at(AccountDeleteTransaction()
                                    .setDeleteAccountId(accountId)
                                    .setTransferAccountId(operatorAccountId)
                                    .freezeWith(&client)
                                    .sign(key1.get())
                                    .sign(key2.get())
                                    .sign(key3.get())
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;
}
