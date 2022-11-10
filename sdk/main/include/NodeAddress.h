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
#ifndef NODE_ADDRESS_H_
#define NODE_ADDRESS_H_

#include <string>
#include <vector>

#include "AccountId.h"
#include "Endpoint.h"

namespace proto
{
class NodeAddress;
}

namespace Hedera
{

/**
 * Class containing all information related the the address(es) of a node
 */
class NodeAddress
{
public:
  inline static int PORT_MIRROR_PLAIN = 5600;
  inline static int PORT_MIRROR_TLS = 443;
  inline static int PORT_NODE_PLAIN = 50211;
  inline static int PORT_NODE_TLS = 50212;

  /**
   * Constructor
   */
  NodeAddress();

  /**
   * Gets the account ID associated with the node
   *
   * @return the account ID associated with the node
   */
  [[nodiscard]] std::shared_ptr<AccountId> getAccountId() const;

  /**
   * Gets a vector of endpoints associated with the node
   *
   * @return the node endpoints
   */
  [[nodiscard]] const std::vector<Endpoint>& getEndpoints() const;

  /**
   * Gets the SHA-384 hash of the node certificate chain
   *
   * @return the certificate chain hash
   */
  [[nodiscard]] std::string getCertificateHash() const;

  /**
   * Creates a new node address object from a protobuf
   *
   * @param protoNodeAddress the protobuf address
   *
   * @return the new node address object
   */
  static NodeAddress fromProtobuf(const proto::NodeAddress& protoNodeAddress);

  /**
   * Gets the string representation of the address, for debugging purposes
   *
   * @return a string representing the address
   */
  [[nodiscard]] std::string toString() const;

private:
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
  int64_t mNodeId;

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
  std::shared_ptr<AccountId> mAccountId;
};
} // namespace Hedera

#endif // NODE_ADDRESS_H_