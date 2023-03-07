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

using namespace Hedera::internal;

class NodeAddressTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const int getTestPort() const { return mTestPort; }
  [[nodiscard]] inline const std::string& getTestIpAddress() const { return mTestIpAddress; }

private:
  const int mTestPort = 50211;
  const std::string mTestIpAddress = "35.237.200.180:50211";
};

//-----
TEST_F(NodeAddressTest, DefaultConstructNodeAddress)
{
  // Given
  const int testPort = getTestPort();
  const std::string testIpAddress = getTestIpAddress();

  // When
  const NodeAddress nodeAddress = NodeAddress::fromString(testIpAddress);

  // Then
  EXPECT_FALSE(nodeAddress.isTlsPort(testPort));
  EXPECT_TRUE(nodeAddress.isNonTlsPort(testPort));
  EXPECT_EQ(nodeAddress.getNodeId(), -1);
  EXPECT_EQ(nodeAddress.getNodeAccountId().getShardNum(), 0ULL);
  EXPECT_EQ(nodeAddress.getNodeAccountId().getRealmNum(), 0ULL);
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getAccountNum().has_value());
  EXPECT_EQ(nodeAddress.getNodeAccountId().getAlias(), nullptr);
  EXPECT_FALSE(nodeAddress.getNodeAccountId().getEvmAddress().has_value());
  EXPECT_TRUE(nodeAddress.getNodeCertHash().empty());
  EXPECT_TRUE(nodeAddress.getDescription().empty());
  EXPECT_TRUE(nodeAddress.getEndpoints().empty());
}

//-----
TEST_F(NodeAddressTest, ConstructFromString)
{
  // Given
  const std::string testIpAddress = getTestIpAddress();

  // When
  NodeAddress nodeAddress = NodeAddress::fromString(testIpAddress);

  // Then
  EXPECT_EQ(nodeAddress.getAddress().toString(), "35.237.200.180");
  EXPECT_EQ(nodeAddress.getAddress().toString(), "35.237.200.180");
}
