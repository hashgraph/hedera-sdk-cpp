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
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileInfo.h"
#include "FileInfoQuery.h"
#include "FileUpdateTransaction.h"
#include "PrivateKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/ReceiptStatusException.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>

using namespace Hedera;

class FileUpdateTransactionIntegrationTests : public BaseIntegrationTest
{
};

//-----
TEST_F(FileUpdateTransactionIntegrationTests, ExecuteFileUpdateTransaction)
{
  // Given
  const std::string memo = "file create memo";
  const std::string newMemo = "update file memo";

  std::unique_ptr<PrivateKey> operatorKey;
  std::shared_ptr<PrivateKey> newKey;
  std::vector<std::byte> contents;
  std::vector<std::byte> newContents;
  FileId fileId;

  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));
  ASSERT_NO_THROW(
    newKey = ED25519PrivateKey::fromString(
      "302e020100300506032b6570042204209d61b19deffd5a60ba844af492ec2cc44449c5697b326919703bac031cae7f60"));
  ASSERT_NO_THROW(contents = internal::Utilities::stringToByteVector("[e2e::FileCreateTransaction]"));
  ASSERT_NO_THROW(newContents = internal::Utilities::stringToByteVector("[e2e::FileUpdateTransaction]"));
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ operatorKey->getPublicKey() })
                             .setContents(contents)
                             .setFileMemo(memo)
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .mFileId.value());

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = FileUpdateTransaction()
                                .setFileId(fileId)
                                .setKeys({ newKey->getPublicKey() })
                                .setContents(newContents)
                                .setFileMemo(newMemo)
                                .freezeWith(&getTestClient())
                                .sign(newKey)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  FileInfo fileInfo;
  ASSERT_NO_THROW(fileInfo = FileInfoQuery().setFileId(fileId).execute(getTestClient()));

  EXPECT_EQ(fileInfo.mSize, contents.size());
  EXPECT_FALSE(fileInfo.mIsDeleted);
  EXPECT_EQ(fileInfo.mAdminKeys.toBytes(), KeyList::of({ newKey->getPublicKey() }).toBytes());
  EXPECT_EQ(fileInfo.mMemo, newMemo);

  // Clean up
  ASSERT_NO_THROW(txReceipt = FileDeleteTransaction()
                                .setFileId(fileId)
                                .freezeWith(&getTestClient())
                                .sign(newKey)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
}

//-----
TEST_F(FileUpdateTransactionIntegrationTests, CannotUpdateImmutableFile)
{
  // Given
  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setContents(internal::Utilities::stringToByteVector("[e2e::FileCreateTransaction]"))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .mFileId.value());

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = FileUpdateTransaction()
                                                      .setFileId(fileId)
                                                      .setContents("[e2e::FileUpdateTransaction]")
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // UNAUTHORIZED
}

//-----
TEST_F(FileUpdateTransactionIntegrationTests, CannotUpdateWithNoFileId)
{
  // Given / When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = FileUpdateTransaction()
                                                      .setContents("[e2e::FileUpdateTransaction]")
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_FILE_ID
}
