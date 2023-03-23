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
#ifndef HEDERA_SDK_CPP_TOKEN_NFT_ALLOWANCE_H_
#define HEDERA_SDK_CPP_TOKEN_NFT_ALLOWANCE_H_

#include "AccountId.h"
#include "TokenId.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace proto
{
class NftAllowance;
}

namespace Hedera
{
/**
 * An approved allowance of NFT transfers for a spender.
 */
class TokenNftAllowance
{
public:
  TokenNftAllowance() = default;

  /**
   * Construct with a token ID, owner, spender, list of serial numbers, and approval policy.
   *
   * @param tokenId       The ID of the token NFT type that is being approved to be spent.
   * @param owner         The ID of the account approving an allowance of its NFTs.
   * @param spender       The ID of the account being allowed to spend the NFTs.
   * @param serialNumbers A list serial numbers of the NFT that is being allowed to be spent.
   * @param allowAll      \c TRUE to allow the spender access to all of the owner's NFTs of the designated token ID,
   *                      \c FALSE to revoke the spender's access to all of the owner's NFTs of the designated token ID.
   */
  TokenNftAllowance(const TokenId& tokenId,
                    AccountId owner,
                    AccountId spender,
                    std::vector<uint64_t> serialNumbers,
                    std::optional<bool> allowAll = std::optional<bool>());

  /**
   * Construct a TokenNftAllowance object from a NftAllowance protobuf object.
   *
   * @param proto The NftAllowance protobuf object from which to construct an TokenNftAllowance object.
   * @return The constructed TokenNftAllowance object.
   */
  [[nodiscard]] static TokenNftAllowance fromProtobuf(const proto::NftAllowance& proto);

  /**
   * Construct a NftAllowance protobuf object from this TokenNftAllowance object.
   *
   * @return A pointer to a constructed NftAllowance protobuf object filled with this TokenNftAllowance object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::NftAllowance> toProtobuf() const;

  /**
   * Set the ID of the token NFT type that is being approved to be spent.
   *
   * @param tokenId The ID of the token NFT type that is being approved to be spent.
   * @return A reference to this TokenNftAllowance object with the newly-set token ID.
   */
  TokenNftAllowance& setTokenId(const TokenId& tokenId);

  /**
   * Set the ID of the account approving an allowance of its NFTs.
   *
   * @param accountId The ID of the account approving an allowance of its NFTs.
   * @return A reference to this TokenNftAllowance object with the newly-set owner account ID.
   */
  TokenNftAllowance& setOwnerAccountId(const AccountId& accountId);

  /**
   * Set the ID of the account being allowed to spend the NFTs.
   *
   * @param accountId The ID of the account being allowed to spend the NFTs.
   * @return A reference to this TokenNftAllowance object with the newly-set spender account ID.
   */
  TokenNftAllowance& setSpenderAccountId(const AccountId& accountId);

  /**
   * Add a serial number of one of the NFTs the spender is being allowed to spend.
   *
   * @param serialNumber The serial number of the NFT that is being allowed to be spent.
   * @return A reference to this TokenNftAllowance object with the added NFT serial number.
   */
  TokenNftAllowance& addSerialNumber(const uint64_t& serialNumber);

  /**
   * Allow the spender access to all of the owner's NFTs of the designated token ID (currently owned and any in the
   * future).
   *
   * @param allowAll \c TRUE to allow the spender access to all of the owner's NFTs of the designated token ID,
   *                 \c FALSE to revoke the spender's access to all of the owner's NFTs of the designated token ID.
   * @return A reference to this TokenNftAllowance object with the newly-set NFT allowance policy.
   */
  TokenNftAllowance& approveForAll(bool allowAll);

  /**
   * Get the ID of the token NFT type that is being approved to be spent.
   *
   * @return The ID of the token NFT type that is being approved to be spent.
   */
  [[nodiscard]] inline TokenId getTokenId() const { return mTokenId; }

  /**
   * Set the ID of the account approving an allowance of its tokens.
   *
   * @return The ID of the account approving an allowance of its tokens.
   */
  [[nodiscard]] inline AccountId getOwnerAccountId() const { return mOwnerAccountId; }

  /**
   * Get the ID of the account being allowed to spend the tokens.
   *
   * @return The ID of the account being allowed to spend the tokens.
   */
  [[nodiscard]] inline AccountId getSpenderAccountId() const { return mSpenderAccountId; }

  /**
   * Get the list of NFT serial numbers that are being allowed to be spent.
   *
   * @return The list of NFT serial numbers that are being allowed to be spent.
   */
  [[nodiscard]] inline std::vector<uint64_t> getSerialNumbers() const { return mSerialNumbers; }

  /**
   * Determine if this allowance allows the spender access to all of the owner's NFTs (currently owned and in the
   * future).
   *
   * @return \c TRUE if this TokenNftAllowance allows the spender access to all of the owner's NFTs (currently owned and
   * in the future). Uninitialized if behavior has not been set.
   */
  [[nodiscard]] inline std::optional<bool> getApprovedForAll() const { return mApprovedForAll; }

  /**
   * Get the ID of the spender account who is granted approved-for-all access and is granting approval of an NFT serial
   * number to another spender.
   *
   * @return The ID of the spender account who is granted approved-for-all access and is granting approval of an NFT
   * serial number to another spender. Uninitialized if no delegating spender exists for this allowance.
   */
  [[nodiscard]] inline std::optional<AccountId> getDelegateSpender() const { return mDelegatingSpenderAccountId; }

private:
  /**
   * The ID of the token that is being approved to be spent.
   */
  TokenId mTokenId;

  /**
   * The ID of the account approving an allowance of its tokens.
   */
  AccountId mOwnerAccountId;

  /**
   * The ID of the account being allowed to spend the tokens.
   */
  AccountId mSpenderAccountId;

  /**
   * The list of serial numbers that are being allowed to be spent.
   */
  std::vector<uint64_t> mSerialNumbers;

  /**
   * Is the spender allowed access to allow of the owner's NFTs?
   */
  std::optional<bool> mApprovedForAll;

  /**
   * The ID of the spender account who is granted an approved-for-all allowance and granting approval on an NFT serial
   * to another spender.
   */
  std::optional<AccountId> mDelegatingSpenderAccountId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_NFT_ALLOWANCE_H_
