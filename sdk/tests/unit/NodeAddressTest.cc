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
#include "NodeAddress.h"
#include "AccountId.h"
#include "Endpoint.h"
#include "IPv4Address.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>
#include <string>
#include <vector>

using namespace Hedera;
using namespace Hedera::internal;

class NodeAddressTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::vector<Endpoint>& getTestEndpoints() const { return mTestEndpoints; }
  [[nodiscard]] inline const std::string& getTestPublicKey() const { return mTestPublicKey; }
  [[nodiscard]] inline int64_t getTestNodeId() const { return mTestNodeId; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestNodeAccountId; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestCertHash() const { return mTestCertHash; }
  [[nodiscard]] inline const std::string& getTestDescription() const { return mTestDescription; }

private:
  const std::vector<Endpoint> mTestEndpoints = {
    Endpoint()
      .setAddress(IPv4Address::fromBytes({ std::byte(0x00), std::byte(0x01), std::byte(0x02), std::byte(0x03) }))
      .setPort(4U)
  };
  const std::string mTestPublicKey = "TestPublicKey";
  const int64_t mTestNodeId = 5LL;
  const AccountId mTestNodeAccountId = AccountId(6ULL, 7ULL, 8ULL);
  const std::vector<std::byte> mTestCertHash = { std::byte(0x09), std::byte(0x0A), std::byte(0x0B) };
  const std::string mTestDescription = "Test Description";
};

//-----
TEST_F(NodeAddressTest, FromProtobuf)
{
  // Given
  proto::NodeAddress protoNodeAddress;

  for (const auto& endpoint : getTestEndpoints())
  {
    *protoNodeAddress.add_serviceendpoint() = *endpoint.toProtobuf();
  }

  protoNodeAddress.set_rsa_pubkey(getTestPublicKey());
  protoNodeAddress.set_nodeid(getTestNodeId());
  protoNodeAddress.set_allocated_nodeaccountid(getTestAccountId().toProtobuf().release());
  protoNodeAddress.set_nodecerthash(Utilities::byteVectorToString(getTestCertHash()));
  protoNodeAddress.set_description(getTestDescription());

  // When
  const NodeAddress nodeAddress = NodeAddress::fromProtobuf(protoNodeAddress);

  // Then
  EXPECT_EQ(nodeAddress.getEndpoints().size(), getTestEndpoints().size());
  EXPECT_EQ(nodeAddress.getPublicKey(), getTestPublicKey());
  EXPECT_EQ(nodeAddress.getNodeId(), getTestNodeId());
  EXPECT_EQ(nodeAddress.getAccountId(), getTestAccountId());
  EXPECT_EQ(nodeAddress.getCertHash(), getTestCertHash());
  EXPECT_EQ(nodeAddress.getDescription(), getTestDescription());
}

//-----
TEST_F(NodeAddressTest, ToProtobuf)
{
  // Given
  NodeAddress nodeAddress;
  nodeAddress.setEndpoints(getTestEndpoints());
  nodeAddress.setPublicKey(getTestPublicKey());
  nodeAddress.setNodeId(getTestNodeId());
  nodeAddress.setAccountId(getTestAccountId());
  nodeAddress.setCertHash(Utilities::byteVectorToString(getTestCertHash()));
  nodeAddress.setDescription(getTestDescription());

  // When
  const std::unique_ptr<proto::NodeAddress> protoNodeAddress = nodeAddress.toProtobuf();

  // Then
  EXPECT_EQ(protoNodeAddress->serviceendpoint_size(), getTestEndpoints().size());
  EXPECT_EQ(protoNodeAddress->rsa_pubkey(), getTestPublicKey());
  EXPECT_EQ(protoNodeAddress->nodeid(), getTestNodeId());
  EXPECT_EQ(protoNodeAddress->nodeaccountid().shardnum(), getTestAccountId().mShardNum);
  EXPECT_EQ(protoNodeAddress->nodeaccountid().realmnum(), getTestAccountId().mRealmNum);
  EXPECT_EQ(protoNodeAddress->nodeaccountid().accountnum(), getTestAccountId().mAccountNum.value());
  EXPECT_EQ(protoNodeAddress->nodecerthash(), Utilities::byteVectorToString(getTestCertHash()));
  EXPECT_EQ(protoNodeAddress->description(), getTestDescription());
}

//-----
TEST_F(NodeAddressTest, GetSetPublicKey)
{
  // Given
  NodeAddress nodeAddress;

  // When
  nodeAddress.setPublicKey(getTestPublicKey());

  // Then
  EXPECT_EQ(nodeAddress.getPublicKey(), getTestPublicKey());
}

//-----
TEST_F(NodeAddressTest, GetSetNodeId)
{
  // Given
  NodeAddress nodeAddress;

  // When
  nodeAddress.setNodeId(getTestNodeId());

  // Then
  EXPECT_EQ(nodeAddress.getNodeId(), getTestNodeId());
}

//-----
TEST_F(NodeAddressTest, GetSetAccountId)
{
  // Given
  NodeAddress nodeAddress;

  // When
  nodeAddress.setAccountId(getTestAccountId());

  // Then
  EXPECT_EQ(nodeAddress.getAccountId(), getTestAccountId());
}

//-----
TEST_F(NodeAddressTest, GetSetCertHash)
{
  // Given
  NodeAddress nodeAddressWithStr;
  NodeAddress nodeAddressWithBytes;

  // When
  nodeAddressWithStr.setCertHash(Utilities::byteVectorToString(getTestCertHash()));
  nodeAddressWithBytes.setCertHash(getTestCertHash());

  // Then
  EXPECT_EQ(nodeAddressWithStr.getCertHash(), getTestCertHash());
  EXPECT_EQ(nodeAddressWithBytes.getCertHash(), getTestCertHash());
}

//-----
TEST_F(NodeAddressTest, GetSetEndpoints)
{
  // Given
  NodeAddress nodeAddress;

  // When
  nodeAddress.setEndpoints(getTestEndpoints());

  // Then
  EXPECT_EQ(nodeAddress.getEndpoints().size(), getTestEndpoints().size());
}

//-----
TEST_F(NodeAddressTest, GetSetDescription)
{
  // Given
  NodeAddress nodeAddress;

  // When
  nodeAddress.setDescription(getTestDescription());

  // Then
  EXPECT_EQ(nodeAddress.getDescription(), getTestDescription());
}
