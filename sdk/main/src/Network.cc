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
#include "Network.h"

#include "AccountId.h"

namespace Hedera
{
namespace
{
//-----
const std::unordered_map<std::string, AccountId, StringHash, std::equal_to<>>
  TESTNET_NODES = {
    {"0.testnet.hedera.com:50211",  AccountId(3LL)},
    { "34.94.106.61:50211",         AccountId(3LL)},
    { "50.18.132.211:50211",        AccountId(3LL)},
    { "138.91.142.219:50211",       AccountId(3LL)},
    { "1.testnet.hedera.com:50211", AccountId(4LL)},
    { "35.237.119.55:50211",        AccountId(4LL)},
    { "3.212.6.13:50211",           AccountId(4LL)},
    { "52.168.76.241:50211",        AccountId(4LL)},
    { "2.testnet.hedera.com:50211", AccountId(5LL)},
    { "35.245.27.193:50211",        AccountId(5LL)},
    { "52.20.18.86:50211",          AccountId(5LL)},
    { "40.79.83.124:50211",         AccountId(5LL)},
    { "3.testnet.hedera.com:50211", AccountId(6LL)},
    { "34.83.112.116:50211",        AccountId(6LL)},
    { "54.70.192.33:50211",         AccountId(6LL)},
    { "52.183.45.65:50211",         AccountId(6LL)},
    { "4.testnet.hedera.com:50211", AccountId(7LL)},
    { "34.94.160.4:50211",          AccountId(7LL)},
    { "54.176.199.109:50211",       AccountId(7LL)},
    { "13.64.181.136:50211",        AccountId(7LL)},
    { "5.testnet.hedera.com:50211", AccountId(8LL)},
    { "34.106.102.218:50211",       AccountId(8LL)},
    { "35.155.49.147:50211",        AccountId(8LL)},
    { "13.78.238.32:50211",         AccountId(8LL)},
    { "6.testnet.hedera.com:50211", AccountId(9LL)},
    { "34.133.197.230:50211",       AccountId(9LL)},
    { "52.14.252.207:50211",        AccountId(9LL)},
    { "52.165.17.231:50211",        AccountId(9LL)},
};

} // namespace

//-----
Network Network::forTestnet()
{
  Network network;
  network.setNetwork(TESTNET_NODES);
  return network;
}

//-----
void Network::setNetwork(
  const std::unordered_map<std::string, AccountId, StringHash, std::equal_to<>>&
    network)
{
  for (const auto& [url, accountId] : network)
  {
    mNodes.push_back(Node(url, accountId));
  }
}

//-----
void Network::close()
{
  for (auto& node : mNodes)
  {
    node.shutdown();
  }
}

} // namespace Hedera
