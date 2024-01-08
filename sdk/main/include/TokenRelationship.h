/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_TOKEN_RELATIONSHIP_H_
#define HEDERA_SDK_CPP_TOKEN_RELATIONSHIP_H_

#include <TokenId.h>

#include <iostream>
#include <optional>
#include <string>

namespace Hedera
{
/**
 * @brief Represents the relationship between an account and a token.
 *
 * This class encapsulates information about the account's relationship with a specific token,
 * including the token ID, symbol, balance, KYC status, freeze status, and whether the relationship is created
 * implicitly.
 */
class TokenRelationship
{
public:
  /**
   * @brief Constructor for TokenRelationship.
   *
   * @param tokenId The unique token ID.
   * @param symbol The symbol of the token.
   * @param balance The balance of the account in the smallest denomination for fungible common tokens,
   *                or the number of NFTs held by the account for non-fungible unique tokens.
   * @param kycStatus The KYC status of the account (optional).
   * @param freezeStatus The freeze status of the account (optional).
   * @param automaticAssociation Specifies if the relationship is created implicitly.
   */
  TokenRelationship(const TokenId& tokenId,
                    const std::string& symbol,
                    uint64_t balance,
                    const std::string& kycStatus,
                    const std::string& freezeStatus,
                    bool automaticAssociation);

  /**
   * @brief Converts the TokenRelationship object to a string representation.
   *
   * @return A string representation of the TokenRelationship.
   */
  std::string toString() const;

private:
  /**
   * @brief Sets the KYC status based on the provided string.
   *
   * @param kycStatusString The string representation of KYC status ("GRANTED", "REVOKED", or "NOT_APPLICABLE").
   * @throws std::invalid_argument if the provided string is not a valid KYC status.
   */
  void setKycStatus(const std::string& kycStatusString);

  /**
   * @brief Sets the freeze status based on the provided string.
   *
   * @param freezeStatus The string representation of freeze status ("FROZEN", "UNFROZEN", or "NOT_APPLICABLE").
   * @throws std::invalid_argument if the provided string is not a valid freeze status.
   */
  void setFreezeStatus(const std::string& freezeStatus);

  TokenId mTokenId;                  /**< The unique token ID. */
  std::string mSymbol;               /**< The symbol of the token. */
  uint64_t mBalance;                 /**< The balance of the account. */
  std::optional<bool> mKycStatus;    /**< The KYC status of the account (optional). */
  std::optional<bool> mFreezeStatus; /**< The freeze status of the account (optional). */
  bool mAutomaticAssociation;        /**< Specifies if the relationship is created implicitly. */
};
} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_RELATIONSHIP_H_
