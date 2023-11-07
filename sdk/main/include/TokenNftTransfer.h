/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#ifndef HEDERA_SDK_CPP_TOKEN_NFT_TRANSFER_H_
#define HEDERA_SDK_CPP_TOKEN_NFT_TRANSFER_H_

#include "AccountId.h"
#include "NftId.h"

#include <memory>

namespace proto
{
class NftTransfer;
}

namespace Hedera
{
/**
 * A sender account, a receiver account, and the serial number of an NFT of a Token with NON_FUNGIBLE_UNIQUE type. When
 * minting NFTs the sender will be the default AccountID instance (0.0.0) and when burning NFTs, the receiver will be
 * the default AccountID instance.
 */
class TokenNftTransfer
{
public:
  TokenNftTransfer() = default;

  /**
   * Construct with an nft ID, sender account ID, receiver account ID, and approval.
   *
   * @param nftId The ID of the NFT.
   * @param sender The ID of the account sending the NFT.
   * @param receiver The ID of the account receiving the NFT.
   * @param approved \c TRUE if this is an approved allowance NFT transfer, otherwise \c FALSE.
   */
  TokenNftTransfer(NftId nftId, AccountId sender, AccountId receiver, bool approved);

  /**
   * Construct a TokenNftTransfer object from an NftTransfer protobuf object and a TokenId object.
   *
   * @param proto   The NftTransfer protobuf object from which to construct an TokenNftTransfer object.
   * @param tokenId The ID of the token.
   * @return The constructed TokenNftTransfer object.
   */
  [[nodiscard]] static TokenNftTransfer fromProtobuf(const proto::NftTransfer& proto, const TokenId& tokenId);

  /**
   * Validate the checksums of the entities associated with this TokenNftTransfer.
   *
   * @param client The Client to use to validate the checksums.
   * @throws BadEntityException If any of the checksums are not valid.
   */
  void validateChecksums(const Client& client) const;

  /**
   * Construct an NftTransfer protobuf object from this TokenNftTransfer object.
   *
   * @return A pointer to a created NftTransfer protobuf object filled with this TokenNftTransfer object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::NftTransfer> toProtobuf() const;

  /**
   * The ID of the NFT.
   */
  NftId mNftId;

  /**
   * The account ID of the sender.
   */
  AccountId mSenderAccountId;

  /**
   * The account ID of the receiver.
   */
  AccountId mReceiverAccountId;

  /**
   * If \c TRUE then the transfer is expected to be an approved allowance and the senderAccountID is expected to be the
   * owner.
   */
  bool mIsApproval = false;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_NFT_TRANSFER_H_
