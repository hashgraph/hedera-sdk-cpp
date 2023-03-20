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
#ifndef HEDERA_SDK_CPP_IMPL_NODE_ADDRESS_H_
#define HEDERA_SDK_CPP_IMPL_NODE_ADDRESS_H_

#include "AccountId.h"
#include "impl/Endpoint.h"

#include <string>
#include <vector>

namespace proto
{
class NodeAddress;
}

namespace Hedera::internal
{
/**
 * Class containing all information related to the address(es) of a node.
 */
class NodeAddress
{
public:
  NodeAddress() = default;

  /**
   * Construct a NodeAddress instance with a given IPv4 address and port.
   *
   * @param ipAddressV4 The stringified IPv4 address.
   * @param port        The port number of the server for the node.
   * @throws std::invalid_argument If the given IP address is malformed.
   */
  NodeAddress(std::string_view ipAddressV4, int port);

  /**
   * Determine if a particular port number corresponds to a TLS port.
   *
   * @param port The port number.
   * @return \c TRUE if the input port number corresponds to a TLS port, otherwise \c FALSE.
   */
  static inline bool isTlsPort(int port) { return (port == PORT_NODE_TLS) || (port == PORT_MIRROR_TLS); }

  /**
   * Determine if a particular port number corresponds to a non-TLS port.
   *
   * @param port The port number.
   * @return \c TRUE if the input port number corresponds to a non-TLS port, otherwise \c FALSE.
   */
  static inline bool isNonTlsPort(int port) { return (port == PORT_NODE_PLAIN) || (port == PORT_MIRROR_PLAIN); }

  /**
   * Create a NodeAddress object from a NodeAddress protobuf object.
   *
   * @param protoNodeAddress The NodeAddress protobuf object from which to create a NodeAddress object.
   * @return The created NodeAddress object.
   */
  [[nodiscard]] static NodeAddress fromProtobuf(const proto::NodeAddress& protoNodeAddress);

  /**
   * Create a NodeAddress object from a given string.
   *
   * @param nodeAddress The string representation from which to create a new NodeAddress object.
   * @return The created NodeAddress object.
   * @throws std::invalid_argument If the given node address is malformed.
   */
  [[nodiscard]] static NodeAddress fromString(std::string_view nodeAddress);

  /**
   * Set a new public key for the node.
   *
   * @param publicKey The public key to be assigned to the node.
   * @return A reference to this NodeAddress with the newly-set public key.
   */
  NodeAddress& setRSAPublicKey(std::string_view publicKey);

  /**
   * Set a new node ID for the node at this address.
   *
   * @param nodeId The ID of the node to set.
   * @return A reference to this NodeAddress with the newly-set node ID.
   */
  NodeAddress& setNodeId(const int64_t& nodeId);

  /**
   * Set a new account ID associated with the node at this address.
   *
   * @param accountId The account ID to be associated with the node.
   * @return A reference to this NodeAddress with the newly-set account ID.
   */
  NodeAddress& setNodeAccountId(const AccountId& accountId);

  /**
   * Set a new certificate hash for the node.
   *
   * @param certHash The certificate hash to be assigned to the node.
   * @return A reference to this NodeAddress with the newly-set certificate hash.
   */
  NodeAddress& setNodeCertHash(std::string_view certHash);

  /**
   * Set a vector of endpoints for the node.
   *
   * @param endpoints The endpoints to be assigned to the node.
   * @return A reference to this NodeAddress with the newly-set endpoints.
   */
  NodeAddress& setEndpoints(const std::vector<std::shared_ptr<Endpoint>>& endpoints);

  /**
   * Set а new description text for the node.
   *
   * @param description The description text to be assigned with the node.
   * @return A reference to this NodeAddress with the newly-set description.
   */
  NodeAddress& setDescription(std::string_view description);

  /**
   * Set a new amount of tinybars staked to the node.
   *
   * @param stake The new amount of tinybars staked to the node.
   * @return A reference to this NodeAddress with the newly-set staked tinybars.
   */
  NodeAddress& setStake(const uint64_t& stake);

  /**
   * Get a string representation of the NodeAddress.
   *
   * @return A string representing this NodeAddress.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Get the default IP address of this NodeAddress.
   *
   * @return The default IP address of this NodeAddress.
   */
  [[nodiscard]] inline IPv4Address getDefaultIpAddress() const { return getDefaultEndpoint()->getAddress(); }

  /**
   * Get the default port number of the gRPC server of this NodeAddress.
   *
   * @return The default port of this NodeAddress.
   */
  [[nodiscard]] inline int getDefaultPort() const { return getDefaultEndpoint()->getPort(); }

  /**
   * Get the node ID of this NodeAddress.
   *
   * @return The node ID of this NodeAddress.
   */
  [[nodiscard]] inline int64_t getNodeId() const { return mNodeId; }

  /**
   * Get the public key of this NodeAddress.
   *
   * @return The hash value representing the public key of the node.
   */
  [[nodiscard]] inline std::string getPublicKey() const { return mRSAPublicKey; }

  /**
   * Get the account ID associated with this NodeAddress.
   *
   * @return The account ID associated with this NodeAddress.
   */
  [[nodiscard]] inline AccountId getNodeAccountId() const { return mNodeAccountId; }

  /**
   * Get the SHA-384 hash of this NodeAddress's certificate chain.
   *
   * @return The SHA-384 hash of this NodeAddress's certificate chain.
   */
  [[nodiscard]] inline std::vector<std::byte> getNodeCertHash() const { return mNodeCertHash; }

  /**
   * Get the default Endpoint of this NodeAddress.
   *
   * @return The default Endpoint of this NodeAddress.
   */
  [[nodiscard]] inline std::shared_ptr<Endpoint> getDefaultEndpoint() const
  {
    if (mEndpoints.empty())
    {
      return nullptr;
    }

    return mEndpoints.front();
  }

  /**
   * Get the Endpoints associated with this NodeAddress.
   *
   * @return The Endpoints associated with this NodeAddress.
   */
  [[nodiscard]] inline const std::vector<std::shared_ptr<Endpoint>>& getEndpoints() const { return mEndpoints; }

  /**
   * Get the description text of this NodeAddress.
   *
   * @return The description text of this NodeAddress.
   */
  [[nodiscard]] inline std::string getDescription() const { return mDescription; }

  /**
   * Get the amount of tinybars staked to the Node at this NodeAddress.
   *
   * @return The amount of tinybars staked to the Node at this NodeAddress.
   */
  [[nodiscard]] inline uint64_t getStake() const { return mStake; }

private:
  /**
   * Port numbers for various node types and security.
   */
  static constexpr int PORT_MIRROR_PLAIN = 5600;
  static constexpr int PORT_MIRROR_TLS = 443;
  static constexpr int PORT_NODE_PLAIN = 50211;
  static constexpr int PORT_NODE_TLS = 50212;

  /**
   * The Endpoints associated with the node.
   */
  std::vector<std::shared_ptr<Endpoint>> mEndpoints;

  /**
   * The node's public key.
   */
  std::string mRSAPublicKey;

  /**
   * The ID of the node.
   */
  int64_t mNodeId = -1;

  /**
   * The account ID associated with the node.
   */
  AccountId mNodeAccountId;

  /**
   * The SHA-384 hash of the node's certificate chain.
   */
  std::vector<std::byte> mNodeCertHash;

  /**
   * A string description of the node.
   */
  std::string mDescription;

  /**
   * The amount of tinybars staked to the node.
   */
  uint64_t mStake = 0;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_NODE_ADDRESS_H_
