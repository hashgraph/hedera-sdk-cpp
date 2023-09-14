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

#include "BaseNetwork.h"
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
class Network : public BaseNetwork<Network, AccountId, Node>
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
   * Construct a custom Network.
   *
   * @param network The map with string representation of node addresses with their corresponding AccountIds.
   * @return A Network object that is set-up to communicate with the custom network.
   */
  [[nodiscard]] static Network forNetwork(const std::unordered_map<std::string, AccountId>& network);

  /**
   * Derived from BaseNetwork. Set the ledger ID of this Network.
   *
   * @param ledgerId The ledger ID to set.
   * @return A reference to this Network object with the newly-set ledger ID.
   */
  Network& setLedgerId(const LedgerId& ledgerId) override;

  /**
   * Set this Network's certificate verification policy.
   *
   * @param verify \c TRUE if this Network should verify remote node certificates, otherwise \c FALSE.
   * @return A reference to this Network object with the newly-set certificate verification policy.
   */
  Network& setVerifyCertificates(bool verify);

  /**
   * Set this Network's transport security policy.
   *
   * @param tls The transport security policy to set.
   * @return A reference to this Network object with the newly-set transport security policy.
   * @throws UninitializedException If TLS is required and a Node on this Network wasn't initialized with a certificate
   *                                hash.
   */
  Network& setTransportSecurity(TLSBehavior tls);

  /**
   * Get a list of node account IDs on which to execute. This will pick 1/3 of the available nodes sorted by health and
   * expected delay from the network.
   *
   * @return A list of AccountIds that are running nodes on which should be executed.
   */
  [[nodiscard]] std::vector<AccountId> getNodeAccountIdsForExecute();

  /**
   * Get a map of this Network, mapping the Node addresses to their AccountIds.
   *
   * @return A map of this Network, mapping the Node addresses to their AccountIds.
   */
  [[nodiscard]] std::unordered_map<std::string, AccountId> getNetwork() const;

private:
  /**
   * Construct with a network map.
   *
   * @param network A map of node addresses to the AccountIds that run those nodes.
   */
  explicit Network(const std::unordered_map<std::string, AccountId>& network);

  /**
   * Construct a Network object configured to communicate with the network represented by the input LedgerId.
   *
   * @param ledgerId The LedgerId of the Network object to construct.
   * @return The constructed Network object.
   */
  [[nodiscard]] static Network getNetworkForLedgerId(const LedgerId& ledgerId);

  /**
   * Get a map of AccountIds to NodeAddresses from a local file for a network based on the input LedgerId.
   *
   * @param ledgerId The LedgerId of the network of which to get the address book.
   * @return The map of node addresses and AccountIds of the Nodes that exist on the network represented by the input
   *         LedgerId.
   */
  [[nodiscard]] static std::unordered_map<AccountId, NodeAddress> getAddressBookForLedgerId(const LedgerId& ledgerId);

  /**
   * Derived from BaseNetwork. Create a Node for this Network based on a network entry.
   *
   * @param address The address of the Node.
   * @param key     The key for the Node.
   * @return A pointer to the created Node.
   */
  [[nodiscard]] std::shared_ptr<Node> createNodeFromNetworkEntry(std::string_view address,
                                                                 const AccountId& key) const override;

  /**
   * Set the ledger ID of this Network. In addition, update the Nodes on this Network with their address book entry
   * contained in the input map.
   *
   * @param ledgerId    The new LedgerId of the Network.
   * @param addressBook The address book with which to update this Network's Node's address book entry.
   */
  Network& setLedgerIdInternal(const LedgerId& ledgerId, const std::unordered_map<AccountId, NodeAddress>& addressBook);

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
   * The maximum number of nodes to be returned for each request.
   */
  unsigned int mMaxNodesPerRequest = 0U;

  /**
   * Should the Nodes on this Network verify remote node certificates?
   */
  bool mVerifyCertificates = true;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_NETWORK_H_
