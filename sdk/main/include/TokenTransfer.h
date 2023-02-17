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

namespace Hedera
{
/**
 * A token ID and list of amounts representing the transferred out (negative) or into (positive) amounts, represented
 * in the lowest denomination of the token.
 */
class TokenTransfer
{
public:
  /**
   * Set the ID of the token being transferred.
   *
   * @param tokenId The ID of the token that is being transferred.
   * @return A reference to this TokenTransfer object with the newly-set token ID.
   */
  TokenTransfer& setTokenId(const TokenId& tokenId);

  /**
   * Set the ID of the account sending/receiving the token.
   *
   * @param accountId The ID of the account that is sending/receiving the token.
   * @return A reference to this TokenTransfer object with the newly-set account ID.
   */
  TokenTransfer& setAccountId(const AccountId& accountId);

  /**
   * Set the amount of the token being transferred.
   *
   * @param amount The amount of the token to transfer.
   * @return A reference to this TokenTransfer object with the newly-set amount.
   */
  TokenTransfer& setAmount(const int64_t& amount);

  /**
   * Set the number of decimals of the transfer amount. (e.g. amount of 10,055 with decimals of 2 means 100.55 of the
   * token gets transferred).
   *
   * @param decimals The number of decimals in the transfer amount.
   * @return A reference to this TokenTransfer object with the newly-set number of decimals.
   */
  TokenTransfer& setExpectedDecimals(uint32_t decimals);

  /**
   * Set the approval status of this transfer.
   *
   * @param approval \c TRUE if this transfer should be approved, otherwise \c FALSE.
   * @return A reference to this TokenTransfer object with the newly-set approval.
   */
  TokenTransfer& setApproval(bool approval);

  /**
   * Get the token ID.
   *
   * @return The token ID.
   */
  [[nodiscard]] inline TokenId getTokenId() const { return mTokenId; }

  /**
   * Get the account ID.
   *
   * @return The account ID.
   */
  [[nodiscard]] inline AccountId getAccountId() const { return mAccountId; }

  /**
   * Get the amount of token to be transferred.
   *
   * @return The amount of token to be transferred.
   */
  [[nodiscard]] inline int64_t getAmount() const { return mAmount; }

  /**
   * Get the expected decimals of the transfer amount.
   *
   * @return The expected decimals of the transfer amount.
   */
  [[nodiscard]] inline uint32_t getExpectedDecimals() const { return mExpectedDecimals; }

  /**
   * Determine if this transfer is approved or not.
   *
   * @return \c TRUE if this is an approved transfer, otherwise \c FALSE.
   */
  [[nodiscard]] inline bool getApproval() const { return mIsApproval; }

private:
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
  int64_t mAmount = 0ULL;

  /**
   * The expected decimals of transfer.
   */
  uint32_t mExpectedDecimals = 0U;

  /**
   * If \c TRUE then the transfer is expected to be an approved allowance.
   */
  bool mIsApproval = false;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_TRANSFER_H_
