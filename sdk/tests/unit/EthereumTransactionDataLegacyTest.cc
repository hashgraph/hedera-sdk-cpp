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
#include "EthereumTransactionDataLegacy.h"
#include "impl/HexConverter.h"

#include <gtest/gtest.h>

using namespace Hedera;

class EthereumTransactionDataLegacyTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::string& getTestTransaction() const { return mTestTransaction; }

private:
  const std::string mTestTransaction =
    "f864012f83018000947e3a9eaf9bcc39e2ffa38eb30bf7a93feacbc18180827653820277a0f9fbff985d374be4a55f296915002eec11ac96f1"
    "ce2df183adf992baa9390b2fa00c1e867cc960d9c74ec2e6a662b7908ec4c8cc9f3091e886bcefbeb2290fb792";
};

//-----
TEST_F(EthereumTransactionDataLegacyTest, FromBytes)
{
  // Given / When
  const EthereumTransactionDataLegacy ethereumTransactionDataLegacy =
    EthereumTransactionDataLegacy::fromBytes(internal::HexConverter::hexToBytes(getTestTransaction()));

  // Then
  EXPECT_EQ(ethereumTransactionDataLegacy.mNonce, internal::HexConverter::hexToBytes("01"));
  EXPECT_EQ(ethereumTransactionDataLegacy.mGasPrice, internal::HexConverter::hexToBytes("2f"));
  EXPECT_EQ(ethereumTransactionDataLegacy.mGasLimit, internal::HexConverter::hexToBytes("018000"));
  EXPECT_EQ(ethereumTransactionDataLegacy.mTo,
            internal::HexConverter::hexToBytes("7e3a9eaf9bcc39e2ffa38eb30bf7a93feacbc181"));
  EXPECT_EQ(ethereumTransactionDataLegacy.mValue, std::vector<std::byte>());
  EXPECT_EQ(ethereumTransactionDataLegacy.mCallData, internal::HexConverter::hexToBytes("7653"));
  EXPECT_EQ(ethereumTransactionDataLegacy.mV, internal::HexConverter::hexToBytes("0277"));
  EXPECT_EQ(ethereumTransactionDataLegacy.mR,
            internal::HexConverter::hexToBytes("f9fbff985d374be4a55f296915002eec11ac96f1ce2df183adf992baa9390b2f"));
  EXPECT_EQ(ethereumTransactionDataLegacy.mS,
            internal::HexConverter::hexToBytes("0c1e867cc960d9c74ec2e6a662b7908ec4c8cc9f3091e886bcefbeb2290fb792"));
}

//-----
TEST_F(EthereumTransactionDataLegacyTest, ToBytes)
{
  // Given
  const std::vector<std::byte> transactionBytes = internal::HexConverter::hexToBytes(getTestTransaction());

  // When
  const std::vector<std::byte> dataBytes = EthereumTransactionDataLegacy::fromBytes(transactionBytes).toBytes();

  // Then
  EXPECT_EQ(dataBytes, transactionBytes);
}

//-----
TEST_F(EthereumTransactionDataLegacyTest, ToString)
{
  // Given / When
  const EthereumTransactionDataLegacy ethereumTransactionDataLegacy =
    EthereumTransactionDataLegacy::fromBytes(internal::HexConverter::hexToBytes(getTestTransaction()));

  // Then
  EXPECT_EQ(ethereumTransactionDataLegacy.toString(),
            "mNonce: 01\n"
            "mGasPrice: 2F\n"
            "mGasLimit: 018000\n"
            "mTo: 7E3A9EAF9BCC39E2FFA38EB30BF7A93FEACBC181\n"
            "mValue: \n"
            "mCallData: 7653\n"
            "mV: 0277\n"
            "mR: F9FBFF985D374BE4A55F296915002EEC11AC96F1CE2DF183ADF992BAA9390B2F\n"
            "mS: 0C1E867CC960D9C74EC2E6A662B7908EC4C8CC9F3091E886BCEFBEB2290FB792");
}
