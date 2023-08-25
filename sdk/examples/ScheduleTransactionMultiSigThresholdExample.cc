/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "ScheduleInfo.h"
#include "ScheduleInfoQuery.h"
#include "ScheduleSignTransaction.h"
#include "Status.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

#include <array>
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

  // Create an account with a Keylist of four keys.
  std::cout << "Generating key list of four keys with a threshold of three.." << std::endl;
  const std::array<std::shared_ptr<PrivateKey>, 4> keys = { ED25519PrivateKey::generatePrivateKey(),
                                                            ED25519PrivateKey::generatePrivateKey(),
                                                            ED25519PrivateKey::generatePrivateKey(),
                                                            ED25519PrivateKey::generatePrivateKey() };

  // Put the four generated keys in a keylist and set the threshold so that only 3 out of 4 keys need to sign.
  KeyList keyList = KeyList::of({ keys.at(0).get(), keys.at(1).get(), keys.at(2).get(), keys.at(3).get() });
  keyList.setThreshold(3);

  // Create an account with the four keys.
  std::cout << "Creating account with generated keys: ";
  TransactionReceipt txReceipt =
    AccountCreateTransaction().setKey(&keyList).setInitialBalance(Hbar(10LL)).execute(client).getReceipt(client);
  std::cout << gStatusToString.at(txReceipt.mStatus) << std::endl;

  // Schedule a transfer out of the created account with only one signature.
  std::cout << "Scheduling transaction with one signature.." << std::endl;
  const AccountId accountId = txReceipt.mAccountId.value();
  const ScheduleId scheduleId = TransferTransaction()
                                  .addHbarTransfer(accountId, Hbar(-1LL))
                                  .addHbarTransfer(operatorAccountId, Hbar(1LL))
                                  .schedule()
                                  .setPayerAccountId(operatorAccountId)
                                  .setAdminKey(operatorPrivateKey)
                                  .freezeWith(&client)
                                  .sign(keys.at(0).get())
                                  .execute(client)
                                  .getReceipt(client)
                                  .mScheduleId.value();
  std::cout << "Scheduled transaction with schedule ID: " << scheduleId.toString() << std::endl;

  // Keep adding signatures until the transaction executes.
  for (int i = 1; i < keys.size(); ++i)
  {
    // Determine if the transaction has executed.
    if (const ScheduleInfo scheduleInfo = ScheduleInfoQuery().setScheduleId(scheduleId).execute(client);
        scheduleInfo.mExecutionTime.has_value())
    {
      std::cout << "Transaction has executed with (should be 3) " << i << " signatures!" << std::endl;
      break;
    }

    // Add a signature if the transfer hasn't yet executed.
    std::cout << "Adding signature.. "
              << gStatusToString.at(ScheduleSignTransaction()
                                      .setScheduleId(scheduleId)
                                      .freezeWith(&client)
                                      .sign(keys.at(i).get())
                                      .execute(client)
                                      .getReceipt(client)
                                      .mStatus)
              << std::endl;
  }

  // Delete the created account.
  std::cout << "Delete created account: "
            << gStatusToString.at(AccountDeleteTransaction()
                                    .setDeleteAccountId(accountId)
                                    .setTransferAccountId(operatorAccountId)
                                    .freezeWith(&client)
                                    .sign(keys.at(0).get())
                                    .sign(keys.at(1).get())
                                    .sign(keys.at(2).get())
                                    .sign(keys.at(3).get())
                                    .execute(client)
                                    .getReceipt(client)
                                    .mStatus)
            << std::endl;
}
