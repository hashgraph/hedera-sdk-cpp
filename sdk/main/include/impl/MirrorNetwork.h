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
#ifndef HEDERA_SDK_CPP_IMPL_MIRROR_NETWORK_H_
#define HEDERA_SDK_CPP_IMPL_MIRROR_NETWORK_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Hedera::internal
{
class MirrorNode;
}

namespace Hedera::internal
{
class MirrorNetwork
{
public:
  /**
   * Construct a MirrorNetwork that is pre-configured for Hedera Mainnet access.
   *
   * @return A MirrorNetwork object that is set-up to communicate with the Hedera Mainnet.
   */
  [[nodiscard]] static MirrorNetwork forMainnet();

  /**
   * Construct a MirrorNetwork that is pre-configured for Hedera Testnet access.
   *
   * @return A MirrorNetwork object that is set-up to communicate with the Hedera Testnet.
   */
  [[nodiscard]] static MirrorNetwork forTestnet();

  /**
   * Construct a MirrorNetwork that is pre-configured for Hedera Previewnet access.
   *
   * @return A MirrorNetwork object that is set-up to communicate with the Hedera Previewnet.
   */
  [[nodiscard]] static MirrorNetwork forPreviewnet();

  /**
   * Construct a MirrorNetwork that is pre-configured for a custom network.
   *
   * @param nodeList The list of MirrorNode addresses for the custom network.
   * @return A MirrorNetwork object that is set-up to communicate with a specific node addresses.
   */
  [[nodiscard]] static MirrorNetwork forNetwork(const std::vector<std::string>& networkMap);

  /**
   * Get a pointer to the next MirrorNode.
   *
   * @return A pointer to the next MirrorNode.
   */
  [[nodiscard]] std::shared_ptr<MirrorNode> getNextMirrorNode() const;

  /**
   * Initiate an orderly shutdown of communications with the MirrorNodes that are a part of this MirrorNetwork.
   *
   * After this method returns, this MirrorNetwork can be re-used. All network communication can be re-established as
   * needed.
   */
  void close() const;

private:
  /**
   * Establish communications with all MirrorNodes for this MirrorNetwork that are specified in the input address list.
   *
   * @param addresses The addresses of the MirrorNodes with which this MirrorNetwork will be communicating.
   */
  void setNetwork(const std::vector<std::string>& addresses);

  /**
   * The list of pointers to MirrorNodes with which this MirrorNetwork is communicating.
   */
  std::vector<std::shared_ptr<MirrorNode>> mNodes;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_MIRROR_NETWORK_H_
