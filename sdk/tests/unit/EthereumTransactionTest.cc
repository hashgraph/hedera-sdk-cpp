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
  void SetUp() override { mClient.setOperator(AccountId(), ECDSAsecp256k1PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestEthereumData() const { return mTestEthereumData; }
  [[nodiscard]] inline const FileId& getTestCallDataFileId() const { return mTestCallDataFileId; }
  [[nodiscard]] inline const Hbar& getTestMaxGasAllowance() const { return mTestMaxGasAllowance; }

private:
  Client mClient;
  const std::vector<std::byte> mTestEthereumData = { std::byte(0x01), std::byte(0x02), std::byte(0x03) };
  const FileId mTestCallDataFileId = FileId(4ULL);
  const Hbar mTestMaxGasAllowance = Hbar(5LL);
};

//-----
TEST_F(EthereumTransactionTest, ConstructEthereumTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto bodyWithEthereumData = std::make_unique<proto::EthereumTransactionBody>();
  auto bodyWithCallDataFileId = std::make_unique<proto::EthereumTransactionBody>();

  bodyWithEthereumData->set_max_gas_allowance(getTestMaxGasAllowance().toTinybars());
  bodyWithCallDataFileId->set_max_gas_allowance(getTestMaxGasAllowance().toTinybars());

  bodyWithEthereumData->set_ethereum_data(internal::Utilities::byteVectorToString(getTestEthereumData()));
  bodyWithCallDataFileId->set_allocated_call_data(getTestCallDataFileId().toProtobuf().release());

  proto::TransactionBody txBodyEthereumData;
  proto::TransactionBody txBodyCallDataFileId;

  txBodyEthereumData.set_allocated_ethereumtransaction(bodyWithEthereumData.release());
  txBodyCallDataFileId.set_allocated_ethereumtransaction(bodyWithCallDataFileId.release());

  // When
  const EthereumTransaction ethereumTransactionEthereumData(txBodyEthereumData);
  const EthereumTransaction ethereumTransactionCallDataFileId(txBodyCallDataFileId);

  // Then
  ASSERT_TRUE(ethereumTransactionEthereumData.getEthereumData().has_value());
  EXPECT_EQ(ethereumTransactionEthereumData.getEthereumData(), getTestEthereumData());
  EXPECT_FALSE(ethereumTransactionEthereumData.getCallDataFileId().has_value());
  EXPECT_EQ(ethereumTransactionEthereumData.getMaxGasAllowance(), getTestMaxGasAllowance());

  EXPECT_FALSE(ethereumTransactionCallDataFileId.getEthereumData().has_value());
  ASSERT_TRUE(ethereumTransactionCallDataFileId.getCallDataFileId().has_value());
  EXPECT_EQ(ethereumTransactionCallDataFileId.getCallDataFileId(), getTestCallDataFileId());
  EXPECT_EQ(ethereumTransactionCallDataFileId.getMaxGasAllowance(), getTestMaxGasAllowance());
}

//-----
TEST_F(EthereumTransactionTest, GetSetEthereumData)
{
  // Given
  EthereumTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setEthereumData(getTestEthereumData()));

  // Then
  ASSERT_TRUE(transaction.getEthereumData().has_value());
  EXPECT_EQ(transaction.getEthereumData(), getTestEthereumData());
}

//-----
TEST_F(EthereumTransactionTest, GetSetEthereumDataFrozen)
{
  // Given
  EthereumTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setEthereumData(getTestEthereumData()), IllegalStateException);
  EXPECT_FALSE(transaction.getEthereumData().has_value());
}

//-----
TEST_F(EthereumTransactionTest, GetSetCallDataFileId)
{
  // Given
  EthereumTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setCallDataFileId(getTestCallDataFileId()));

  // Then
  ASSERT_TRUE(transaction.getCallDataFileId().has_value());
  EXPECT_EQ(transaction.getCallDataFileId(), getTestCallDataFileId());
}

//-----
TEST_F(EthereumTransactionTest, GetSetCallDataFileIdFrozen)
{
  // Given
  EthereumTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setCallDataFileId(getTestCallDataFileId()), IllegalStateException);
  EXPECT_FALSE(transaction.getCallDataFileId().has_value());
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
  EthereumTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setMaxGasAllowance(getTestMaxGasAllowance()), IllegalStateException);
}

//-----
TEST_F(EthereumTransactionTest, ResetEthereumData)
{
  // Given
  EthereumTransaction transaction;
  ASSERT_NO_THROW(transaction.setEthereumData(getTestEthereumData()));

  // When
  EXPECT_NO_THROW(transaction.setCallDataFileId(getTestCallDataFileId()));

  // Then
  EXPECT_FALSE(transaction.getEthereumData().has_value());
  EXPECT_TRUE(transaction.getCallDataFileId().has_value());
}

//-----
TEST_F(EthereumTransactionTest, ResetCallDataFileId)
{
  // Given
  EthereumTransaction transaction;
  ASSERT_NO_THROW(transaction.setCallDataFileId(getTestCallDataFileId()));

  // When
  EXPECT_NO_THROW(transaction.setEthereumData(getTestEthereumData()));

  // Then
  EXPECT_TRUE(transaction.getEthereumData().has_value());
  EXPECT_FALSE(transaction.getCallDataFileId().has_value());
}
