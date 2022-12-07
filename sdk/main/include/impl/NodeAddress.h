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
  /**
   * Creates a new node address object from a protobuf
   *
   * @param protoNodeAddress the protobuf address
   *
   * @return the new node address object
   */
  static NodeAddress fromProtobuf(const proto::NodeAddress& protoNodeAddress);

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
   * Gets the string representation of the address, for debugging purposes
   *
   * @return a string representing the address
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Gets the account ID associated with the node
   *
   * @return the account ID associated with the node
   */
  [[nodiscard]] inline AccountId getAccountId() const { return mAccountId; }

  /**
   * Gets a vector of endpoints associated with the node
   *
   * @return the node endpoints
   */
  [[nodiscard]] inline const std::vector<Endpoint>& getEndpoints() const { return mEndpoints; }

  /**
   * Gets the SHA-384 hash of the node certificate chain
   *
   * @return the certificate chain hash
   */
  [[nodiscard]] inline std::string getCertificateHash() const { return mCertificateHash; }

private:
  /**
   * Port numbers for various node types and security.
   */
  static constexpr int PORT_MIRROR_PLAIN = 5600;
  static constexpr int PORT_MIRROR_TLS = 443;
  static constexpr int PORT_NODE_PLAIN = 50211;
  static constexpr int PORT_NODE_TLS = 50212;

  /**
   * The endpoints associated with the node
   */
  std::vector<Endpoint> mEndpoints;

  /**
   * The node's public key
   */
  std::string mRSAPublicKey;

  /**
   * The ID of the node
   */
  int64_t mNodeId = -1;

  /**
   * The SHA-384 hash of the node's certificate chain
   */
  std::string mCertificateHash;

  /**
   * A string description of the node
   */
  std::string mDescription;

  /**
   * The account ID associated with the node
   */
  AccountId mAccountId;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_NODE_ADDRESS_H_
