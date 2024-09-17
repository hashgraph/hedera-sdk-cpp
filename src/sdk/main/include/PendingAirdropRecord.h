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
#ifndef HEDERA_SDK_CPP_PENDING_AIRDROP_RECORD_H_
#define HEDERA_SDK_CPP_PENDING_AIRDROP_RECORD_H_

#include "PendingAirdropId.h"

namespace proto
{
class PendingAirdropRecord;
class PendingAirdropId;
}

namespace Hedera
{
/**
 * A record of a new pending airdrop.
 */
class PendingAirdropRecord
{
public:
  PendingAirdropRecord() = default;

  /**
   * Construct with two AccountIDs.
   *
   * @param PendingAirdropId The PendingAirdropId.
   * @param amount The amount airdropped.
   */
  explicit PendingAirdropRecord(PendingAirdropId PendingAirdropId, uint64_t amount);

  /**
   * Construct an PendingAirdropRecord object from an PendingAirdropRecord protobuf object.
   *
   * @param proto The PendingAirdropRecord protobuf object from which to create an PendingAirdropRecord object.
   * @return The constructed PendingAirdropRecord object.
   */
  [[nodiscard]] static PendingAirdropRecord fromProtobuf(const proto::PendingAirdropRecord& proto);

  /**
   * Construct an PendingAirdropRecord protobuf object from this PendingAirdropRecord object.
   *
   * @return A pointer to the created PendingAirdropRecord protobuf object filled with this PendingAirdropRecord
   * object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::PendingAirdropRecord> toProtobuf() const;

  /**
   * Get the string representation of this PendingAirdropRecord object.
   *
   * @return The string representation of this PendingAirdropRecord.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * A unique, composite, identifier for a pending airdrop.
   * This field is REQUIRED.
   */
  PendingAirdropId mPendingAirdropId;

  /**
   * A single pending airdrop amount.
   * If the pending airdrop is for a fungible/common token this field is REQUIRED
   * and SHALL be the current amount of tokens offered.
   * If the pending airdrop is for a non-fungible/unique token, this field SHALL NOT
   * be set.
   */
  uint64_t mAmount;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_PENDING_AIRDROP_RECORD_H_