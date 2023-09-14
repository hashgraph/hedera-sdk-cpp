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
#include "impl/BaseNetwork.h"
#include "AccountId.h"
#include "impl/BaseNode.h"
#include "impl/BaseNodeAddress.h"
#include "impl/MirrorNetwork.h"
#include "impl/MirrorNode.h"
#include "impl/Network.h"
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <thread>

namespace Hedera::internal
{
//-----
template<typename NetworkType, typename KeyType, typename NodeType>
NetworkType& BaseNetwork<NetworkType, KeyType, NodeType>::setNetwork(
  const std::unordered_map<std::string, KeyType>& network)
{
  // New containers to hold new network.
  std::unordered_map<KeyType, std::unordered_set<std::shared_ptr<NodeType>>> newNetwork;
  std::unordered_set<std::shared_ptr<NodeType>> newNodes;

  // Go through each entry in the new network list and either move the NodeType that already exists for that entry to
  // the newNodes list, or create a new NodeType for that entry.
  for (const auto& [address, key] : network)
  {
    // Determine if this entry already has a NodeType.
    bool entryAlreadyExists = false;
    for (const auto& node : mNodes)
    {
      if (node->getAddress().toString() == address && node->getKey() == key)
      {
        // Move this node to the newNodes set and go to the next entry.
        mNodes.erase(node);
        newNodes.insert(node);
        newNetwork[key].insert(node);
        entryAlreadyExists = true;
        break;
      }
    }

    // If the entry was found, go to the next entry.
    if (entryAlreadyExists)
    {
      continue;
    }

    // If an entry doesn't exist, create one and add it to the nodes lists and network.
    const std::shared_ptr<NodeType> newNode = createNodeFromNetworkEntry(address, key);
    newNodes.insert(newNode);
    newNetwork[key].insert(newNode);
  }

  // The remaining nodes in mNodes need to be closed and removed.
  for (const std::shared_ptr<NodeType>& node : mNodes)
  {
    node->close();
    removeNodeFromNetwork(node);
  }

  // Set the new nodes list and network.
  mNodes = newNodes;
  mNetwork = newNetwork;
  mHealthyNodes.clear();

  return static_cast<NetworkType&>(*this);
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
void BaseNetwork<NetworkType, KeyType, NodeType>::increaseBackoff(const std::shared_ptr<NodeType>& node)
{
  node->increaseBackoff();
  mHealthyNodes.erase(node);
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
void BaseNetwork<NetworkType, KeyType, NodeType>::decreaseBackoff(const std::shared_ptr<NodeType>& node)
{
  node->decreaseBackoff();
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
std::vector<std::shared_ptr<NodeType>> BaseNetwork<NetworkType, KeyType, NodeType>::getNodeProxies(const KeyType& key)
{
  readmitNodes();
  return { mNetwork[key].cbegin(), mNetwork[key].cend() };
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
void BaseNetwork<NetworkType, KeyType, NodeType>::close()
{
  for (const std::shared_ptr<NodeType>& node : mNodes)
  {
    node->close();
  }
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
NetworkType& BaseNetwork<NetworkType, KeyType, NodeType>::setMaxNodeAttempts(unsigned int attempts)
{
  mMaxNodeAttempts = attempts;
  return static_cast<NetworkType&>(*this);
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
NetworkType& BaseNetwork<NetworkType, KeyType, NodeType>::setMinNodeBackoff(
  const std::chrono::duration<double>& backoff)
{
  mMinNodeBackoff = backoff;
  return static_cast<NetworkType&>(*this);
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
NetworkType& BaseNetwork<NetworkType, KeyType, NodeType>::setMaxNodeBackoff(
  const std::chrono::duration<double>& backoff)
{
  mMaxNodeBackoff = backoff;
  return static_cast<NetworkType&>(*this);
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
NetworkType& BaseNetwork<NetworkType, KeyType, NodeType>::setMinNodeReadmitTime(
  const std::chrono::duration<double>& time)
{
  mMinNodeReadmitTime = time;
  return static_cast<NetworkType&>(*this);
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
NetworkType& BaseNetwork<NetworkType, KeyType, NodeType>::setMaxNodeReadmitTime(
  const std::chrono::duration<double>& time)
{
  mMaxNodeReadmitTime = time;
  return static_cast<NetworkType&>(*this);
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
NetworkType& BaseNetwork<NetworkType, KeyType, NodeType>::setCloseTimeout(const std::chrono::duration<double>& timeout)
{
  mCloseTimeout = timeout;
  return static_cast<NetworkType&>(*this);
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
NetworkType& BaseNetwork<NetworkType, KeyType, NodeType>::setLedgerId(const LedgerId& ledgerId)
{
  mLedgerId = ledgerId;
  return static_cast<NetworkType&>(*this);
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
std::vector<std::shared_ptr<NodeType>> BaseNetwork<NetworkType, KeyType, NodeType>::getNumberOfMostHealthyNodes(
  unsigned int count)
{
  readmitNodes();

  // First, remove any nodes from the network that have exceeded the maximum number of node attempts.
  if (mMaxNodeAttempts > 0U)
  {
    auto nodeIter = mNodes.begin();
    while (nodeIter != mNodes.end())
    {
      if ((*nodeIter)->getBadGrpcStatusCount() >= mMaxNodeAttempts)
      {
        (*nodeIter)->close();
        removeNodeFromNetwork(*nodeIter);
      }
      else
      {
        ++nodeIter;
      }
    }
  }

  std::unordered_set<std::shared_ptr<NodeType>> nodes;
  while (count > nodes.size())
  {
    // Readmit nodes each time one is fetched. Since readmitting only happens periodically, calling this each time
    // should not have a significant impact on performance.
    readmitNodes();

    // If there are no healthy nodes, wait until one can be readmitted.
    if (mHealthyNodes.empty())
    {
      std::this_thread::sleep_for(mEarliestReadmitTime - std::chrono::system_clock::now());
      continue;
    }

    // Add a random node (it won't be added if its repeated).
    auto iter = mHealthyNodes.cbegin();
    std::advance(iter, internal::Utilities::getRandomNumber(0U, static_cast<unsigned int>(mHealthyNodes.size()) - 1U));
    nodes.insert(*iter);
  }

  return { nodes.cbegin(), nodes.cend() };
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
void BaseNetwork<NetworkType, KeyType, NodeType>::setTransportSecurity(TLSBehavior tls)
{
  mTransportSecurity = tls;
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
void BaseNetwork<NetworkType, KeyType, NodeType>::readmitNodes()
{
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

  // Do nothing if the time for node re-admittance hasn't been reached yet.
  if (now <= mEarliestReadmitTime)
  {
    return;
  }

  // Determine the next earliest readmit time.
  std::chrono::system_clock::time_point nextEarliestReadmitTime =
    now + std::chrono::duration_cast<std::chrono::system_clock::duration>(mMaxNodeReadmitTime);

  for (const auto& node : mNodes)
  {
    if (node->getReadmitTime() > now && node->getReadmitTime() < nextEarliestReadmitTime)
    {
      nextEarliestReadmitTime = node->getReadmitTime();
    }
  }

  // Set the next earliest readmit time.
  mEarliestReadmitTime = nextEarliestReadmitTime;

  // Readmit nodes as healthy if they're passed their readmit time.
  for (const auto& node : mNodes)
  {
    if (mHealthyNodes.find(node) == mHealthyNodes.end() && node->getReadmitTime() <= now)
    {
      mHealthyNodes.insert(node);
    }
  }
}

//-----
template<typename NetworkType, typename KeyType, typename NodeType>
void BaseNetwork<NetworkType, KeyType, NodeType>::removeNodeFromNetwork(const std::shared_ptr<NodeType>& node)
{
  mNetwork[node->getKey()].erase(node);
  mNodes.erase(node);
  mHealthyNodes.erase(node);
}

/**
 * Explicit template instantiations.
 */
template class BaseNetwork<Network, AccountId, Node>;
template class BaseNetwork<MirrorNetwork, BaseNodeAddress, MirrorNode>;

} // namespace Hedera::internal
