/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_SCHEDULE_ID_H_
#define HEDERA_SDK_CPP_SCHEDULE_ID_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace proto
{
class ScheduleID;
}

namespace Hedera
{
/**
 * The ID for a schedule on Hedera.
 */
class ScheduleId
{
public:
  ScheduleId() = default;

  /**
   * Construct with a schedule number.
   *
   * @param num The desired schedule number.
   */
  explicit ScheduleId(const uint64_t& num);

  /**
   * Construct with a shard, realm, and schedule number.
   *
   * @param shard The desired shard number.
   * @param realm The desired realm number.
   * @param num   The desired schedule number.
   */
  explicit ScheduleId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num);

  /**
   * Compare this ScheduleId to another ScheduleId and determine if they represent the same schedule.
   *
   * @param other The other ScheduleId with which to compare this ScheduleId.
   * @return \c TRUE if this ScheduleId is the same as the input ScheduleId, otherwise \c FALSE.
   */
  bool operator==(const ScheduleId& other) const;

  /**
   * Construct a ScheduleId object from a string of the form "<shard>.<realm>.<num>".
   *
   * @param id The schedule ID string from which to construct.
   * @throws std::invalid_argument If the input string is malformed.
   * @return The constructed ScheduleId object.
   */
  [[nodiscard]] static ScheduleId fromString(std::string_view id);

  /**
   * Construct a ScheduleId object from a ScheduleId protobuf object.
   *
   * @param proto The ScheduleId protobuf object from which to construct a ScheduleId object.
   * @return The constructed ScheduleId object.
   */
  [[nodiscard]] static ScheduleId fromProtobuf(const proto::ScheduleID& proto);

  /**
   * Construct a ScheduleId object from a byte array.
   *
   * @param bytes The byte array from which to construct.
   * @return The constructed ScheduleId object.
   * @throws std::invalid_argument If a ScheduleId object cannot be constructed from the input byte array.
   */
  [[nodiscard]] static ScheduleId fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a ScheduleId protobuf object from this ScheduleId object.
   *
   * @return A pointer to the created ScheduleId protobuf object filled with this ScheduleId object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::ScheduleID> toProtobuf() const;

  /**
   * Get the byte representation of this ScheduleId object.
   *
   * @return The byte representation of this ScheduleId.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Get the string representation of this ScheduleId object with the form "<shard>.<realm>.<num>".
   *
   * @return The string representation of this ScheduleId.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * The shard number.
   */
  uint64_t mShardNum = 0ULL;

  /**
   * The realm number.
   */
  uint64_t mRealmNum = 0ULL;

  /**
   * The schedule ID number.
   */
  uint64_t mScheduleNum = 0ULL;
};

} // namespace Hedera

namespace std
{
template<>
struct hash<Hedera::ScheduleId>
{
  /**
   * Operator override to enable use of ScheduleId as map key
   */
  size_t operator()(const Hedera::ScheduleId& id) const { return hash<string>()(id.toString()); }
};

} // namespace std

#endif // HEDERA_SDK_CPP_SCHEDULE_ID_H_
