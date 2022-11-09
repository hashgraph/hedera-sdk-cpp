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
#ifndef CONTRACT_ID_H_
#define CONTRACT_ID_H_

#include <memory>
#include <optional>

namespace proto
{
class ContractID;
}

namespace Hedera
{
/**
 * The ID for a smart contract instance on Hedera.
 */
class ContractId
{
public:
  /**
   * Default constructor.
   */
  ContractId() = default;

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
   * Default comparator operator.
   *
   * @param other The other ContractId to compare.
   * @return \c TRUE if the input ContractId is the same as this one, otherwise \c FALSE
   */
  bool operator==(const ContractId& other) const = default;

  /**
   * Retrieve the contract ID from a protobuf ContractID.
   *
   * @param proto The ContractID protobuf object.
   * @return A ContractId object with the protobuf ContractID data.
   */
  static ContractId fromProtobuf(const proto::ContractID& id);

  /**
   * Convert this ContractId to its corresponding protobuf ContractID.
   *
   * @return Pointer to the created protobuf ContractID.
   */
  [[nodiscard]] proto::ContractID* toProtobuf() const;

  /**
   * Set the shard number.
   *
   * @param num The shard number to set.
   * @return Reference to this ContractId object.
   */
  ContractId& setShardNum(const uint64_t& num);

  /**
   * Set the realm number.
   *
   * @param num The realm number to set.
   * @return Reference to this ContractId object.
   */
  ContractId& setRealmNum(const uint64_t& num);

  /**
   * Set the contract number.
   *
   * @param num The contract number to set.
   * @return Reference to this ContractId object.
   */
  ContractId& setContractNum(const uint64_t& num);

  /**
   * Extract the shard number.
   *
   * @return The shard number.
   */
  [[nodiscard]] inline uint64_t getShardNum() const { return mShardNum; }

  /**
   * Extract the realm number.
   *
   * @return The realm number.
   */
  [[nodiscard]] inline uint64_t getRealmNum() const { return mRealmNum; }

  /**
   * Extract the contract number.
   *
   * @return The contract number.
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

#endif // CONTRACT_ID_H_