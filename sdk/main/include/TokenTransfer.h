/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_TOKEN_TRANSFER_H_
#define HEDERA_SDK_CPP_TOKEN_TRANSFER_H_

#include "AccountId.h"
#include "TokenId.h"

#include <memory>
#include <vector>

namespace proto
{
class AccountAmount;
class TokenTransferList;
}

namespace Hedera
{
/**
 * A token ID and list of amounts representing the transferred out (negative) or into (positive) amounts, represented
 * in the lowest denomination of the token.
 */
class TokenTransfer
{
public:
  TokenTransfer() = default;

  /**
   * Construct with a token ID, account ID, amount, and approval.
   *
   * @param tokenId The ID of the token involved with this TokenTransfer.
   * @param accountId The ID of the account to/from which the token is being transferred.
   * @param amount The amount of the token being transferred.
   * @param isApproved \c TRUE if this transfer is approved, otherwise \c FALSE.
   */
  TokenTransfer(const TokenId& tokenId, AccountId accountId, int64_t amount, bool isApproved);

  /**
   * Construct with a token ID, account ID, amount, expected decimals of the token, and approval.
   *
   * @param tokenId The ID of the token involved with this TokenTransfer.
   * @param accountId The ID of the account to/from which the token is being transferred.
   * @param amount The amount of the token being transferred.
   * @param isApproved \c TRUE if this transfer is approved, otherwise \c FALSE.
   */
  TokenTransfer(const TokenId& tokenId, AccountId accountId, int64_t amount, uint32_t decimals, bool isApproved);

  /**
   * Construct a list of TokenTransfers objects from a TokenTransferList protobuf object.
   *
   * @param proto The TokenTransferList protobuf object from which to construct the list of TokenTransfer objects.
   * @return The list of TokenTransfer objects.
   */
  [[nodiscard]] static std::vector<TokenTransfer> fromProtobuf(const proto::TokenTransferList& proto);

  /**
   * Construct an AccountAmount protobuf object from this TokenTransfer object.
   *
   * @return A pointer to the constructed AccountAmount protobuf object.
   */
  [[nodiscard]] std::unique_ptr<proto::AccountAmount> toProtobuf() const;

  /**
   * The ID of the token being transferred.
   */
  TokenId mTokenId;

  /**
   * The ID of the account to/from which the token is being transferred.
   */
  AccountId mAccountId;

  /**
   * The amount of the token to transfer.
   */
  int64_t mAmount = 0LL;

  /**
   * The expected decimals of the transfer amount.
   */
  uint32_t mExpectedDecimals = 0U;

  /**
   * If \c TRUE then the transfer is expected to be an approved allowance.
   */
  bool mIsApproval = false;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_TRANSFER_H_
