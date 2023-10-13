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
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "FileAppendTransaction.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileId.h"
#include "FileInfo.h"
#include "FileInfoQuery.h"
#include "Status.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "impl/Utilities.h"

#include <iostream>
#include <vector>

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
                          .setKeys({ client.getOperatorPublicKey() })
                          .setContents(internal::Utilities::stringToByteVector("Hedera Hashgraph is great!"))
                          .execute(client)
                          .getReceipt(client)
                          .mFileId.value();
  std::cout << "Created new file with ID " << fileId.toString() << std::endl;

  // Create the contents to append.
  const std::vector<std::byte> toAppend(4096 * 9, std::byte(0x01));

  // Append the contents to the file.
  const std::vector<TransactionResponse> txResponses = FileAppendTransaction()
                                                         .setFileId(fileId)
                                                         .setContents(toAppend)
                                                         .setMaxChunks(40)
                                                         .setMaxTransactionFee(Hbar(1000))
                                                         .executeAll(client);

  // Since FileAppendTransactions get the receipts between chunks, there's no need to check here that all chunks were
  // completely processed.

  // Check the new file size.
  std::cout << "The new file size is " << FileInfoQuery().setFileId(fileId).execute(client).mSize << std::endl;

  // Delete the file.
  const TransactionReceipt txReceipt = FileDeleteTransaction().setFileId(fileId).execute(client).getReceipt(client);
  std::cout << "Deleted file with response code: " << gStatusToString.at(txReceipt.mStatus) << std::endl;

  return 0;
}
