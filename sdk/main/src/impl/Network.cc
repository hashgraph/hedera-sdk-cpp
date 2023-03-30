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
#include "impl/Network.h"

#include "AccountId.h"

namespace Hedera::internal
{
//-----
Network Network::forMainnet()
{
  Network network;
  network.setNetwork(NodeAddressBook::fromFile("mainnet.pb"));
  return network;
}

//-----
Network Network::forTestnet()
{
  Network network;
  network.setNetwork(NodeAddressBook::fromFile("testnet.pb"));
  return network;
}

//-----
Network Network::forPreviewnet()
{
  Network network;
  network.setNetwork(NodeAddressBook::fromFile("previewnet.pb"));
  return network;
}

//-----
Network Network::forNetwork(const std::unordered_map<std::string, AccountId>& networkMap)
{
  Network network;
  network.setNetwork(networkMap);
  return network;
}

//-----
std::vector<std::shared_ptr<Node>> Network::getNodesWithAccountIds(const std::vector<AccountId>& accountIds) const
{
  if (accountIds.empty())
  {
    return mNodes;
  }

  std::vector<std::shared_ptr<Node>> nodesWithCorrectAccountIds;
  for (const auto& node : mNodes)
  {
    for (const auto& id : accountIds)
    {
      if (node->getAccountId() == id)
      {
        nodesWithCorrectAccountIds.push_back(node);
      }
    }
  }

  return nodesWithCorrectAccountIds;
}

//-----
void Network::close() const
{
  for (const auto& node : mNodes)
  {
    node->shutdown();
  }
}

void Network::setTLSBehavior(TLSBehavior desiredBehavior) const
{
  for (const auto& node : mNodes)
  {
    node->setTLSBehavior(desiredBehavior);
  }
}

//-----
void Network::setNetwork(const NodeAddressBook& nodeAddressBook, TLSBehavior tls)
{
  for (const auto& [accountId, nodeAddress] : nodeAddressBook.getAddressMap())
  {
    mNodes.push_back(std::make_shared<Node>(nodeAddress, tls));
  }
}

//-----
void Network::setNetwork(const std::unordered_map<std::string, AccountId>& networkMap, TLSBehavior tls)
{
  std::unordered_map<AccountId, std::shared_ptr<NodeAddress>> addressMap;
  std::unordered_map<std::string, Hedera::AccountId>::const_iterator it;

  for (it = networkMap.begin(); it != networkMap.end(); it++)
  {
    const std::string& nodeAddressAsString = it->first;
    const AccountId& accountId = it->second;

    NodeAddress nodeAddress = NodeAddress::fromString(nodeAddressAsString);
    std::shared_ptr<NodeAddress> nodeAddressPtr = std::make_shared<NodeAddress>(nodeAddress);
    nodeAddressPtr->setNodeAccountId(accountId);
    addressMap.insert(std::pair<Hedera::AccountId, std::shared_ptr<NodeAddress>>(accountId, nodeAddressPtr));
  }

  setNetwork(NodeAddressBook::fromAddressMap(addressMap), tls);
}

} // namespace Hedera::internal
