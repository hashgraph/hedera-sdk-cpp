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
#include "impl/Network.h"
#include "AccountId.h"
#include "impl/Endpoint.h"
#include "impl/NodeAddress.h"

#include <algorithm>
#include <cmath>
#include <fstream>

namespace Hedera::internal
{
//-----
Network Network::forMainnet()
{
  return getNetworkForLedgerId(LedgerId::MAINNET);
}

//-----
Network Network::forTestnet()
{
  return getNetworkForLedgerId(LedgerId::TESTNET);
}

//-----
Network Network::forPreviewnet()
{
  return getNetworkForLedgerId(LedgerId::PREVIEWNET);
}

//-----
Network Network::forNetwork(const std::unordered_map<std::string, AccountId>& networkMap)
{
  return Network(networkMap);
}

//-----
Network& Network::setLedgerId(const LedgerId& ledgerId)
{
  return setLedgerIdInternal(ledgerId, getAddressBookForLedgerId(ledgerId));
}

//-----
Network& Network::setVerifyCertificates(bool verify)
{
  mVerifyCertificates = verify;

  // Set the new certificate verification policy for all Nodes on this Network.
  std::for_each(getNodes().cbegin(),
                getNodes().cend(),
                [&verify](const std::shared_ptr<Node>& node) { node->setVerifyCertificates(verify); });

  return *this;
}

//-----
Network& Network::setMaxNodesPerRequest(unsigned int max)
{
  mMaxNodesPerRequest = max;
  return *this;
}

//-----
Network& Network::setTransportSecurity(TLSBehavior tls)
{
  if (isTransportSecurity() != tls)
  {
    for (const std::shared_ptr<Node>& node : getNodes())
    {
      switch (tls)
      {
        case TLSBehavior::REQUIRE:
        {
          node->toSecure();
          break;
        }
        case TLSBehavior::DISABLE:
        {
          node->toInsecure();
          break;
        }
        default:
        {
          // Unimplemented other TLSBehaviors. Do nothing for now.
        }
      }
    }

    BaseNetwork<Network, AccountId, Node>::setTransportSecurityInternal(tls);
  }

  return *this;
}

//-----
std::vector<AccountId> Network::getNodeAccountIdsForExecute()
{
  std::vector<AccountId> accountIds;
  for (const std::shared_ptr<Node>& node : getNumberOfMostHealthyNodes(
         mMaxNodesPerRequest > 0U ? std::min(mMaxNodesPerRequest, static_cast<unsigned int>(getNodes().size()))
                                  : static_cast<unsigned int>(std::ceil(static_cast<double>(getNodes().size()) / 3.0))))
  {
    accountIds.push_back(node->getAccountId());
  }

  return accountIds;
}

//-----
std::unordered_map<std::string, AccountId> Network::getNetwork() const
{
  std::unordered_map<std::string, AccountId> network;
  for (const std::shared_ptr<Node>& node : getNodes())
  {
    network[node->getAddress().toString()] = node->getAccountId();
  }

  return network;
}

//-----
Network::Network(const std::unordered_map<std::string, AccountId>& network)
{
  setNetwork(network);
}

//-----
Network Network::getNetworkForLedgerId(const LedgerId& ledgerId)
{
  const std::unordered_map<AccountId, NodeAddress> addressBook = getAddressBookForLedgerId(ledgerId);
  std::unordered_map<std::string, AccountId> network;
  for (const auto& [accountId, nodeAddress] : addressBook)
  {
    for (const auto& endpoint : nodeAddress.getEndpoints())
    {
      if (endpoint.getPort() == BaseNodeAddress::PORT_NODE_TLS)
      {
        network[endpoint.toString()] = accountId;
        break;
      }
    }
  }

  return Network(network).setLedgerIdInternal(ledgerId, addressBook);
}

//-----
std::unordered_map<AccountId, NodeAddress> Network::getAddressBookForLedgerId(const LedgerId& ledgerId)
{
  // The address book can only be fetched for known Hedera networks.
  if (!ledgerId.isKnownNetwork())
  {
    return {};
  }

  std::ifstream infile(ledgerId.toString() + ".pb", std::ios_base::binary);
  const NodeAddressBook addressBook =
    NodeAddressBook::fromBytes({ std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() });

  std::unordered_map<AccountId, NodeAddress> addresses;
  for (const auto& nodeAddress : addressBook.getNodeAddresses())
  {
    addresses.try_emplace(nodeAddress.getAccountId(), nodeAddress);
  }

  return addresses;
}

//-----
std::shared_ptr<Node> Network::createNodeFromNetworkEntry(std::string_view address, const AccountId& key) const
{
  auto node = std::make_shared<Node>(key, address);
  node->setVerifyCertificates(mVerifyCertificates);
  return node;
}

//-----
Network& Network::setLedgerIdInternal(const LedgerId& ledgerId,
                                      const std::unordered_map<AccountId, NodeAddress>& addressBook)
{
  // Set the ledger ID.
  BaseNetwork<Network, AccountId, Node>::setLedgerId(ledgerId);

  // Update each Node's address book entry with the new address book.
  std::for_each(getNodes().cbegin(),
                getNodes().cend(),
                [&addressBook](const std::shared_ptr<Node>& node)
                {
                  node->setNodeCertificateHash(addressBook.empty()
                                                 ? std::vector<std::byte>()
                                                 : addressBook.at(node->getAccountId()).getCertHash());
                });

  return *this;
}

} // namespace Hedera::internal
