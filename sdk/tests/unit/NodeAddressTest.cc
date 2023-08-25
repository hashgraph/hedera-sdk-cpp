/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include "impl/NodeAddress.h"
#include "exceptions/IllegalStateException.h"
#include "impl/Endpoint.h"
#include "impl/HexConverter.h"
#include "impl/IPv4Address.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>
#include <string>
#include <vector>

using namespace Hedera::internal;

class NodeAddressTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const int64_t& getTestNodeId() const { return mTestNodeId; }
  [[nodiscard]] inline const int& getTestPortTLS() const { return mTestPortTLS; }
  [[nodiscard]] inline const int& getTestPortPlain() const { return mTestPortPlain; }
  [[nodiscard]] inline const std::string& getTestRSAPublicKey() const { return mTestRSAPublicKey; }
  [[nodiscard]] inline const std::string& getTestIpAddress() const { return mTestIpAddress; }
  [[nodiscard]] inline const std::string& getTestDescription() const { return mTestDescription; }
  [[nodiscard]] inline const std::string& getTestNodeCertHash() const { return mTestNodeCertHash; }
  [[nodiscard]] inline const std::string& getTestNodeAddress() const { return mTestNodeAddress; }

  void SetUp() override
  {
    const int testPortTLS = getTestPortTLS();
    std::stringstream outputStream;
    outputStream << getTestIpAddress() << ":" << std::to_string(testPortTLS);
    mTestNodeAddress = outputStream.str();
  }

private:
  const int64_t mTestNodeId = 9;
  const int mTestPortTLS = 50212;
  const int mTestPortPlain = 50211;
  const std::string mTestRSAPublicKey = "TestPublicKey";
  const std::string mTestIpAddress = "35.237.200.180";
  const std::string mTestDescription = "Test Description";
  const std::string mTestNodeCertHash = "Certificate Hash Value";
  std::string mTestNodeAddress;
};

// Test creation of NodeAddress instance using the default constructor.
TEST_F(NodeAddressTest, DefaultConstructNodeAddress)
{
  // Given
  const int testPortTLS = getTestPortTLS();

  // When
  const NodeAddress nodeAddress;

  // Then
  EXPECT_TRUE(nodeAddress.isTlsPort(testPortTLS));
  EXPECT_FALSE(nodeAddress.isNonTlsPort(testPortTLS));
  EXPECT_EQ(nodeAddress.getNodeId(), -1);
  EXPECT_EQ(nodeAddress.getNodeAccountId().getShardNum(), 0ULL);
  EXPECT_EQ(nodeAddress.getNodeAccountId().getRealmNum(), 0ULL);
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getAccountNum().has_value());
  EXPECT_EQ(nodeAddress.getNodeAccountId().getPublicKeyAlias(), nullptr);
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getEvmAddressAlias().has_value());
  EXPECT_TRUE(nodeAddress.getNodeCertHash().empty());
  EXPECT_TRUE(nodeAddress.getDescription().empty());
  EXPECT_TRUE(nodeAddress.getEndpoints().empty());
}

// Test setters & getters of NodeAddress.
TEST_F(NodeAddressTest, GettersAndSettersNodeAddress)
{
  // Given
  NodeAddress testNodeAddress;
  const int64_t testNodeId = getTestNodeId();
  const int testPortTLS = getTestPortTLS();
  const std::string& testRSAPublicKey = getTestRSAPublicKey();
  const std::string& testStringForIpAddressV4_1 = getTestIpAddress();
  const std::string& testStringForIpAddressV4_2 = "127.0.0.1";
  const IPv4Address& testIpAddressV4_1 = IPv4Address::fromString(testStringForIpAddressV4_1);
  const IPv4Address& testIpAddressV4_2 = IPv4Address::fromString(testStringForIpAddressV4_2);
  const std::string& testDescription = getTestDescription();
  const std::string& testNodeCertHash = getTestNodeCertHash();
  const std::vector<std::byte> nodeCertHashVec = Utilities::stringToByteVector(testNodeCertHash);
  const auto testEndpointPtr_1 = std::make_shared<Endpoint>(testIpAddressV4_1, testPortTLS);
  const auto testEndpointPtr_2 = std::make_shared<Endpoint>(testIpAddressV4_2, testPortTLS);
  std::vector<std::shared_ptr<Endpoint>> testEndpoints;
  testEndpoints.push_back(testEndpointPtr_1);
  testEndpoints.push_back(testEndpointPtr_2);
  const std::vector<std::shared_ptr<Endpoint>> testConstEndpoints(testEndpoints);

  // When
  testNodeAddress.setNodeId(testNodeId);
  testNodeAddress.setRSAPublicKey(testRSAPublicKey);
  testNodeAddress.setDescription(testDescription);
  testNodeAddress.setNodeCertHash(testNodeCertHash);
  testNodeAddress.setEndpoints(testConstEndpoints);

  // Then
  EXPECT_EQ(testNodeAddress.getNodeId(), testNodeId);
  EXPECT_EQ(testNodeAddress.getPublicKey(), testRSAPublicKey);
  EXPECT_EQ(testNodeAddress.getDescription(), testDescription);
  EXPECT_EQ(testNodeAddress.getNodeCertHash(), nodeCertHashVec);
  EXPECT_EQ(testNodeAddress.getDefaultIpAddress().toString(), testStringForIpAddressV4_1);
  EXPECT_EQ(testNodeAddress.getDefaultPort(), testPortTLS);
}

