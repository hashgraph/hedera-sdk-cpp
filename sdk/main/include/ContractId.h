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

#include <memory>

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
   * Define a default comparator operator so that ContractId's can be compared.
   */
  bool operator==(const ContractId&) const = default;

  /**
   * Construct with a contract number.
   *
   * @param num The contract number to set.
   */
  explicit ContractId(const uint64_t& num);

  /**
   * Construct with with a shard, realm, and contract number.
   *
   * @param shard The shard number to set.
   * @param realm The realm number to set.
   * @param num   The contract number to set.
   */
  explicit ContractId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num);

  /**
   * Retrieve the contract ID from an ContractID protobuf object.
   *
   * @param proto The ContractID protobuf object.
   * @return An ContractId object with the ContractID protobuf object data.
   */
  static ContractId fromProtobuf(const proto::ContractID& id);

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
   */
  ContractId& setShardNum(const uint64_t& num);

  /**
   * Set the realm number.
   *
   * @param num The realm number to set.
   * @return A reference to this ContractId object with the newly-set realm number.
   */
  ContractId& setRealmNum(const uint64_t& num);

  /**
   * Set the contract number.
   *
   * @param num The contract number to set.
   * @return A reference to this ContractId object with the newly-set contract number.
   */
  ContractId& setContractNum(const uint64_t& num);

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
  [[nodiscard]] inline uint64_t getContractNum() const { return mContractNum; }

private:
  /**
   * The shard number. Defaults to 0.
   */
  uint64_t mShardNum = 0ULL;

  /**
   * The realm number. Defaults to 0.
   */
  uint64_t mRealmNum = 0ULL;

  /**
   * The contract number. Defaults to 0.
   */
  uint64_t mContractNum = 0ULL;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_ID_H_
