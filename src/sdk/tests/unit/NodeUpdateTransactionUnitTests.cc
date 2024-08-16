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
#include "ED25519PrivateKey.h"
#include "NodeUpdateTransaction.h"

#include <gtest/gtest.h>

using namespace Hedera;

class NodeUpdateTransactionUnitTests : public ::testing::Test
{
protected:
  NodeUpdateTransaction transaction;
  Endpoint endpoint = Endpoint()
                        .setAddress(IPv4Address::fromBytes({
                          std::byte(0x01),
                          std::byte(0x01),
                          std::byte(0x01),
                          std::byte(0x01),
                        }))
                        .setPort(50211);
};

//-----
TEST_F(NodeUpdateTransactionUnitTests, SetAndGetAccountId)
{
  // Given
  Hedera::AccountId accountId(1234);

  // When
  transaction.setAccountId(accountId);

  // Then
  ASSERT_EQ(transaction.getAccountId(), accountId);
}

//-----
TEST_F(NodeUpdateTransactionUnitTests, SetAndGetDescription)
{
  // Given
  std::string description = "Node Description";

  // When
  transaction.setDescription(description);

  // Then
  ASSERT_TRUE(transaction.getDescription().has_value());
  ASSERT_EQ(transaction.getDescription().value(), description);
}

//-----
TEST_F(NodeUpdateTransactionUnitTests, SetAndGetGossipEndpoints)
{
  // Given
  std::vector<Endpoint> gossipEndpoints = { endpoint };

  // When
  transaction.setGossipEndpoints(gossipEndpoints);

  // Then
  ASSERT_EQ(transaction.getGossipEndpoints().size(), gossipEndpoints.size());
}

//-----
TEST_F(NodeUpdateTransactionUnitTests, SetAndGetServiceEndpoints)
{
  // Given
  std::vector<Endpoint> serviceEndpoints = { endpoint };

  // When
  transaction.setServiceEndpoints(serviceEndpoints);

  // Then
  ASSERT_EQ(transaction.getServiceEndpoints().size(), serviceEndpoints.size());
}

//-----
TEST_F(NodeUpdateTransactionUnitTests, SetAndGetGossipCaCertificate)
{
  // Given
  std::vector<std::byte> caCertificate = { std::byte{ 0x01 }, std::byte{ 0x02 }, std::byte{ 0x03 } };

  // When
  transaction.setGossipCaCertificate(caCertificate);

  // Then
  ASSERT_EQ(transaction.getGossipCaCertificate(), caCertificate);
}

//-----
TEST_F(NodeUpdateTransactionUnitTests, SetAndGetGrpcCertificateHash)
{
  // Given
  std::vector<std::byte> grpcHash = { std::byte{ 0x04 }, std::byte{ 0x05 } };

  // When
  transaction.setGrpcCertificateHash(grpcHash);

  // Then
  ASSERT_TRUE(transaction.getGrpcCertificateHash().has_value());
  ASSERT_EQ(transaction.getGrpcCertificateHash().value(), grpcHash);
}

//-----
TEST_F(NodeUpdateTransactionUnitTests, SetAndGetAdminKey)
{
  // Given
  const std::shared_ptr<ED25519PrivateKey> adminKey = ED25519PrivateKey::generatePrivateKey();

  // When
  transaction.setAdminKey(adminKey);

  // Then
  ASSERT_EQ(transaction.getAdminKey(), adminKey);
}