// Test creation of NodeAddress instance using a protobuf object.
TEST_F(NodeAddressTest, ConstructFromProtobuf)
{
  // Given
  const int testPortPlain = getTestPortPlain();
  const int64_t testNodeId = getTestNodeId();
  const std::string& testRSAPublicKey = getTestRSAPublicKey();
  const std::string& testIpAddressV4 = getTestIpAddress();
  const std::string& testDescription = getTestDescription();
  auto testProtoNodeAddress = proto::NodeAddress();
  testProtoNodeAddress.set_nodeid(testNodeId);
  testProtoNodeAddress.set_description(testDescription);
  testProtoNodeAddress.set_rsa_pubkey(testRSAPublicKey);
  proto::ServiceEndpoint* serviceEndpoint = testProtoNodeAddress.add_serviceendpoint();
  serviceEndpoint->set_ipaddressv4(testIpAddressV4);
  serviceEndpoint->set_port(testPortPlain);

  // When
  const NodeAddress nodeAddress = NodeAddress::fromProtobuf(testProtoNodeAddress);

  // Then
  EXPECT_EQ(nodeAddress.getDefaultIpAddress().toString(), testIpAddressV4);
  EXPECT_EQ(nodeAddress.getDefaultPort(), getTestPortTLS());
  EXPECT_EQ(nodeAddress.getNodeId(), testNodeId);
  EXPECT_EQ(nodeAddress.getPublicKey(), testRSAPublicKey);
  EXPECT_EQ(nodeAddress.getNodeAccountId().getShardNum(), 0ULL);
  EXPECT_EQ(nodeAddress.getNodeAccountId().getRealmNum(), 0ULL);
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getAccountNum().has_value());
  EXPECT_EQ(nodeAddress.getNodeAccountId().getPublicKeyAlias(), nullptr);
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getEvmAddressAlias().has_value());
  EXPECT_TRUE(nodeAddress.getNodeCertHash().empty());
  EXPECT_FALSE(nodeAddress.getDescription().empty());
  EXPECT_EQ(nodeAddress.getDescription(), testDescription);
  EXPECT_FALSE(nodeAddress.getEndpoints().empty());
}

// Test creation of NodeAddress instance using a node address represented as a string.
TEST_F(NodeAddressTest, ConstructFromString)
{
  // Given
  const int testPort = getTestPortTLS();
  const std::string& testIpAddressV4 = getTestIpAddress();
  const std::string testNodeAddress = getTestNodeAddress();

  // When
  const NodeAddress nodeAddress = NodeAddress::fromString(testNodeAddress);

  // Then
  EXPECT_EQ(nodeAddress.getDefaultIpAddress().toString(), testIpAddressV4);
  EXPECT_EQ(nodeAddress.getDefaultPort(), testPort);
  EXPECT_EQ(nodeAddress.getNodeId(), -1);
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getEvmAddressAlias().has_value());
  EXPECT_EQ(nodeAddress.getNodeAccountId().getShardNum(), 0ULL);
  EXPECT_EQ(nodeAddress.getNodeAccountId().getRealmNum(), 0ULL);
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getAccountNum().has_value());
  EXPECT_EQ(nodeAddress.getNodeAccountId().getPublicKeyAlias(), nullptr);
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getEvmAddressAlias().has_value());
  EXPECT_TRUE(nodeAddress.getNodeCertHash().empty());
  EXPECT_TRUE(nodeAddress.getDescription().empty());
  EXPECT_FALSE(nodeAddress.getEndpoints().empty());
}

// Test throwing an exception when trying to create a NodeAddress from malformed string.
TEST_F(NodeAddressTest, ConstructFromMalformedStringAndThrowException)
{
  // Given
  const std::string testNodeAddress_1 = "1";
  const std::string testNodeAddress_2 = "aaa.bbb.ccc.ddd";

  // When & Then
  EXPECT_THROW(auto nodeAddress = NodeAddress::fromString(testNodeAddress_1), std::invalid_argument);
  EXPECT_THROW(auto nodeAddress = NodeAddress::fromString(testNodeAddress_2), std::invalid_argument);
}
