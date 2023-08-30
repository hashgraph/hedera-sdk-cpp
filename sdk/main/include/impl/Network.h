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
#ifndef HEDERA_SDK_CPP_IMPL_NETWORK_H_
#define HEDERA_SDK_CPP_IMPL_NETWORK_H_

#include "AccountId.h"
#include "Node.h"
#include "NodeAddressBook.h"
#include "TLSBehavior.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Hedera
{
class AccountId;
}

namespace Hedera::internal
{
class Network
{
public:
  /**
   * Construct a Network that is pre-configured for Hedera Mainnet access.
   *
   * @return A Network object that is set-up to communicate with the Hedera Mainnet.
   */
  [[nodiscard]] static Network forMainnet();

  /**
   * Construct a Network that is pre-configured for Hedera Testnet access.
   *
   * @return A Network object that is set-up to communicate with the Hedera Testnet.
   */
  [[nodiscard]] static Network forTestnet();

  /**
   * Construct a Network that is pre-configured for Hedera Previewnet access.
   *
   * @return A Network object that is set-up to communicate with the Hedera Previewnet.
   */
  [[nodiscard]] static Network forPreviewnet();

  /**
   * Construct a Network that is pre-configured for local node access.
   *
   * @param networkMap The map with string representation of node addresses with their corresponding accountId.
   * @return A Network object that is set-up to communicate with a specific node addresses.
   */
  [[nodiscard]] static Network forNetwork(const std::unordered_map<std::string, AccountId>& networkMap);

  /**
   * Get a list of Node pointers that point to Nodes on this Network that are associated with the input account IDs. If
   * no account IDs are specified, pointers to all Nodes on this Network are returned.
   *
   * @param accountIds The IDs of the accounts that are associated with the desired Nodes.
   * @return A list of Node pointers that point to Nodes with the requested account IDs.
   */
  [[nodiscard]] std::vector<std::shared_ptr<Node>> getNodesWithAccountIds(
    const std::vector<AccountId>& accountIds) const;

  /**
   * Initiate an orderly shutdown of communications with the Nodes that are a part of this Network. Preexisting
   * transactions or queries continue but subsequent calls would be immediately cancelled.
   *
   * After this method returns, this Network can be re-used. All network communication can be re-established as needed.
   */
  void close() const;

  /**
   * Set the TLS behavior of all Nodes on this Network.
   *
   * @param desiredBehavior The desired behavior.
   * @throws UninitializedException If TLS is required and a Node on this Network wasn't initialized with a certificate
   *                                hash.
   */
  void setTLSBehavior(TLSBehavior desiredBehavior) const;

private:
  /**
   * Establish communications with all Nodes for this Network that are specified in the input address book.
   *
   * @param nodeAddressBook The address book that contains the Nodes with which this Network will be communicating.
   * @param tls The TLS behavior all nodes should initially use. Default value is REQUIRE.
   * @throws UninitializedException If a NodeAddress in the input NodeAddressBook wasn't initialized with a certificate
   *                                hash.
   */
  void setNetwork(const NodeAddressBook& nodeAddressBook, TLSBehavior tls = TLSBehavior::REQUIRE);

  /**
   * Establish communications with all node addresses for this Network that are specified in the input map collection.
   *
   * @param networkMap The map containing string node addresses pointing to specific accountId object.
   * @param tls The TLS behavior all nodes should initially use. Default value is DISABLE.
   * @throws UninitializedException If a NodeAddress in the input NodeAddressBook wasn't initialized with a certificate
   *         hash.
   */
  void setNetwork(const std::unordered_map<std::string, AccountId>& networkMap, TLSBehavior tls = TLSBehavior::DISABLE);

  /**
   * The list of pointers to Nodes with which this Network is communicating.
   */
  std::vector<std::shared_ptr<Node>> mNodes;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_NETWORK_H_
