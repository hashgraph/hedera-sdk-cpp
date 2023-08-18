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
#ifndef HEDERA_SDK_CPP_TOKEN_WIPE_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOKEN_WIPE_TRANSACTION_H_

#include "AccountId.h"
#include "TokenId.h"
#include "Transaction.h"

#include <cstdint>
#include <vector>

namespace proto
{
class TokenWipeAccountTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Wipes the provided amount of fungible or non-fungible tokens from the specified Hedera account. This transaction does
 * not delete tokens from the treasury account. This transaction must be signed by the token's Wipe Key. Wiping an
 * account's tokens burns the tokens and decreases the total supply.
 *
 *  - If the provided account is not found, the transaction will resolve to INVALID_ACCOUNT_ID.
 *  - If the provided account has been deleted, the transaction will resolve to ACCOUNT_DELETED.
 *  - If the provided token is not found, the transaction will resolve to INVALID_TOKEN_ID.
 *  - If the provided token has been deleted, the transaction will resolve to TOKEN_WAS_DELETED.
 *  - If an Association between the provided token and the account is not found, the transaction will resolve to
 *    TOKEN_NOT_ASSOCIATED_TO_ACCOUNT.
 *  - If Wipe Key is not present in the Token, the transaction results in TOKEN_HAS_NO_WIPE_KEY.
 *  - If the provided account is the token's Treasury Account, the transaction results in
 *    CANNOT_WIPE_TOKEN_TREASURY_ACCOUNT.
 *
 * On success, tokens are removed from the account and the total supply of the token is decreased by the wiped amount.
 * The amount provided is in the lowest denomination possible.
 *  - Example: Token A has 2 decimals. In order to wipe 100 tokens from an account, one must provide an amount of 10000.
 *             In order to wipe 100.55 tokens, one must provide an amount of 10055.
 *
 * This transaction accepts zero-unit token wipe operations for fungible tokens (HIP-564).
 *
 * Transaction Signing Requirements:
 *  - Wipe key.
 *  - Transaction fee payer account key.
 */
class TokenWipeTransaction : public Transaction<TokenWipeTransaction>
{
public:
  TokenWipeTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a TokenWipe transaction.
   */
  explicit TokenWipeTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the token to wipe.
   *
   * @param tokenId The ID of the token to wipe.
   * @return A reference to this TokenWipeTransaction object with the newly-set token ID.
   * @throws IllegalStateException If this TokenWipeTransaction is frozen.
   */
  TokenWipeTransaction& setTokenId(const TokenId& tokenId);

  /**
   * Set the ID of the account from which to wipe the token(s).
   *
   * @param accountId The ID of the account from which to wipe the token(s).
   * @return A reference to this TokenWipeTransaction object with the newly-set account ID.
   * @throws IllegalStateException If this TokenWipeTransaction is frozen.
   */
  TokenWipeTransaction& setAccountId(const AccountId& accountId);

  /**
   * Set the amount of FUNGIBLE_COMMON tokens to wipe from the account. This should be in the lowest denomination
   * possible.
   *
   * @param amount The amount of FUNGIBLE_COMMON tokens to wipe from the account.
   * @return A reference to this TokenWipeTransaction object with the newly-set amount.
   * @throws IllegalStateException If this TokenWipeTransaction is frozen.
   */
  TokenWipeTransaction& setAmount(uint64_t amount);

  /**
   * Set the serial numbers of NON_FUNGIBLE_UNIQUE tokens to wipe from the account.
   *
   * @param serialNumbers The serial numbers of NON_FUNGIBLE_UNIQUE tokens to wipe from the account.
   * @return A reference to this TokenWipeTransaction object with the newly-set serial numbers.
   * @throws IllegalStateException If this TokenWipeTransaction is frozen.
   */
  TokenWipeTransaction& setSerialNumbers(const std::vector<uint64_t>& serialNumbers);

  /**
   * Get the ID of the token to wipe.
   *
   * @return The ID of the token to wipe.
   */
  [[nodiscard]] inline TokenId getTokenId() const { return mTokenId; }

  /**
   * Get the ID of the account from which to wipe the token(s).
   *
   * @return The ID of the account from which to wipe the token(s).
   */
  [[nodiscard]] inline AccountId getAccountId() const { return mAccountId; }

  /**
   * Get the amount of FUNGIBLE_COMMON tokens to wipe from the account.
   *
   * @return The amount of FUNGIBLE_COMMON tokens to wipe from the account.
   */
  [[nodiscard]] inline uint64_t getAmount() const { return mAmount; }

  /**
   * Get the serial numbers of the NON_FUNGIBLE_UNIQUE tokens to wipe from the account.
   *
   * @return The serial numbers of the NON_FUNGIBLE_UNIQUE tokens to wipe from the account.
   */
  [[nodiscard]] inline std::vector<uint64_t> getSerialNumbers() const { return mSerialNumbers; }

private:
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TokenWipeTransaction object.
   *
   * @param client The Client trying to construct this TokenWipeTransaction.
   * @param node   The Node to which this TokenWipeTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TokenWipeTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                TokenWipeTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TokenWipeTransaction to a Node.
   *
   * @param client   The Client submitting this TokenWipeTransaction.
   * @param deadline The deadline for submitting this TokenWipeTransaction.
   * @param node     Pointer to the Node to which this TokenWipeTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the TokenWipeTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a TokenWipeAccountTransactionBody protobuf object from this TokenWipeTransaction object.
   *
   * @return A pointer to a TokenWipeAccountTransactionBody protobuf object filled with this TokenWipeTransaction
   *         object's data.
   */
  [[nodiscard]] proto::TokenWipeAccountTransactionBody* build() const;

  /**
   * The ID of the token to wipe from the account.
   */
  TokenId mTokenId;

  /**
   * The ID of the account from which to wipe the tokens.
   */
  AccountId mAccountId;

  /**
   * Applicable to tokens of type FUNGIBLE_COMMON. The amount of tokens to wipe from the specified account. Amount must
   * be a positive non-zero number in the lowest denomination possible and not bigger than the token balance of the
   * account.
   */
  uint64_t mAmount = 0ULL;

  /**
   * Applicable to tokens of type NON_FUNGIBLE_UNIQUE. THe list of serial numbers to be wiped from the account.
   */
  std::vector<uint64_t> mSerialNumbers;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_WIPE_TRANSACTION_H_
