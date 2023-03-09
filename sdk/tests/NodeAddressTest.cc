/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#include "impl/IPv4Address.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>
#include <string>

using namespace Hedera::internal;

class NodeAddressTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const int& getTestNodeId() const { return mTestNodeId; }
  [[nodiscard]] inline const int& getTestPortTLS() const { return mTestPortTLS; }
  [[nodiscard]] inline const int& getTestPortPlain() const { return mTestPortPlain; }
  [[nodiscard]] inline const std::string& getTestIpAddress() const { return mTestIpAddress; }
  [[nodiscard]] const std::string getTestNodeAddress() const
  {
    const int testPortTLS = getTestPortTLS();
    std::stringstream outputStream;
    outputStream << getTestIpAddress() << ":" << std::to_string(testPortTLS);
    return outputStream.str();
  }

private:
  const int64_t mTestNodeId = 9;
  const int mTestPortTLS = 50212;
  const int mTestPortPlain = 50211;
  const std::string mTestIpAddress = "35.237.200.180";
};

// Test creation of NodeAddress instance using the default constructor.
TEST_F(NodeAddressTest, DefaultConstructNodeAddress)
{
  // Given
  const int testPortTLS = getTestPortTLS();
  const std::string& testIpAddress = getTestIpAddress();
  const std::string testNodeAddress = getTestNodeAddress();

  // When
  const NodeAddress nodeAddress = NodeAddress::fromString(testNodeAddress);

  // Then
  EXPECT_TRUE(nodeAddress.isTlsPort(testPortTLS));
  EXPECT_FALSE(nodeAddress.isNonTlsPort(testPortTLS));
  EXPECT_EQ(nodeAddress.getNodeId(), -1);
  EXPECT_EQ(nodeAddress.getNodeAccountId().getShardNum(), 0ULL);
  EXPECT_EQ(nodeAddress.getNodeAccountId().getRealmNum(), 0ULL);
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getAccountNum().has_value());
  EXPECT_EQ(nodeAddress.getNodeAccountId().getAlias(), nullptr);
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getEvmAddress().has_value());
  EXPECT_TRUE(nodeAddress.getNodeCertHash().empty());
  EXPECT_TRUE(nodeAddress.getDescription().empty());
  EXPECT_FALSE(nodeAddress.getEndpoints().empty());
}

// Test creation of NodeAddress instance using a protobuf object.
TEST_F(NodeAddressTest, ConstructFromProtobuf)
{
  // Given
  const int testPortPlain = getTestPortPlain();
  const int64_t testNodeId = getTestNodeId();
  const std::string& testIpAddressV4 = getTestIpAddress();
  const std::string testDescription = "Test Description";
  proto::NodeAddress testProtoNodeAddress = proto::NodeAddress();
  testProtoNodeAddress.set_nodeid(testNodeId);
  testProtoNodeAddress.set_description(testDescription);
  proto::ServiceEndpoint* serviceEndpoint = testProtoNodeAddress.add_serviceendpoint();
  serviceEndpoint->set_ipaddressv4(testIpAddressV4);
  serviceEndpoint->set_port(testPortPlain);

  // When
  const NodeAddress nodeAddress = NodeAddress::fromProtobuf(testProtoNodeAddress);

  // Then
  EXPECT_EQ(nodeAddress.getDefaultIpAddress().toString(), testIpAddressV4);
  EXPECT_EQ(nodeAddress.getDefaultPort(), getTestPortTLS());
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getEvmAddress().has_value());
  EXPECT_TRUE(nodeAddress.getNodeCertHash().empty());
  EXPECT_FALSE(nodeAddress.getDescription().empty());
  EXPECT_EQ(nodeAddress.getDescription(), testDescription);
  EXPECT_FALSE(nodeAddress.getEndpoints().empty());
}

// Test creation of NodeAddress instance using a given string node address.
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
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getEvmAddress().has_value());
  EXPECT_TRUE(nodeAddress.getNodeCertHash().empty());
  EXPECT_TRUE(nodeAddress.getDescription().empty());
  EXPECT_FALSE(nodeAddress.getEndpoints().empty());
}
