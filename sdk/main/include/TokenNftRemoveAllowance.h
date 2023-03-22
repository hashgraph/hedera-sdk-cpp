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
#ifndef HEDERA_SDK_CPP_TOKEN_NFT_REMOVE_ALLOWANCE_H_
#define HEDERA_SDK_CPP_TOKEN_NFT_REMOVE_ALLOWANCE_H_

#include "AccountId.h"
#include "TokenId.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace proto
{
class NftRemoveAllowance;
}

namespace Hedera
{
/**
 * An NFT transfer allowance to remove.
 */
class TokenNftRemoveAllowance
{
public:
  TokenNftRemoveAllowance() = default;

  /**
   * Construct with a token ID, owner, and list of serial numbers.
   *
   * @param tokenId       The ID of the token NFT type that is being approved to be spent.
   * @param owner         The ID of the account approving an allowance of its NFTs.
   * @param serialNumbers A list serial numbers of the NFT that is being allowed to be spent.
   */
  TokenNftRemoveAllowance(const TokenId& tokenId, AccountId owner, std::vector<uint64_t> serialNumbers);

  /**
   * Construct a TokenNftRemoveAllowance object from a NftRemoveAllowance protobuf object.
   *
   * @param proto The NftRemoveAllowance protobuf object from which to construct an TokenNftRemoveAllowance object.
   * @return The constructed TokenNftRemoveAllowance object.
   */
  [[nodiscard]] static TokenNftRemoveAllowance fromProtobuf(const proto::NftRemoveAllowance& proto);

  /**
   * Construct a NftRemoveAllowance protobuf object from this TokenNftRemoveAllowance object.
   *
   * @return A pointer to a constructed NftRemoveAllowance protobuf object filled with this TokenNftRemoveAllowance
   *         object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::NftRemoveAllowance> toProtobuf() const;

  /**
   * Set the ID of the token NFT type who's allowance is being revoked.
   *
   * @param tokenId The ID of the token NFT type who's allowance is being revoked.
   * @return A reference to this TokenNftRemoveAllowance object with the newly-set token ID.
   */
  TokenNftRemoveAllowance& setTokenId(const TokenId& tokenId);

  /**
   * Set the ID of the account who's revoking an NFT allowances.
   *
   * @param accountId The ID of the account who's revoking an NFT allowances.
   * @return A reference to this TokenNftRemoveAllowance object with the newly-set owner account ID.
   */
  TokenNftRemoveAllowance& setOwnerAccountId(const AccountId& accountId);

  /**
   * Add a serial number of one of the NFTs who's being removed from an allowance.
   *
   * @param serialNumber The serial number of the NFT that is being no longer being allowed to be spent.
   * @return A reference to this TokenNftRemoveAllowance object with the added NFT serial number.
   */
  TokenNftRemoveAllowance& addSerialNumber(const uint64_t& serialNumber);

  /**
   * Get the ID of the token NFT type whose allowance is being revoked.
   *
   * @return The ID of the token NFT type whose allowance is being revoked.
   */
  [[nodiscard]] inline TokenId getTokenId() const { return mTokenId; }

  /**
   * Set the ID of the account whose revoking an NFT allowances.
   *
   * @return The ID of the account whose revoking an NFT allowances.
   */
  [[nodiscard]] inline AccountId getOwnerAccountId() const { return mOwnerAccountId; }

  /**
   * Get the list of NFT serial numbers whose allowance is being revoked.
   *
   * @return The list of NFT serial numbers whose allowance is being revoked.
   */
  [[nodiscard]] inline std::vector<uint64_t> getSerialNumbers() const { return mSerialNumbers; }

private:
  /**
   * The ID of the token NFT type whose allowance is being revoked.
   */
  TokenId mTokenId;

  /**
   * The ID of the account whose revoking an NFT allowances.
   */
  AccountId mOwnerAccountId;

  /**
   * The list of NFT serial numbers whose allowance is being revoked.
   */
  std::vector<uint64_t> mSerialNumbers;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_NFT_REMOVE_ALLOWANCE_H_
