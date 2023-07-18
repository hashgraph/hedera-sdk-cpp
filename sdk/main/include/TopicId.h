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
#ifndef HEDERA_SDK_CPP_TOPIC_ID_H_
#define HEDERA_SDK_CPP_TOPIC_ID_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace proto
{
class TopicID;
}

namespace Hedera
{
/**
 * The ID for a topic on Hedera.
 */
class TopicId
{
public:
  TopicId() = default;

  /**
   * Construct with a topic number.
   *
   * @param num The desired topic number.
   */
  explicit TopicId(const uint64_t& num);

  /**
   * Construct with a shard, realm, and topic number.
   *
   * @param shard The desired shard number.
   * @param realm The desired realm number.
   * @param num   The desired topic number.
   */
  explicit TopicId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num);

  /**
   * Compare this TopicId to another TopicId and determine if they represent the same topic.
   *
   * @param other The other TopicId with which to compare this TopicId.
   * @return \c TRUE if this TopicId is the same as the input TopicId, otherwise \c FALSE.
   */
  bool operator==(const TopicId& other) const;

  /**
   * Construct a TopicId object from a string of the form "<shard>.<realm>.<num>".
   *
   * @param id The topic ID string from which to construct.
   * @throws std::invalid_argument If the input string is malformed.
   * @return The constructed TopicId object.
   */
  [[nodiscard]] static TopicId fromString(std::string_view id);

  /**
   * Construct a TopicId object from a solidity address.
   *
   * @param addr The solidity address from which to construct.
   * @throws std::invalid_argument If the input address is malformed.
   * @return The constructed TopicId object.
   */
  [[nodiscard]] static TopicId fromSolidityAddress(std::string_view addr);

  /**
   * Construct a TopicId object from a TopicId protobuf object.
   *
   * @param proto The TopicId protobuf object from which to construct a TopicId object.
   * @return The constructed TopicId object.
   */
  [[nodiscard]] static TopicId fromProtobuf(const proto::TopicID& proto);

  /**
   * Construct a TopicId object from a byte array.
   *
   * @param bytes The byte array from which to construct.
   * @return The constructed TopicId object.
   * @throws std::invalid_argument If a TopicId object cannot be constructed from the input byte array.
   */
  [[nodiscard]] static TopicId fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Get the solidity address representation of this TopicId object.
   *
   * @return The solidity address representation, as a string.
   */
  [[nodiscard]] std::string toSolidityAddress() const;

  /**
   * Construct a TopicId protobuf object from this TopicId object.
   *
   * @return A pointer to the created TopicId protobuf object filled with this TopicId object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::TopicID> toProtobuf() const;

  /**
   * Get the byte representation of this TopicId object.
   *
   * @return The byte representation of this TopicId.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Get the string representation of this TopicId object with the form "<shard>.<realm>.<num>".
   *
   * @return The string representation of this TopicId.
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
   * The topic ID number.
   */
  uint64_t mTopicNum = 0ULL;
};

} // namespace Hedera

namespace std
{
template<>
struct hash<Hedera::TopicId>
{
  /**
   * Operator override to enable use of TopicId as map key
   */
  size_t operator()(const Hedera::TopicId& id) const { return hash<string>()(id.toString()); }
};

} // namespace std

#endif // HEDERA_SDK_CPP_TOPIC_ID_H_
