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
#ifndef HEDERA_SDK_CPP_DELEGATE_CONTRACT_ID_H_
#define HEDERA_SDK_CPP_DELEGATE_CONTRACT_ID_H_

#include "ContractId.h"

namespace proto
{
class ContractID;
}

namespace Hedera
{
/**
 * A smart contract that, if the recipient of the active message frame, should be treated as having signed. (Note this
 * does not mean the code being executed in the frame code being executed in the frame delegatecall. So setting this key
 * is a more permissive version of setting the contractId key, which also requires the code in the active message frame
 * belong to the contract with the given id. The delegate contract ID can be set as Key.
 */
class DelegateContractId : public ContractId
{
public:
  DelegateContractId() = default;

  /**
   * Construct with a contract number.
   *
   * @param num The desired contract number.
   * @throws std::invalid_argument If the contract number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  explicit DelegateContractId(const uint64_t& num);

  /**
   * Construct with a shard, realm, and contract number.
   *
   * @param shard The desired shard number.
   * @param realm The desired realm number.
   * @param num   The desired contract number.
   * @throws std::invalid_argument If any number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  explicit DelegateContractId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num);

  /**
   * Compare this DelegateContractId to another DelegateContractId and determine if they represent the same contract.
   *
   * @param other The other DelegateContractId with which to compare this DelegateContractId.
   * @return \c TRUE if this DelegateContractId is the same as the input DelegateContractId, otherwise \c FALSE.
   */
  [[nodiscard]] bool operator==(const DelegateContractId& other) const;

  /**
   * Construct an DelegateContractId object from a string of the form "<shard>.<realm>.<num>".
   *
   * @param id The account ID string from which to construct.
   * @return The constructed DelegateContractId object.
   */
  [[nodiscard]] static DelegateContractId fromString(std::string_view id);

  /**
   * Construct an DelegateContractId object from an DelegateContractId protobuf object.
   *
   * @param proto The DelegateContractId protobuf object from which to create an DelegateContractId object.
   * @return The constructed DelegateContractId object.
   */
  [[nodiscard]] static DelegateContractId fromProtobuf(const proto::ContractID& id);

  /**
   * Derived from Key. Create a clone of this DelegateContractId object.
   *
   * @return A pointer to the created clone of this DelegateContractId.
   */
  [[nodiscard]] std::unique_ptr<Key> clone() const override;

  /**
   * Derived from Key. Construct a Key protobuf object from this DelegateContractId object.
   *
   * @return A pointer to the created Key protobuf object filled with this DelegateContractId object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::Key> toProtobufKey() const override;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_DELEGATE_CONTRACT_ID_H_
