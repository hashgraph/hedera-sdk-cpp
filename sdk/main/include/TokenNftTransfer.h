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
  /**
   * Construct a TokenNftTransfer object from an NftTransfer protobuf object.
   *
   * @param proto The NftTransfer protobuf object from which to construct an TokenNftTransfer object.
   * @return The constructed TokenNftTransfer object.
   */
  [[nodiscard]] static TokenNftTransfer fromProtobuf(const proto::NftTransfer& proto);

  /**
   * Construct an NftTransfer protobuf object from this TokenNftTransfer object.
   *
   * @return A pointer to a created NftTransfer protobuf object filled with this TokenNftTransfer object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::NftTransfer> toProtobuf() const;

  /**
   * Set the ID of the NFT.
   *
   * @param nftId The ID of the NFT.
   * @param A reference to this TokenNftTransfer object with the newly-set NFT ID.
   */
  TokenNftTransfer& setNftId(const NftId& nftId);

  /**
   * Set the ID of the account sending the NFT.
   *
   * @param accountId The ID of the account that is sending the NFT.
   * @return A reference to this TokenNftTransfer object with the newly-set sender account ID.
   */
  TokenNftTransfer& setSenderAccountId(const AccountId& accountId);

  /**
   * Set the ID of the account receiving the NFT.
   *
   * @param accountId The ID of the account that is receiving the NFT.
   * @return A reference to this TokenNftTransfer object with the newly-set receiver account ID.
   */
  TokenNftTransfer& setReceiverAccountId(const AccountId& accountId);

  /**
   * Set if this TokenNftTransfer is approved.
   *
   * @param approval \c TRUE if this is an approved transfer, otherwise \c FALSE.
   * @return A reference to this TokenNftTransfer object with the newly-set approval status.
   */
  TokenNftTransfer& setApproval(bool approval);

  /**
   * Get the NFT ID.
   *
   * @return The NFT ID.
   */
  [[nodiscard]] inline NftId getNftId() const { return mNftId; }

  /**
   * Get the sender account ID.
   *
   * @return The sender account ID.
   */
  [[nodiscard]] inline AccountId getSenderAccountId() const { return mSenderAccountID; }

  /**
   * Get the receiver account ID.
   *
   * @return The receiver account ID.
   */
  [[nodiscard]] inline AccountId getReceiverAccountId() const { return mReceiverAccountID; }

  /**
   * Determine if this transfer is approved or not.
   *
   * @return \c TRUE if this is an approved transfer, otherwise \c FALSE.
   */
  [[nodiscard]] inline bool getApproval() const { return mIsApproval; }

private:
  /**
   * The ID of the NFT.
   */
  NftId mNftId;

  /**
   * The account ID of the sender.
   */
  AccountId mSenderAccountID;

  /**
   * The account ID of the receiver.
   */
  AccountId mReceiverAccountID;

  /**
   * If \c TRUE then the transfer is expected to be an approved allowance and the senderAccountID is expected to be the
   * owner.
   */
  bool mIsApproval = false;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_NFT_TRANSFER_H_
