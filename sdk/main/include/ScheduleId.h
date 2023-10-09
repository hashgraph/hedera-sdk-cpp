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
#ifndef HEDERA_SDK_CPP_SCHEDULE_ID_H_
#define HEDERA_SDK_CPP_SCHEDULE_ID_H_

#include <cstddef>
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
class Client;
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
   * @param num The schedule number.
   */
  explicit ScheduleId(uint64_t num);

  /**
   * Construct with a shard, realm, a schedule number, and optionally a checksum.
   *
   * @param shard    The shard number.
   * @param realm    The realm number.
   * @param num      The schedule number.
   * @param checksum The checksum.
   */
  explicit ScheduleId(uint64_t shard, uint64_t realm, uint64_t num, std::string_view checksum = "");

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
   * @return The constructed ScheduleId object.
   * @throws std::invalid_argument If the input string is malformed.
   */
  [[nodiscard]] static ScheduleId fromString(std::string_view id);

  /**
   * Construct a ScheduleId from a ScheduleId protobuf object.
   *
   * @param proto The ScheduleId protobuf object from which to create a ScheduleId object.
   * @return The constructed ScheduleId object.
   */
  [[nodiscard]] static ScheduleId fromProtobuf(const proto::ScheduleID& proto);

  /**
   * Construct a ScheduleId object from a representative byte array.
   *
   * @param bytes The byte array from which to construct a ScheduleId object.
   * @return The constructed ScheduleId object.
   */
  [[nodiscard]] static ScheduleId fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Verify the checksum of this ScheduleId using the input Client's network.
   *
   * @param client The Client with which to validate this ScheduleId's checksum.
   * @throws BadEntityException If the checksum of this ScheduleId is invalid.
   */
  void validateChecksum(const Client& client) const;

  /**
   * Construct a ScheduleID protobuf object from this ScheduleId object.
   *
   * @return A pointer to the created ScheduleId protobuf object filled with this ScheduleId object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::ScheduleID> toProtobuf() const;

  /**
   * Get the string representation of this ScheduleId object.
   *
   * @return The string representation of this ScheduleId.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Get the string representation of this ScheduleId object with the checksum.
   *
   * @param client The Client with which to generate the checksum.
   * @return The string representation of this ScheduleId object with the checksum.
   */
  [[nodiscard]] std::string toStringWithChecksum([[maybe_unused]] const Client& client) const;

  /**
   * Get a byte array representation of this ScheduleId object.
   *
   * @return A byte array representation of this ScheduleId object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Get the checksum of this ContractId.
   *
   * @return The checksum of this ContractId.
   */
  [[nodiscard]] inline std::string getChecksum() const { return mChecksum; }

  /**
   * The shard number.
   */
  uint64_t mShardNum = 0ULL;

  /**
   * The realm number.
   */
  uint64_t mRealmNum = 0ULL;

  /**
   * The schedule number.
   */
  uint64_t mScheduleNum = 0ULL;

private:
  /**
   * The checksum of this ScheduleIds.
   */
  mutable std::string mChecksum;
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
