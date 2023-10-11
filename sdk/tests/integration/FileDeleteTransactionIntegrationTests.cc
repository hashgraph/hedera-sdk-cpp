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
#include "PrivateKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/ReceiptStatusException.h"

#include <gtest/gtest.h>

using namespace Hedera;

class FileDeleteTransactionIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(FileDeleteTransactionIntegrationTest, ExecuteFileDeleteTransaction)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ operatorKey->getPublicKey() })
                             .setContents({})
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .mFileId.value());

  // When
  EXPECT_NO_THROW(const TransactionReceipt txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));

  // Then
  FileInfo fileInfo;
  ASSERT_NO_THROW(fileInfo = FileInfoQuery().setFileId(fileId).execute(getTestClient()));
  EXPECT_TRUE(fileInfo.mIsDeleted);
}

//-----
TEST_F(FileDeleteTransactionIntegrationTest, CannotDeleteFileWithNoAdminKey)
{
  // Given
  FileId fileId;
  ASSERT_NO_THROW(
    fileId =
      FileCreateTransaction().setContents({}).execute(getTestClient()).getReceipt(getTestClient()).mFileId.value());

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt =
                 FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()),
               ReceiptStatusException); // UNAUTHORIZED
}

//-----
TEST_F(FileDeleteTransactionIntegrationTest, CannotDeleteFileWithoutFileId)
{
  // Given / When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt =
                 FileDeleteTransaction().execute(getTestClient()).getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_FILE_ID
}
