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
#ifndef HEDERA_SDK_CPP_CONTRACT_LOG_INFO_H_
#define HEDERA_SDK_CPP_CONTRACT_LOG_INFO_H_

#include "ContractId.h"

#include <cstddef>
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
   * Construct a ContractLogInfo object from a ContractLogInfo protobuf object.
   *
   * @param proto The ContractLogInfo protobuf object from which to construct a ContractLogInfo object.
   * @return The constructed ContractLogInfo object.
   */
  [[nodiscard]] static ContractLogInfo fromProtobuf(const proto::ContractLoginfo& proto);

  /**
   * Get the ID of the contract that emitted this log event.
   *
   * @return The ID of the contract that emitted this log event.
   */
  [[nodiscard]] inline ContractId getContractId() const { return mContractId; }

  /**
   * Get the bloom filter for this particular log event.
   *
   * @return The bloom filter for this particular log event.
   */
  [[nodiscard]] inline std::vector<std::byte> getBloom() const { return mBloom; }

  /**
   * Get the topics for this particular log event.
   *
   * @return The topics for this particular log event.
   */
  [[nodiscard]] inline std::vector<std::vector<std::byte>> getTopics() const { return mTopics; }

  /**
   * Get the log event data.
   *
   * @return The log event data.
   */
  [[nodiscard]] inline std::vector<std::byte> getData() const { return mBloom; }

private:
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
