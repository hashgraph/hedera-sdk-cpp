/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_CONTRACT_LOG_INFO_H_
#define HEDERA_SDK_CPP_CONTRACT_LOG_INFO_H_

#include "ContractId.h"

#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace proto
{
class ContractLoginfo;
}

namespace Hedera
{
/**
 * The log information for an event returned by a smart contract function call. One function call may return several
 * such events.
 */
class ContractLogInfo
{
public:
  /**
   * Construct a ContractLogInfo object from a ContractLoginfo protobuf object.
   *
   * @param proto The ContractLoginfo protobuf object from which to construct a ContractLogInfo object.
   * @return The constructed ContractLogInfo object.
   */
  [[nodiscard]] static ContractLogInfo fromProtobuf(const proto::ContractLoginfo& proto);

  /**
   * Construct a ContractLogInfo object from a byte array.
   *
   * @param bytes The byte array from which to construct a TransactionRecord object.
   * @return The constructed ContractLogInfo object.
   */
  [[nodiscard]] static ContractLogInfo fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a ContractLoginfo protobuf object from this ContractLogInfo object.
   *
   * @return A pointer to the created ContractLoginfo protobuf object.
   */
  [[nodiscard]] std::unique_ptr<proto::ContractLoginfo> toProtobuf() const;

  /**
   * Construct a representative byte array from this ContractLogInfo object.
   *
   * @return The byte array representing this ContractLogInfo object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Construct a string representation of this ContractLogInfo object.
   *
   * @return The string representation of this ContractLogInfo object.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Write this ContractLogInfo to an output stream.
   *
   * @param os      The output stream.
   * @param logInfo The ContractLogInfo to print.
   * @return The output stream with this ContractLogInfo written to it.
   */
  friend std::ostream& operator<<(std::ostream& os, const ContractLogInfo& logInfo);

  /**
   * The ID of the contract that emitted this log event.
   */
  ContractId mContractId;

  /**
   * The bloom filter for this particular log event.
   */
  std::vector<std::byte> mBloom;

  /**
   * The topics for this particular log event.
   */
  std::vector<std::vector<std::byte>> mTopics;

  /**
   * The log event data.
   */
  std::vector<std::byte> mData;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_LOG_INFO_H_
