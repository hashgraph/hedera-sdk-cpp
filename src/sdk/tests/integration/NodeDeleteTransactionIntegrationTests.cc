/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "FileId.h"
#include "FreezeTransaction.h"
#include "NodeDeleteTransaction.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "impl/HexConverter.h"

#include <gtest/gtest.h>

using namespace Hedera;

class NodeUpdateTransactionIntegrationTests : public BaseIntegrationTest
{
protected:
  [[nodiscard]] const uint64_t& getNodeId() const { return mNodeId; }
  [[nodiscard]] const FileId& getFileId() const { return mFileId; }
  [[nodiscard]] const std::vector<std::byte> getFileHash() const
  {
    return internal::HexConverter::hexToBytes(mFileHash);
  }

private:
  const uint64_t mNodeId = 2;
  const FileId mFileId = FileId::fromString("0.0.150");
  // The file hash needs to be taken from the network context to be correct
  const std::string mFileHash =
    "d9ec902a9fb8dc0f1a43c84b451c59dfe47622d9e5c33965a0ace77003fcad9e0b71478976dbee9dee5d2403f9267b18";
};

//-----
TEST_F(NodeUpdateTransactionIntegrationTests, CanExecuteNodeUpdateTransaction)
{
  // Given / When / Then
  TransactionResponse txResponse;

  ASSERT_NO_THROW(
    txResponse = NodeDeleteTransaction().setNodeId(getNodeId()).freezeWith(&getTestClient()).execute(getTestClient()));

  ASSERT_NO_THROW(txResponse = FreezeTransaction()
                                 .setFreezeType(FreezeType::PREPARE_UPGRADE)
                                 .setFileId(getFileId())
                                 .setFileHash(getFileHash())
                                 .freezeWith(&getTestClient())
                                 .execute(getTestClient()));

  ASSERT_NO_THROW(txResponse = FreezeTransaction()
                                 .setFreezeType(FreezeType::FREEZE_UPGRADE)
                                 .setStartTime(std::chrono::system_clock::now() + std::chrono::seconds(5))
                                 .setFileId(getFileId())
                                 .setFileHash(getFileHash())
                                 .freezeWith(&getTestClient())
                                 .execute(getTestClient()));

  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = txResponse.getReceipt(getTestClient()));
}