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

#include <cstddef>
#include <memory>
#include <ostream>
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
  ContractNonceInfo() = default;

  /**
   * Construct with a contract ID and nonce.
   *
   * @param contractId The ID of the contract.
   * @param nonce      The nonce.
   */
  explicit ContractNonceInfo(ContractId contractId, const int64_t& nonce);

  /**
   * Compare this ContractNonceInfo to another ContractNonceInfo and determine if they represent the same nonce for a
   * specific ContractId.
   *
   * @param other The other ContractNonceInfo with which to compare this ContractNonceInfo.
   * @return \c TRUE if this ContractNonceInfo is the same as the input ContractNonceInfo, otherwise \c FALSE.
   */
  bool operator==(const ContractNonceInfo& other) const;

  /**
   * Construct a ContractNonceInfo object from a ContractNonceInfo protobuf object.
   *
   * @param proto The ContractNonceInfo protobuf object from which to construct a ContractNonceInfo object.
   * @return The constructed ContractNonceInfo object.
   */
  [[nodiscard]] static ContractNonceInfo fromProtobuf(const proto::ContractNonceInfo& proto);

  /**
   * Construct a ContractNonceInfo object from a byte array.
   *
   * @param bytes The byte array from which to construct a TransactionRecord object.
   * @return The constructed ContractNonceInfo object.
   */
  [[nodiscard]] static ContractNonceInfo fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a ContractNonceInfo protobuf object from this ContractNonceInfo object.
   *
   * @return A pointer to the created ContractNonceInfo protobuf object.
   */
  [[nodiscard]] std::unique_ptr<proto::ContractNonceInfo> toProtobuf() const;

  /**
   * Construct a representative byte array from this ContractNonceInfo object.
   *
   * @return The byte array representing this ContractNonceInfo object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Construct a string representation of this ContractNonceInfo object.
   *
   * @return The string representation of this ContractNonceInfo object.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Write this ContractNonceInfo to an output stream.
   *
   * @param os        The output stream.
   * @param nonceInfo The ContractNonceInfo to print.
   * @return The output stream with this ContractNonceInfo written to it.
   */
  friend std::ostream& operator<<(std::ostream& os, const ContractNonceInfo& nonceInfo);

  /**
   * The ID of the contract.
   */
  ContractId mContractId;

  /**
   * The current value of the contract account's nonce property.
   */
  int64_t mNonce = 0LL;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_NONCE_INFO_H_
