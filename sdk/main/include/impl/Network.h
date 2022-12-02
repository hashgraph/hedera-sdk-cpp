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
#ifndef HEDERA_SDK_CPP_IMPL_NETWORK_H_
#define HEDERA_SDK_CPP_IMPL_NETWORK_H_

#include "AccountId.h"
#include "Node.h"
#include "NodeAddressBook.h"
#include "TLSBehavior.h"

#include <memory>
#include <string>
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
   * Create a testnet network.
   *
   * @return The new testnet network.
   */
  static Network forTestnet();

  /**
   * Get a list of nodes on this Client's network that are associated with the input account IDs. If no account IDs are
   * specified, returns all nodes
   *
   * @param accountIds The account IDs of the requested nodes.
   *
   * @return List of nodes with the requested account IDs.
   */
  [[nodiscard]] std::vector<std::shared_ptr<Node>> getNodesWithAccountIds(
    const std::vector<std::shared_ptr<AccountId>>& accountIds) const;

  /**
   * Close this network.
   */
  void close() const;

  /**
   * Sets the TLS behavior of all nodes.
   *
   * @param desiredBehavior The desired behavior.
   */
  void setTLSBehavior(TLSBehavior desiredBehavior) const;

private:
  /**
   * Set the network configuration for this network.
   *
   * @param nodeAddressBook The address book the network will be communicating with
   */
  void setNetwork(const NodeAddressBook& nodeAddressBook);

  /**
   * List of nodes with which this network is communicating.
   */
  std::vector<std::shared_ptr<Node>> mNodes;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_NETWORK_H_
