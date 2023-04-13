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
#ifndef HEDERA_SDK_CPP_CONTRACT_ID_H_
#define HEDERA_SDK_CPP_CONTRACT_ID_H_

#include "EvmAddress.h"

#include <memory>
#include <optional>

namespace proto
{
class ContractID;
}

namespace Hedera
{
/**
 * The ID for a smart contract instance.
 */
class ContractId
{
public:
  ContractId() = default;

  /**
   * Construct with a contract number.
   *
   * @param num The desired contract number.
   * @throws std::invalid_argument If the contract number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  explicit ContractId(const uint64_t& num);

  /**
   * Construct with an EVM address.
   *
   * @param address The desired EVM address.
   */
  explicit ContractId(const EvmAddress& address);

  /**
   * Construct with a shard, realm, and contract number.
   *
   * @param shard The desired shard number.
   * @param realm The desired realm number.
   * @param num   The desired contract number.
   * @throws std::invalid_argument If any number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  explicit ContractId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num);

  /**
   * Construct with shard and realm numbers, and an EVM address.
   *
   * @param shard   The desired shard number.
   * @param realm   The desired realm number.
   * @param address The desired EVM address.
   * @throws std::invalid_argument If the shard or realm number is too big (max value is
   *                               std::numeric_limits<int64_t>::max()).
   */
  explicit ContractId(const uint64_t& shard, const uint64_t& realm, const EvmAddress& address);

  /**
   * Compare this ContractId to another ContractId and determine if they represent the same contract.
   *
   * @param other The other ContractId with which to compare this ContractId.
   * @return \c TRUE if this ContractId is the same as the input ContractId, otherwise \c FALSE.
   */
  bool operator==(const ContractId& other) const;

  /**
   * Construct an ContractId object from an ContractID protobuf object.
   *
   * @param proto The ContractID protobuf object from which to create an ContractId object.
   * @return The constructed ContractId object.
   */
  [[nodiscard]] static ContractId fromProtobuf(const proto::ContractID& id);

  /**
   * Construct an ContractID protobuf object from this ContractId object.
   *
   * @return A pointer to the created ContractID protobuf object filled with this ContractId object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::ContractID> toProtobuf() const;

  /**
   * Set the shard number.
   *
   * @param num The desired shard number to set.
   * @return A reference to this ContractId object with the newly-set shard number.
   * @throws std::invalid_argument If the shard number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  ContractId& setShardNum(const uint64_t& num);

  /**
   * Set the realm number.
   *
   * @param num The realm number to set.
   * @return A reference to this ContractId object with the newly-set realm number.
   * @throws std::invalid_argument If the realm number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  ContractId& setRealmNum(const uint64_t& num);

  /**
   * Set the contract number. This is mutually exclusive with mEvmAddress, and will reset the value of mEvmAddress if is
   * set.
   *
   * @param num The contract number to set.
   * @return A reference to this ContractId object with the newly-set contract number.
   * @throws std::invalid_argument If the contract number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  ContractId& setContractNum(const uint64_t& num);

  /**
   * Set the contract EVM address. This is mutually exclusive with mContractNum, and will reset the value of the
   * mContractNum is is set.
   *
   * @param address The EVM address to set.
   * @return A reference to this ContractId object with the newly-set contract EVM address.
   */
  ContractId& setEvmAddress(const EvmAddress& address);

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
  [[nodiscard]] inline std::optional<uint64_t> getContractNum() const { return mContractNum; }

  /**
   * Get the contract EVM address.
   *
   * @return The contract EVM address.
   */
  [[nodiscard]] inline std::optional<EvmAddress> getEvmAddress() const { return mEvmAddress; }

private:
  /**
   * Check if the shard, realm, or contract numbers (respectively) are too big.
   *
   * @throws std::invalid_argument If the shard, realm, or contract number (respectively) is too big.
   */
  void checkShardNum() const;
  void checkRealmNum() const;
  void checkContractNum() const;

  /**
   * The shard number.
   */
  uint64_t mShardNum = 0ULL;

  /**
   * The realm number.
   */
  uint64_t mRealmNum = 0ULL;

  /**
   * The contract number.
   */
  std::optional<uint64_t> mContractNum;

  /**
   * The 20-byte EVM address of the contract.
   *
   * Every contract has an EVM address determined by its <tt>shard.realm.num</tt> id. This address is as follows:
   *  - The first 4 bytes are the big-endian representation of the shard.
   *  - The next 8 bytes are the big-endian representation of the realm.
   *  - The final 8 bytes are the big-endian representation of the number.
   *
   * Contracts created via CREATE2 have an additional, primary address that is derived from the EIP-1014 specification,
   * and does not have a simple relation to a shard.realm.num id.
   *
   * (Please do note that CREATE2 contracts can also be referenced by the three-part EVM address described above.)
   */
  std::optional<EvmAddress> mEvmAddress;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_ID_H_
