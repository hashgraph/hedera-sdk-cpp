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
#ifndef ACCOUNT_ID_H_
#define ACCOUNT_ID_H_

#include "EvmAddress.h"
#include "PublicKey.h"

#include "helper/InitType.h"

#include <regex>
#include <string>

namespace proto
{
class AccountID;
}

namespace Hedera
{
template<typename T>
class InitType;

class Client;
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
   * Constructor
   */
  AccountId();

  /**
   * Construct with the account number part of the account ID.
   *
   * @param num The account number with which to construct this account ID.
   */
  explicit AccountId(const int64_t& num);

  /**
   * Construct with all parts of the account ID.
   *
   * @param shard The shard number to set.
   * @param realm The realm number to set.
   * @param num   The account number to set.
   */
  explicit AccountId(const int64_t& shard,
                     const int64_t& realm,
                     const int64_t& num);

  /**
   * Construct with all parts of the account ID and the checksum.
   *
   * @param shard    The shard number to set.
   * @param realm    The realm number to set.
   * @param num      The account number to set.
   * @param checksum The checksum of the account ID.
   */
  explicit AccountId(const int64_t& shard,
                     const int64_t& realm,
                     const int64_t& num,
                     const InitType<std::string>& checksum);

  /**
   * Construct with all parts of the account ID, the checksum, and the aliases.
   *
   * @param shard        The shard number to set.
   * @param realm        The realm number to set.
   * @param num          The account number to set.
   * @param aliasKey     The key of the account alias.
   * @param aliasEvmAddr The EVM address of the account alias.
   * @param checksum     The checksum of the account ID.
   */
  explicit AccountId(const int64_t& shard,
                     const int64_t& realm,
                     const int64_t& num,
                     const InitType<PublicKey>& aliasKey,
                     const InitType<EvmAddress>& aliasEvmAddr,
                     const InitType<std::string>& checksum);

  /**
   * Determine if this account ID is equal to another.
   *
   * @param other The account ID with which to compare
   * @return \c TRUE if the input account ID equals this one, otherwise \c FALSE
   */
  bool operator==(const AccountId& other) const;

  /**
   * Retrieve the account ID from a string.
   *
   * @param id A string representing a valid account ID.
   * @return   The account ID object.
   */
  static AccountId fromString(const std::string& id);

  /**
   * Retrieve the account ID from a solidity address.
   *
   * @param address A string representing the solidity address.
   * @return        The account ID object.
   */
  static AccountId fromSolidityAddress(const std::string& address);

  /**
   * Retrieve the account ID from a protobuf.
   *
   * @param id The account ID protobuf object.
   * @return   The account ID object.
   */
  static AccountId fromProtobuf(const proto::AccountID& id);

  /**
   * Extract the solidity address.
   *
   * @return The solidity address as a string.
   */
  std::string toSolidityAddress() const;

  /**
   * Convert this account ID to its corresponding protobuf object.
   *
   * @return A dynamically-allocated account ID protobuf object filled-in with
   *         this account ID's data. It's the responsibility of the user to
   *         delete this object when done with it.
   */
  proto::AccountID* toProtobuf() const;

  /**
   * Verify that the client has a valid checksum.
   *
   * @param client The client to verify
   */
  void validateChecksum(const Client& client) const;

  /**
   * Extract a string represenation of this account ID.
   *
   * @return A string representation of this account ID.
   */
  std::string toString() const;

  /**
   * Extract a string representation of this account ID with the checksum.
   *
   * @param client The client with which the checksum is generated.
   * @return       The account ID with checksum.
   */
  std::string toStringWithChecksum(const Client& client) const;

  /**
   * Extract the checksum of this account ID.
   *
   * @return This account ID's checksum and its validity.
   */
  inline InitType<std::string> getChecksum() const { return mChecksum; }

  /**
   * The shard number.
   */
  int64_t mShard;

  /**
   * The realm number.
   */
  int64_t mRealm;

  /**
   * The id number.
   */
  int64_t mAccountNum;

  /**
   * The alias key.
   */
  InitType<PublicKey> mAliasKey;

  /**
   * The alias EVM address.
   */
  InitType<EvmAddress> mAliasEvmAddress;

private:
  /**
   * This account ID's checksum.
   */
  InitType<std::string> mChecksum;
};

} // namespace Hedera

#endif // ACCOUNT_ID_H_