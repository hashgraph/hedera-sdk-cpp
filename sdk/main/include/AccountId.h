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

#include "EvmAddress.h"
#include "PublicKey.h"

#include <memory>
#include <optional>
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
   * Construct with an account number.
   *
   * @param num The desired account number.
   * @throws std::invalid_argument If the account number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  explicit AccountId(const uint64_t& num);

  /**
   * Construct with an account alias.
   *
   * @param alias The desired public key account alias.
   */
  explicit AccountId(const std::shared_ptr<PublicKey>& alias);

  /**
   * Construct with an EVM address.
   *
   * @param address The desired EVM address.
   */
  explicit AccountId(const EvmAddress& address);

  /**
   * Construct with a shard, realm, and account number.
   *
   * @param shard The desired shard number.
   * @param realm The desired realm number.
   * @param num   The desired account number.
   * @throws std::invalid_argument If any number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  explicit AccountId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num);

  /**
   * Construct with shard and realm numbers, and an account alias.
   *
   * @param shard The desired shard number.
   * @param realm The desired realm number.
   * @param alias The desired account alias.
   * @throws std::invalid_argument If the shard or realm number is too big (max value is
   *                               std::numeric_limits<int64_t>::max()).
   */
  explicit AccountId(const uint64_t& shard, const uint64_t& realm, const std::shared_ptr<PublicKey>& alias);

  /**
   * Construct with shard and realm numbers, and an EVM address.
   *
   * @param shard   The desired shard number.
   * @param realm   The desired realm number.
   * @param address The desired EVM address.
   * @throws std::invalid_argument If the shard or realm number is too big (max value is
   *                               std::numeric_limits<int64_t>::max()).
   */
  explicit AccountId(const uint64_t& shard, const uint64_t& realm, const EvmAddress& address);

  /**
   * Compare this AccountId to another AccountId and determine if they represent the same account.
   *
   * @param other The other AccountId with which to compare this AccountId.
   * @return \c TRUE if this AccountId is the same as the input AccountId, otherwise \c FALSE.
   */
  bool operator==(const AccountId& other) const;

  /**
   * Construct an AccountId object from a string of the form "<shard>.<realm>.<num>". <num> can be the account number,
   * the stringified PublicKey alias (DER-encoded), or the stringified EVM address.
   *
   * @param id The account ID string from which to construct.
   * @return The constructed AccountId object.
   * @throws std::invalid_argument If the input string is malformed or the type of <num> cannot be determined.
   */
  [[nodiscard]] static AccountId fromString(std::string_view id);

  /**
   * Create an AccountId object from an AccountID protobuf object.
   *
   * @param proto The AccountID protobuf object from which to create an AccountId object.
   * @return The constructed AccountId object.
   */
  [[nodiscard]] static AccountId fromProtobuf(const proto::AccountID& proto);

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
   * Set the account number. This is mutually exclusive with mPublicKeyAlias and mEvmAddressAlias, and will reset the
   * value of the mPublicKeyAlias or mEvmAddressAlias if either is set.
   *
   * @param num The account number to set.
   * @return A reference to this AccountId object with the newly-set account number.
   * @throws std::invalid_argument If the account number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  AccountId& setAccountNum(const uint64_t& num);

  /**
   * Set the account public key alias. This is mutually exclusive with mAccountNum and mEvmAddressAlias, and will reset
   * the value of the mAccountNum or mEvmAddressAlias if either is set.
   *
   * @param alias The public key alias to set.
   * @return A reference to this AccountId object with the newly-set account alias.
   */
  AccountId& setPublicKeyAlias(const std::shared_ptr<PublicKey>& alias);

  /**
   * Set the account EVM address alias. This is mutually exclusive with mAccountNum and mPublicKeyAlias, and will reset
   * the value of the mAccountNum or mPublicKeyAlias if either is set.
   *
   * @param address The EVM address alias to set.
   * @return A reference to this AccountId object with the newly-set account EVM address.
   */
  AccountId& setEvmAddressAlias(const EvmAddress& address);

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
  [[nodiscard]] inline std::optional<uint64_t> getAccountNum() const { return mAccountNum; }

  /**
   * Get the account public key alias.
   *
   * @return The account public key alias.
   */
  [[nodiscard]] inline std::shared_ptr<PublicKey> getPublicKeyAlias() const { return mPublicKeyAlias; }

  /**
   * Get the account EVM address alias.
   *
   * @return The account EVM address alias.
   */
  [[nodiscard]] inline std::optional<EvmAddress> getEvmAddressAlias() const { return mEvmAddressAlias; }

private:
  /**
   * Check if the shard, realm, or account numbers (respectively) are too big.
   *
   * @throws std::invalid_argument If the shard, realm, or account number (respectively) is too big.
   */
  void checkShardNum() const;
  void checkRealmNum() const;
  void checkAccountNum() const;

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
  std::optional<uint64_t> mAccountNum;

  /**
   * The public key to be used as the account's alias. Currently only primitive key bytes are supported as an alias
   * (ThresholdKey, KeyList, ContractID, and delegatable_contract_id are not supported)
   *
   * At most one account can ever have a given alias and it is used for account creation if it was automatically created
   * using a crypto transfer. It will be nullptr if an account is created normally. It is immutable once it is set for
   * an account.
   *
   * If a transaction auto-creates the account, any further transfers to that alias will simply be deposited in that
   * account, without creating anything, and with no creation fee being charged.
   */
  std::shared_ptr<PublicKey> mPublicKeyAlias = nullptr;

  /**
   * The ethereum account 20-byte EVM address to be used initially in place of the public key bytes. This EVM address
   * may be either the encoded form of the shard.realm.num or the keccak-256 hash of a ECDSA_SECP256K1 primitive key.
   *
   * If a transaction lazily-creates this account, a subsequent transaction will be required containing the public key
   * bytes that map to the EVM address bytes. Lazy account creates will only support the keccak-256 hash of a
   * ECDSA_SECP256K1 primitive key form.
   */
  std::optional<EvmAddress> mEvmAddressAlias;
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
