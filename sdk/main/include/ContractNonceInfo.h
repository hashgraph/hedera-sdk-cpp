/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_CONTRACT_NONCE_INFO_H_
#define HEDERA_SDK_CPP_CONTRACT_NONCE_INFO_H_

#include "ContractId.h"

#include <string>
#include <vector>

namespace proto
{
class ContractNonceInfo;
}

namespace Hedera
{
/**
 * Info about a contract account's nonce value.
 * A nonce of a contract is only incremented when that contract creates another contract.
 */
class ContractNonceInfo
{
public:
  /**
   * Id of the contract
   */
  ContractId mContractId;

  /**
   * The current value of the contract account's nonce property
   */
  int64_t mNonce;

  /**
   * Default construct for empty ContractNonceInfo object.
   */
  ContractNonceInfo() = default;

  /**
   * Construct with a contract Id and nonce.
   */
  explicit ContractNonceInfo(const ContractId& contractId, const int64_t& nonce);

  /**
   * Compare this ContractNonceInfo to another ContractNonceInfo and determine if they represent
   * the same nonce for a specific ContractId.
   *
   * @param other The other ContractNonceInfo with which to compare this ContractNonceInfo.
   * @return \c TRUE if this ContractNonceInfo is the same as the input ContractNonceInfo, otherwise \c FALSE.
   */
  bool operator==(const ContractNonceInfo& other) const;

  /**
   * Create an ContractNonceInfo object from a protobuf object.
   *
   * @param proto the protobuf object
   * @return The constructed ContractNonceInfo object.
   */
  [[nodiscard]] static ContractNonceInfo fromProtobuf(const proto::ContractNonceInfo& proto);

  /**
   * Create an ContractNonceInfo object from an array of bytes.
   *
   * @param bytes The bytes from which to create an ContractNonceInfo object.
   * @return The constructed ContractNonceInfo object.
   */
  [[nodiscard]] static ContractNonceInfo fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a ContractNonceInfo protobuf object from this ContractNonceInfo object.
   *
   * @return A pointer to the created ContractNonceInfo protobuf object filled with this ContractNonceInfo object's
   *         data.
   */
  [[nodiscard]] std::unique_ptr<proto::ContractNonceInfo> toProtobuf() const;

  /**
   * Create a byte array representation of this ContractNonceInfo object.
   *
   * @return A byte array representation of this ContractNonceInfo object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Get a string representation of this ContractNonceInfo object.
   *
   * @return A string representation of this ContractNonceInfo.
   */
  [[nodiscard]] std::string toString() const;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_NONCE_INFO_H_
