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
#ifndef NETWORK_H_
#define NETWORK_H_

#include "Node.h"

#include "helper/StringHash.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace Hedera
{
class AccountId;
}

namespace Hedera
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
   * Get a list of nodes on this network that are associated with the input account IDs.
   *
   * @param accountIds The account IDs of the requested nodes.
   * @return List of nodes with the requested account IDs.
   */
  std::vector<std::shared_ptr<Node>> getNodesWithAccountIds(const std::vector<AccountId>& accountIds) const;

  /**
   * Close this network.
   */
  void close();

private:
  /**
   * Set the network configuration for this network.
   *
   * @param network The URLs and the account IDs of the associated nodes with
   *                which this network will be communicating.
   */
  void setNetwork(const std::unordered_map<std::string, AccountId, StringHash, std::equal_to<>>& network);

  /**
   * List of nodes with which this network is communicating.
   */
  std::vector<std::shared_ptr<Node>> mNodes;
};

} // namespace Hedera

#endif // NETWORK_H_