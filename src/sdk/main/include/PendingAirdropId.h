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
#ifndef HEDERA_SDK_CPP_PENDING_AIRDROP_ID_H_
#define HEDERA_SDK_CPP_PENDING_AIRDROP_ID_H_

#include "AccountId.h"
#include "NftId.h"
#include "TokenId.h"

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace proto
{
class PendingAirdropId;
class TokenReference;
}

namespace Hedera
{
class Client;
}

namespace Hedera
{
/**
 * A unique, composite, identifier for a pending airdrop.
 *
 * Each pending airdrop SHALL be uniquely identified by a PendingAirdropId.
 * A PendingAirdropId SHALL be recorded when created and MUST be provided in any transaction
 * that would modify that pending airdrop (such as a `claimAirdrop` or `cancelAirdrop`).
 */
class PendingAirdropId
{
public:
  PendingAirdropId() = default;

  /**
   * Construct with two AccountIDs.
   *
   * @param sender The accountID of the sender.
   * @param receiver The accountID of the receiver.
   */
  explicit PendingAirdropId(AccountId sender, AccountId receiver);

  /**
   * Construct an PendingAirdropId object from an PendingAirdropId protobuf object.
   *
   * @param proto The PendingAirdropId protobuf object from which to create an PendingAirdropId object.
   * @return The constructed PendingAirdropId object.
   */
  [[nodiscard]] static PendingAirdropId fromProtobuf(const proto::PendingAirdropId& proto);

  /**
   * Verify the checksum of this PendingAirdropId using the input Client's network.
   *
   * @param client The Client with which to validate this PendingAirdropId's checksum.
   * @throws BadEntityException If the checksum of this PendingAirdropId is invalid.
   */
  void validateChecksum(const Client& client) const;

  /**
   * Construct an PendingAirdropId protobuf object from this PendingAirdropId object.
   *
   * @return A pointer to the created PendingAirdropId protobuf object filled with this PendingAirdropId object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::PendingAirdropId> toProtobuf() const;

  /**
   * Get the string representation of this PendingAirdropId object.
   *
   * @return The string representation of this PendingAirdropId.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * A sending account.
   * This is the account that initiated, and SHALL fund, this pending airdrop.
   * This field is REQUIRED.
   */
  AccountId mSenderId;

  /**
   * A receiving account.
   * This is the ID of the account that SHALL receive the airdrop.
   * This field is REQUIRED.
   */
  AccountId mReceiverId;

  /**
   * A token ID.
   * This is the type of token for a fungible/common token airdrop.
   * This field is REQUIRED for a fungible/common token and MUST NOT be used for a
   * non-fungible/unique token.
   */
  std::optional<TokenId> mFt;

  /**
   * The id of a single NFT, consisting of a Token ID and serial number.
   * This is the type of token for a non-fungible/unique token airdrop.
   * This field is REQUIRED for a non-fungible/unique token and MUST NOT be used for a
   * fungible/common token.
   */
  std::optional<NftId> mNft;
};

}

#endif // HEDERA_SDK_CPP_PENDING_AIRDROP_ID_H_