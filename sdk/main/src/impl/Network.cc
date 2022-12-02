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
#include "impl/Network.h"

#include "AccountId.h"

namespace Hedera::internal
{

//-----
Network Network::forTestnet()
{
  Network network;
  network.setNetwork(NodeAddressBook::fromFile("testnet.pb"));

  return network;
}

//-----
std::vector<std::shared_ptr<Node>> Network::getNodesWithAccountIds(
  const std::vector<std::shared_ptr<AccountId>>& accountIds) const
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
void Network::setNetwork(const NodeAddressBook& nodeAddressBook)
{
  for (const auto& [accountId, nodeAddress] : nodeAddressBook.getAddressMap())
  {
    mNodes.push_back(std::make_shared<Node>(nodeAddress));
  }
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

} // namespace Hedera::internal
