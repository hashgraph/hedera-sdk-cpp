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
#ifndef HEDERA_SDK_CPP_TOKEN_ID_H_
#define HEDERA_SDK_CPP_TOKEN_ID_H_

#include <memory>
#include <string>
#include <string_view>

namespace proto
{
class TokenID;
}

namespace Hedera
{
/**
 * The ID for a token on Hedera.
 */
class TokenId
{
public:
  TokenId() = default;

  /**
   * Construct with a token number.
   *
   * @param num The desired token number.
   * @throws std::invalid_argument If the token number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  explicit TokenId(const uint64_t& num);

  /**
   * Construct with a shard, realm, and token number.
   *
   * @param shard The desired shard number.
   * @param realm The desired realm number.
   * @param num   The desired token number.
   * @throws std::invalid_argument If any number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  explicit TokenId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num);

  /**
   * Compare this TokenId to another TokenId and determine if they represent the same token.
   *
   * @param other The other TokenId with which to compare this TokenId.
   * @return \c TRUE if this TokenId is the same as the input TokenId, otherwise \c FALSE.
   */
  bool operator==(const TokenId& other) const;

  /**
   * Construct a TokenId object from a string of the form "<shard>.<realm>.<num>".
   *
   * @param id The token ID string from which to construct.
   * @throws std::invalid_argument If the input string is malformed.
   */
  [[nodiscard]] static TokenId fromString(std::string_view id);

  /**
   * Construct an TokenId object from a TokenID protobuf object.
   *
   * @param proto The TokenID protobuf object from which to construct a TokenId object.
   * @return The constructed TokenId object.
   */
  [[nodiscard]] static TokenId fromProtobuf(const proto::TokenID& proto);

  /**
   * Construct a TokenID protobuf object from this TokenId object.
   *
   * @return A pointer to the created TokenID protobuf object filled with this TokenId object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::TokenID> toProtobuf() const;

  /**
   * Get the string representation of this TokenId object with the form "<shard>.<realm>.<num>".
   *
   * @return The string representation of this TokenId.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Set the shard number.
   *
   * @param num The desired shard number to set.
   * @return A reference to this TokenId object with the newly-set shard number.
   * @throws std::invalid_argument If the shard number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  TokenId& setShardNum(const uint64_t& num);

  /**
   * Set the realm number.
   *
   * @param num The realm number to set.
   * @return A reference to this TokenId object with the newly-set realm number.
   * @throws std::invalid_argument If the realm number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  TokenId& setRealmNum(const uint64_t& num);

  /**
   * Set the token number.
   *
   * @param num The token number to set.
   * @return A reference to this TokenId object with the newly-set token number.
   * @throws std::invalid_argument If the account number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  TokenId& setTokenNum(const uint64_t& num);

  /**
   * Get the shard number.
   *
   * @return The shard number.
   */
  [[nodiscard]] inline uint64_t getShardNum() const { return mShardNum; }

  /**
   * Get the realm number.
   *
   * @return The realm number.
   */
  [[nodiscard]] inline uint64_t getRealmNum() const { return mRealmNum; }

  /**
   * Get the token number.
   *
   * @return The token number.
   */
  [[nodiscard]] inline uint64_t getTokenNum() const { return mTokenNum; }

private:
  /**
   * Check if the shard, realm, or token numbers (respectively) are too big.
   *
   * @throws std::invalid_argument If the shard, realm, or token number (respectively) is too big.
   */
  void checkShardNum() const;
  void checkRealmNum() const;
  void checkTokenNum() const;

  /**
   * The shard number.
   */
  uint64_t mShardNum = 0ULL;

  /**
   * The realm number.
   */
  uint64_t mRealmNum = 0ULL;

  /**
   * The token ID number.
   */
  uint64_t mTokenNum = 0ULL;
};

} // namespace Hedera

namespace std
{
template<>
struct hash<Hedera::TokenId>
{
  /**
   * Operator override to enable use of TokenId as map key
   */
  size_t operator()(const Hedera::TokenId& id) const { return hash<string>()(id.toString()); }
};

} // namespace std

#endif // HEDERA_SDK_CPP_TOKEN_ID_H_
