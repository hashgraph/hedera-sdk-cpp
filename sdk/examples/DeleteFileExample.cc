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
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileId.h"
#include "PublicKey.h"
#include "Status.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "impl/Utilities.h"

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
  Client client = Client::forTestnet();
  client.setOperator(AccountId::fromString(argv[1]), ED25519PrivateKey::fromString(argv[2]));

  // Create a new file.
  const FileId fileId = FileCreateTransaction()
                          .setKey(client.getOperatorPublicKey())
                          .setContents(internal::Utilities::stringToByteVector("Hedera Hashgraph is great!"))
                          .execute(client)
                          .getReceipt(client)
                          .getFileId()
                          .value();
  std::cout << "Created new file with ID " << fileId.toString() << std::endl;

  // Delete the newly-created file.
  const TransactionReceipt txReceipt = FileDeleteTransaction().setFileId(fileId).execute(client).getReceipt(client);
  std::cout << "Deleted file with response code: " << gStatusToString.at(txReceipt.getStatus()) << std::endl;

  return 0;
}
