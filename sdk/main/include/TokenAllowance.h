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
#ifndef HEDERA_SDK_CPP_TOKEN_ALLOWANCE_H_
#define HEDERA_SDK_CPP_TOKEN_ALLOWANCE_H_

#include "AccountId.h"
#include "TokenId.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace proto
{
class TokenAllowance;
}

namespace Hedera
{
/**
 * An approved allowance of fungible token transfers for a spender.
 */
class TokenAllowance
{
public:
  TokenAllowance() = default;

  /**
   * Construct with a token ID, owner, spender, and amount.
   *
   * @param tokenId The ID of the token that is being approved to be spent.
   * @param owner   The ID of the account approving an allowance of its tokens.
   * @param spender The ID of the account being allowed to spend the tokens.
   * @param amount  The amount of tokens that are being allowed to be spent.
   */
  TokenAllowance(const TokenId& tokenId, AccountId owner, AccountId spender, uint64_t amount);

  /**
   * Construct a TokenAllowance object from a TokenAllowance protobuf object.
   *
   * @param proto The TokenAllowance protobuf object from which to construct an TokenAllowance object.
   * @return The constructed TokenAllowance object.
   */
  [[nodiscard]] static TokenAllowance fromProtobuf(const proto::TokenAllowance& proto);

  /**
   * Construct a TokenAllowance object from a byte array.
   *
   * @param bytes The byte array from which to construct an TokenAllowance object.
   * @return The constructed TokenAllowance object.
   */
  [[nodiscard]] static TokenAllowance fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a TokenAllowance protobuf object from this TokenAllowance object.
   *
   * @return A pointer to a constructed TokenAllowance protobuf object filled with this TokenAllowance object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::TokenAllowance> toProtobuf() const;

  /**
   * Construct a byte array from this TokenAllowance object.
   *
   * @return A byte array representing this TokenAllowance object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

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
   * The amount of tokens that are being allowed to be spent.
   */
  uint64_t mAmount = 0ULL;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_ALLOWANCE_H_
