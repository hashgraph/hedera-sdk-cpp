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
Network Network::forNetwork(const std::map<std::string, Hedera::AccountId>& networkMap)
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
void Network::setNetwork(const std::map<std::string, Hedera::AccountId>& networkMap)
{
  std::cout << "In Network::setNetwork" << std::endl;

  std::unordered_map<AccountId, std::shared_ptr<internal::NodeAddress>> addressMap;
  std::map<std::string, Hedera::AccountId>::const_iterator it;

  std::cout << "Before for loop." << std::endl;

  for (it = networkMap.begin(); it != networkMap.end(); it++)
  {
    const std::string& nodeAddressAsString = it->first;
    const AccountId& accountId = it->second;

    std::cout << "nodeAddressAsString: " << nodeAddressAsString << std::endl;
    std::cout << "accountId: " << accountId.toString() << std::endl;

    internal::NodeAddress nodeAddress = internal::NodeAddress::fromString(nodeAddressAsString);
    nodeAddress.setNodeCertHash("Test Node Certificate Hash");
    nodeAddress.setNodeAccountId(accountId);
    std::cout << "nodeAddress: " << nodeAddress.toString() << std::endl;

    std::shared_ptr<internal::NodeAddress> nodeAddressPtr = std::make_shared<internal::NodeAddress>(nodeAddress);
    // std::cout << std::endl << "nodeAddressPtr: " << nodeAddressPtr->toString() << std::endl;

    addressMap.insert(std::pair<Hedera::AccountId, std::shared_ptr<internal::NodeAddress>>(accountId, nodeAddressPtr));
    // std::cout << std::endl << "addressMap.size(): " << addressMap.size() << std::endl;

    std::cout << std::endl << std::endl;
  }

  // std::cout << "BEFORE Trace Trap" << std::endl;

  NodeAddressBook nodeAddressBook;
  nodeAddressBook.setAddressMap(addressMap);
  setNetwork(nodeAddressBook);
}

//-----
void Network::setNetwork(const NodeAddressBook& nodeAddressBook)
{
  for (const auto& [accountId, nodeAddress] : nodeAddressBook.getAddressMap())
  {
    mNodes.push_back(std::make_shared<Node>(nodeAddress));
  }
}

} // namespace Hedera::internal
