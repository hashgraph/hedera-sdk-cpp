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
#ifndef HEDERA_SDK_CPP_FREEZE_TYPE_H_
#define HEDERA_SDK_CPP_FREEZE_TYPE_H_

#include <string_view>
#include <unordered_map>

namespace proto
{
enum FreezeType : int;
}

namespace Hedera
{
/**
 * The type of network freeze or upgrade operation to be performed. This type dictates which fields are required.
 */
enum class FreezeType
{
  /**
   * An (invalid) default value for this enum, to ensure the client explicitly sets the intended type of freeze
   * transaction.
   */
  UNKNOWN_FREEZE_TYPE,
  /**
   * Freezes the network at the specified time. The start time field must be provided and must reference a future time.
   * Any values specified for the update file and file hash fields will be ignored. This transaction does not perform
   * any network changes or upgrades and requires manual intervention to restart the network.
   */
  FREEZE_ONLY,
  /**
   * A non-freezing operation that initiates network wide preparation in advance of a scheduled freeze upgrade. The
   * update_file and file_hash fields must be provided and valid. The start_time field may be omitted and any value
   * present will be ignored.
   */
  PREPARE_UPGRADE,
  /**
   * Freezes the network at the specified time and performs the previously prepared automatic upgrade across the entire
   * network.
   */
  FREEZE_UPGRADE,
  /**
   * Aborts a pending network freeze operation.
   */
  FREEZE_ABORT,
  /**
   * Performs an immediate upgrade on auxiliary services and containers providing telemetry/metrics. Does not impact
   * network operations.
   */
  TELEMETRY_UPGRADE
};

/**
 * Map of protobuf FreezeTypes to the corresponding FreezeType.
 */
extern const std::unordered_map<proto::FreezeType, FreezeType> gProtobufFreezeTypeToFreezeType;

/**
 * Map of FreezeTypes to the corresponding protobuf FreezeType.
 */
extern const std::unordered_map<FreezeType, proto::FreezeType> gFreezeTypeToProtobufFreezeType;

/**
 * Map of FreezeType to its corresponding string.
 */
[[maybe_unused]] extern const std::unordered_map<FreezeType, const char*> gFreezeTypeToString;

} // namespace Hedera

#endif // HEDERA_SDK_CPP_FREEZE_TYPE_H_
