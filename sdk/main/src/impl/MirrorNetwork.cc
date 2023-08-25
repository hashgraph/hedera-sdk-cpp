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
#include "impl/MirrorNetwork.h"
#include "impl/MirrorNode.h"

#include <random>

namespace Hedera::internal
{
//-----
MirrorNetwork MirrorNetwork::forMainnet()
{
  MirrorNetwork network;
  network.setNetwork({ "mainnet-public.mirrornode.hedera.com:443" });
  return network;
}

//-----
MirrorNetwork MirrorNetwork::forTestnet()
{
  MirrorNetwork network;
  network.setNetwork({ "testnet.mirrornode.hedera.com:443" });
  return network;
}

//-----
MirrorNetwork MirrorNetwork::forPreviewnet()
{
  MirrorNetwork network;
  network.setNetwork({ "previewnet.mirrornode.hedera.com:443" });
  return network;
}

//-----
MirrorNetwork MirrorNetwork::forNetwork(const std::vector<std::string>& networkMap)
{
  MirrorNetwork network;
  network.setNetwork(networkMap);
  return network;
}

//-----
std::shared_ptr<MirrorNode> MirrorNetwork::getNextMirrorNode() const
{
  if (mNodes.empty())
  {
    return nullptr;
  }

  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution distribution(0, static_cast<int>(mNodes.size()) - 1);
  return mNodes.at(distribution(gen));
}

//-----
void MirrorNetwork::close() const
{
  for (const auto& node : mNodes)
  {
    node->shutdown();
  }
}

//-----
void MirrorNetwork::setNetwork(const std::vector<std::string>& addresses)
{
  for (const std::string& str : addresses)
  {
    mNodes.push_back(std::make_shared<MirrorNode>(str));
  }
}

} // namespace Hedera::internal
