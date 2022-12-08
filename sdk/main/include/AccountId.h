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
#ifndef HEDERA_SDK_CPP_ACCOUNT_ID_H_
#define HEDERA_SDK_CPP_ACCOUNT_ID_H_

#include <memory>
#include <string>

namespace proto
{
class AccountID;
}

namespace Hedera
{
/**
 * The ID for a crypto-currency account on Hedera.
 */
class AccountId
{
public:
  AccountId() = default;

  /**
   * Define a default comparator operator so that AccountId objects can be compared.
   */
  bool operator==(const AccountId&) const = default;

  /**
   * Construct with an account number.
   *
   * @param num The account number to set.
   * @throws std::invalid_argument If the account number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  explicit AccountId(const uint64_t& num);

  /**
   * Construct with a shard, realm, and account number.
   *
   * @param shard The shard number to set.
   * @param realm The realm number to set.
   * @param num   The account number to set.
   * @throws std::invalid_argument If any number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  explicit AccountId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num);

  /**
   * Construct from a string of the form "<shard>.<realm>.<num>".
   *
   * @param str The string from which to construct.
   * @throws std::invalid_argument If input string is malformed.
   */
  explicit AccountId(const std::string& str);

  /**
   * Create an AccountId object from an AccountID protobuf object.
   *
   * @param proto The AccountID protobuf object from which to create an AccountId object.
   * @return The created AccountId object.
   */
  static AccountId fromProtobuf(const proto::AccountID& proto);

  /**
   * Construct an AccountID protobuf object from this AccountId object.
   *
   * @return A pointer to the created AccountID protobuf object filled with this AccountId object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::AccountID> toProtobuf() const;

  /**
   * Get a string representation of this AccountId object with the form "<shard>.<realm>.<num>".
   *
   * @return A string representation of this AccountId.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Set the shard number.
   *
   * @param num The desired shard number to set.
   * @return A reference to this AccountId object with the newly-set shard number.
   * @throws std::invalid_argument If the shard number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  AccountId& setShardNum(const uint64_t& num);

  /**
   * Set the realm number.
   *
   * @param num The realm number to set.
   * @return A reference to this AccountId object with the newly-set realm number.
   * @throws std::invalid_argument If the realm number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  AccountId& setRealmNum(const uint64_t& num);

  /**
   * Set the account number.
   *
   * @param num The account number to set.
   * @return A reference to this AccountId object with the newly-set account number.
   * @throws std::invalid_argument If the account number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  AccountId& setAccountNum(const uint64_t& num);

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
   * Get the account number.
   *
   * @return The account number.
   */
  [[nodiscard]] inline uint64_t getAccountNum() const { return mAccountNum; }

private:
  /**
   * The shard number.
   */
  uint64_t mShardNum = 0ULL;

  /**
   * The realm number.
   */
  uint64_t mRealmNum = 0ULL;

  /**
   * The account ID number.
   */
  uint64_t mAccountNum = 0ULL;
};

} // namespace Hedera

namespace std
{
template<>
struct hash<Hedera::AccountId>
{
  /**
   * Operator override to enable use of AccountId as map key
   */
  size_t operator()(const Hedera::AccountId& id) const { return hash<string>()(id.toString()); }
};

} // namespace std

#endif // HEDERA_SDK_CPP_ACCOUNT_ID_H_
