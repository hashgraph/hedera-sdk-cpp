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
#include "AccountId.h"
#include "impl/Network.h"

#include <gtest/gtest.h>
#include <string>
#include <unordered_map>
#include <vector>

using namespace Hiero;

class NetworkUnitTests : public ::testing::Test
{
};

//-----
TEST_F(NetworkUnitTests, ConstructForMainnet)
{
  // Given / When
  Hiero::internal::Network mainnetNetwork = Hiero::internal::Network::forMainnet();

  // Then
  std::unordered_map<std::string, AccountId> networkMap;
  std::vector<AccountId> nodeAccountIds;

  EXPECT_NO_THROW(networkMap = mainnetNetwork.getNetwork());
  EXPECT_NO_THROW(nodeAccountIds = mainnetNetwork.getNodeAccountIdsForExecute());

  EXPECT_GT(networkMap.size(), 0);
  EXPECT_GT(nodeAccountIds.size(), 0);

  // Clean up
  mainnetNetwork.close();
}

TEST_F(NetworkUnitTests, ConstructForTestnet)
{
  // Given / When
  Hiero::internal::Network testnetNetwork = Hiero::internal::Network::forTestnet();

  // Then
  std::unordered_map<std::string, AccountId> networkMap;
  std::vector<AccountId> nodeAccountIds;

  EXPECT_NO_THROW(networkMap = testnetNetwork.getNetwork());
  EXPECT_NO_THROW(nodeAccountIds = testnetNetwork.getNodeAccountIdsForExecute());

  EXPECT_GT(networkMap.size(), 0);
  EXPECT_GT(nodeAccountIds.size(), 0);

  // Clean up
  testnetNetwork.close();
}

TEST_F(NetworkUnitTests, ConstructForPreviewnet)
{
  // Given / When
  Hiero::internal::Network previewnetNetwork = Hiero::internal::Network::forPreviewnet();

  // Then
  std::unordered_map<std::string, AccountId> networkMap;
  std::vector<AccountId> nodeAccountIds;

  EXPECT_NO_THROW(networkMap = previewnetNetwork.getNetwork());
  EXPECT_NO_THROW(nodeAccountIds = previewnetNetwork.getNodeAccountIdsForExecute());

  EXPECT_GT(networkMap.size(), 0);
  EXPECT_GT(nodeAccountIds.size(), 0);

  // Clean up
  previewnetNetwork.close();
}

TEST_F(NetworkUnitTests, ConstructCustomNetwork)
{
  // Given
  const std::unordered_map<std::string, AccountId> testNetwork = {
    { "2.testnet.hedera.com:50211", AccountId(5ULL) },
    { "3.testnet.hedera.com:50211", AccountId(6ULL) }
  };

  // When
  Hiero::internal::Network customNetwork = Hiero::internal::Network::forNetwork(testNetwork);

  // Then
  std::unordered_map<std::string, AccountId> networkMap;
  std::vector<AccountId> nodeAccountIds;

  EXPECT_NO_THROW(networkMap = customNetwork.getNetwork());
  EXPECT_NO_THROW(nodeAccountIds = customNetwork.getNodeAccountIdsForExecute());

  EXPECT_GT(networkMap.size(), 0);
  EXPECT_GT(nodeAccountIds.size(), 0);

  // Clean up
  customNetwork.close();
}

TEST_F(NetworkUnitTests, GetSetLedgerIdForMainnet)
{
  // Given
  Hiero::internal::Network mainnetNetwork = Hiero::internal::Network::forMainnet();

  // When
  mainnetNetwork.setLedgerId(LedgerId::TESTNET);

  // Then
  EXPECT_EQ(mainnetNetwork.getLedgerId(), LedgerId::TESTNET);

  // Clean up
  mainnetNetwork.close();
}

TEST_F(NetworkUnitTests, GetSetTransportSecurity)
{
  // Given
  Hiero::internal::Network testnetNetwork = Hiero::internal::Network::forTestnet();

  // When
  testnetNetwork.setTransportSecurity(Hiero::internal::TLSBehavior::DISABLE);

  // Then
  EXPECT_EQ(testnetNetwork.isTransportSecurity(), Hiero::internal::TLSBehavior::DISABLE);

  // Clean up
  testnetNetwork.close();
}

TEST_F(NetworkUnitTests, GetSetGetMaxNodesPerRequest)
{
  // Given
  Hiero::internal::Network testnetNetwork = Hiero::internal::Network::forTestnet();

  // When
  testnetNetwork.setMaxNodesPerRequest(10);

  // Then
  EXPECT_EQ(testnetNetwork.getNumberOfNodesForRequest(), 10);

  // Clean up
  testnetNetwork.close();
}

TEST_F(NetworkUnitTests, VerifyCertificates)
{
  // Given
  Hiero::internal::Network testnetNetwork = Hiero::internal::Network::forTestnet();

  // When
  testnetNetwork.setVerifyCertificates(false);

  // Then
  EXPECT_EQ(testnetNetwork.isVerifyCertificates(), false);

  // Clean up
  testnetNetwork.close();
}