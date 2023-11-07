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
#ifndef HEDERA_SDK_CPP_NETWORK_VERSION_INFO_H_
#define HEDERA_SDK_CPP_NETWORK_VERSION_INFO_H_

#include "SemanticVersion.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace proto
{
class NetworkGetVersionInfoResponse;
}

namespace Hedera
{
/**
 * Contains information about the network's version.
 */
class NetworkVersionInfo
{
public:
  NetworkVersionInfo() = default;

  /**
   * Construct from a protobuf schema SemanticVersion and a Hedera services SemanticVersion.
   *
   * @param hapi   The SemanticVersion of the protobuf schema.
   * @param hedera The SemanticVersion of the Hedera services.
   */
  NetworkVersionInfo(const SemanticVersion& hapi, const SemanticVersion& hedera);

  /**
   * Construct a NetworkVersionInfo object from a NetworkGetVersionInfoResponse protobuf object.
   *
   * @param proto The NetworkGetVersionInfoResponse protobuf object from which to construct a NetworkVersionInfo object.
   * @return The constructed NetworkVersionInfo object.
   */
  [[nodiscard]] static NetworkVersionInfo fromProtobuf(const proto::NetworkGetVersionInfoResponse& proto);

  /**
   * Construct a NetworkVersionInfo object from a byte array.
   *
   * @param bytes The byte array from which to construct a NetworkVersionInfo object.
   * @return The constructed NetworkVersionInfo object.
   */
  [[nodiscard]] static NetworkVersionInfo fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a NetworkGetVersionInfoResponse protobuf object from this NetworkVersionInfo object.
   *
   * @return A pointer to the created NetworkVersionInfo protobuf object.
   */
  [[nodiscard]] std::unique_ptr<proto::NetworkGetVersionInfoResponse> toProtobuf() const;

  /**
   * Construct a representative byte array from this NetworkVersionInfo object.
   *
   * @return The byte array representing this NetworkVersionInfo object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * The version of the protobuf schema in use by the network.
   */
  SemanticVersion mProtobufVersion;

  /**
   * The version of the Hedera services in use by the network.
   */
  SemanticVersion mServicesVersion;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_NETWORK_VERSION_INFO_H_
