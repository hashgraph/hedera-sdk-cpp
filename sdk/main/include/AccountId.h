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
#ifndef ACCOUNT_ID_H_
#define ACCOUNT_ID_H_

#include "PublicKey.h"

#include <memory>
#include <optional>

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
  /**
   * Default constructor.
   */
  AccountId() = default;

  /**
   * Construct with an account number.
   *
   * @param num The account number to set.
   */
  explicit AccountId(const uint64_t& num);

  /**
   * Construct with a shard, realm, and account number.
   *
   * @param shard The shard number to set.
   * @param realm The realm number to set.
   * @param num   The account number to set.
   */
  explicit AccountId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num);

  /**
   * Construct from a string of the form "<shard>.<realm>.<num>".
   *
   * @param str The string from which to construct.
   * @throws std::invalid_argument if input string is malformed.
   */
  explicit AccountId(const std::string& str);

  /**
   * Default comparator operator.
   *
   * @param other The other AccountId to compare
   * @return \c TRUE if the input AccountId is the same as this one, otherwise \c FALSE
   */
  bool operator==(const AccountId& other) const = default;

  /**
   * Retrieve the account ID from a protobuf AccountID.
   *
   * @param proto The AccountID protobuf object.
   * @return An AccountId object with the protobuf AccountID data.
   */
  static AccountId fromProtobuf(const proto::AccountID& proto);

  /**
   * Convert this AccountId to its corresponding protobuf AccountID.
   *
   * @return Pointer to the created protobuf AccountID.
   */
  proto::AccountID* toProtobuf() const;

  /**
   * Put this AccountId in a string with the form "<shard>.<realm>.<num>".
   *
   * @return String representation of this AccountId.
   */
  std::string toString() const;

  /**
   * Set the shard number.
   *
   * @param num The shard number to set.
   * @return Reference to this AccountId object.
   */
  AccountId& setShardNum(const uint64_t& num);

  /**
   * Set the realm number.
   *
   * @param num The realm number to set.
   * @return Reference to this AccountId object.
   */
  AccountId& setRealmNum(const uint64_t& num);

  /**
   * Set the account number.
   *
   * @param num The account number to set.
   * @return Reference to this AccountId object.
   */
  AccountId& setAccountNum(const uint64_t& num);

  /**
   * Extract the shard number.
   *
   * @return The shard number.
   */
  inline uint64_t getShardNum() const { return mShardNum; }

  /**
   * Extract the realm number.
   *
   * @return The realm number.
   */
  inline uint64_t getRealmNum() const { return mRealmNum; }

  /**
   * Extract the account number.
   *
   * @return The account number.
   */
  inline std::optional<uint64_t> getAccountNum() const { return mAccountNum; }

private:
  /**
   * The shard number.
   */
  uint64_t mShardNum;

  /**
   * The realm number.
   */
  uint64_t mRealmNum;

  /**
   * The account ID number.
   */
  std::optional<uint64_t> mAccountNum;
};

} // namespace Hedera

#endif // ACCOUNT_ID_H_