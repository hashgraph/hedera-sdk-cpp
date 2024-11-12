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
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "FileContentsQuery.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileId.h"
#include "FileInfo.h"
#include "FileInfoQuery.h"
#include "FileUpdateTransaction.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"

#include <dotenv.h>
#include <iostream>
#include <string>

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

  // The file is required to be a byte array, you can easily use the bytes of a file instead.
  std::string fileStringContents = "Hiero hashgraph is great!";

  const FileId fileId = FileCreateTransaction()
                          .setKeys({ client.getOperatorPublicKey() })
                          .setContents(fileStringContents)
                          .execute(client)
                          .getReceipt(client)
                          .mFileId.value();

  std::cout << "Created new file with ID " << fileId.toString() << std::endl;

  // Query the newly-created file.
  FileInfo fileInfo = FileInfoQuery().setFileId(fileId).execute(client);
  std::cout << "File Info: " << fileInfo.toString() << std::endl;

  // Query the newly-created file contents.
  FileContents fileContents = FileContentsQuery().setFileId(fileId).execute(client);
  std::cout << "Contents of file in bytes: " << std::endl;
  for (std::byte byte : fileContents)
  {
    std::cout << std::to_integer<int>(byte) << " ";
  }
  std::cout << std::endl;

  // Update file contents
  std::string fileStringContentsUpdated = "Hiero hashgraph is great! Updated contents.";
  std::cout << "Updating file contents! " << std::endl;
  TransactionReceipt txReceipt = FileUpdateTransaction()
                                   .setFileId(fileId)
                                   .setKeys({ client.getOperatorPublicKey() })
                                   .setContents(fileStringContentsUpdated)
                                   .freezeWith(&client)
                                   .sign(operatorPrivateKey)
                                   .execute(client)
                                   .getReceipt(client);

  FileContents updatedFileContents = FileContentsQuery().setFileId(fileId).execute(client);
  std::cout << "Updated contents of file: " << std::endl;
  for (std::byte byte : updatedFileContents)
  {
    std::cout << std::to_integer<int>(byte) << " ";
  }
  std::cout << std::endl;

  // Delete the newly-created file.
  txReceipt = FileDeleteTransaction().setFileId(fileId).execute(client).getReceipt(client);
  std::cout << "Deleted file with response code: " << gStatusToString.at(txReceipt.mStatus) << std::endl;

  return 0;
}
