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
#include "EthereumTransaction.h"
#include "AccountId.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "FileId.h"
#include "Hbar.h"
#include "exceptions/IllegalStateException.h"
#include "impl/Utilities.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <proto/ethereum_transaction.pb.h>
#include <proto/transaction_body.pb.h>
#include <vector>

using namespace Hedera;

class EthereumTransactionTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::vector<std::byte>& getTestEthereumData() const { return mTestEthereumData; }
  [[nodiscard]] inline const FileId& getTestCallDataFileId() const { return mTestCallDataFileId; }
  [[nodiscard]] inline const Hbar& getTestMaxGasAllowance() const { return mTestMaxGasAllowance; }

private:
  const std::vector<std::byte> mTestEthereumData = { std::byte(0x01), std::byte(0x02), std::byte(0x03) };
  const FileId mTestCallDataFileId = FileId(4ULL);
  const Hbar mTestMaxGasAllowance = Hbar(5LL);
};

//-----
TEST_F(EthereumTransactionTest, ConstructEthereumTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::EthereumTransactionBody>();
  body->set_ethereum_data(internal::Utilities::byteVectorToString(getTestEthereumData()));
  body->set_allocated_call_data(getTestCallDataFileId().toProtobuf().release());
  body->set_max_gas_allowance(getTestMaxGasAllowance().toTinybars());

  proto::TransactionBody txBody;
  txBody.set_allocated_ethereumtransaction(body.release());

  // When
  const EthereumTransaction ethereumTransaction(txBody);

  // Then
  EXPECT_EQ(ethereumTransaction.getEthereumData(), getTestEthereumData());
  ASSERT_TRUE(ethereumTransaction.getCallDataFileId().has_value());
  EXPECT_EQ(ethereumTransaction.getCallDataFileId(), getTestCallDataFileId());
  EXPECT_EQ(ethereumTransaction.getMaxGasAllowance(), getTestMaxGasAllowance());
}

//-----
TEST_F(EthereumTransactionTest, GetSetEthereumData)
{
  // Given
  EthereumTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setEthereumData(getTestEthereumData()));

  // Then
  EXPECT_EQ(transaction.getEthereumData(), getTestEthereumData());
}

//-----
TEST_F(EthereumTransactionTest, GetSetEthereumDataFrozen)
{
  // Given
  EthereumTransaction transaction = EthereumTransaction()
                                      .setNodeAccountIds({ AccountId(1ULL) })
                                      .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setEthereumData(getTestEthereumData()), IllegalStateException);
}

//-----
TEST_F(EthereumTransactionTest, GetSetCallDataFileId)
{
  // Given
  EthereumTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setCallDataFileId(getTestCallDataFileId()));

  // Then
  EXPECT_EQ(transaction.getCallDataFileId(), getTestCallDataFileId());
}

//-----
TEST_F(EthereumTransactionTest, GetSetCallDataFileIdFrozen)
{
  // Given
  EthereumTransaction transaction = EthereumTransaction()
                                      .setNodeAccountIds({ AccountId(1ULL) })
                                      .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setCallDataFileId(getTestCallDataFileId()), IllegalStateException);
}

//-----
TEST_F(EthereumTransactionTest, GetSetMaxGasAllowance)
{
  // Given
  EthereumTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setMaxGasAllowance(getTestMaxGasAllowance()));

  // Then
  EXPECT_EQ(transaction.getMaxGasAllowance(), getTestMaxGasAllowance());
}

//-----
TEST_F(EthereumTransactionTest, GetSetMaxGasAllowanceFrozen)
{
  // Given
  EthereumTransaction transaction = EthereumTransaction()
                                      .setNodeAccountIds({ AccountId(1ULL) })
                                      .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setMaxGasAllowance(getTestMaxGasAllowance()), IllegalStateException);
}
