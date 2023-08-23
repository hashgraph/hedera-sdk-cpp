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
#ifndef HEDERA_SDK_CPP_TOKEN_UNFREEZE_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOKEN_UNFREEZE_TRANSACTION_H_

#include "AccountId.h"
#include "TokenId.h"
#include "Transaction.h"

namespace proto
{
class TokenUnfreezeAccountTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Unfreezes transfers of the specified token for the account. The transaction must be signed by the token's freeze key.
 *
 *  - If the provided account is not found, the transaction will resolve to INVALID_ACCOUNT_ID.
 *  - If the provided account has been deleted, the transaction will resolve to ACCOUNT_DELETED.
 *  - If the provided token is not found, the transaction will resolve to INVALID_TOKEN_ID.
 *  - If the provided token has been deleted, the transaction will resolve to TOKEN_WAS_DELETED.
 *  - If an Association between the provided token and account is not found, the transaction will resolve to
 *    TOKEN_NOT_ASSOCIATED_TO_ACCOUNT.
 *  - If no Freeze Key is defined, the transaction will resolve to TOKEN_HAS_NO_FREEZE_KEY.
 *
 * Once executed the Account is marked as Unfrozen and will be able to receive or send tokens. The operation is
 * idempotent.
 *
 * Transaction Signing Requirements:
 *  - Freeze key.
 *  - Transaction fee payer account key.
 */
class TokenUnfreezeTransaction : public Transaction<TokenUnfreezeTransaction>
{
public:
  TokenUnfreezeTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a TokenUnfreeze transaction.
   */
  explicit TokenUnfreezeTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the account to be unfrozen for the specified token.
   *
   * @param accountId The ID of the account to be unfrozen for the specified token.
   * @return A reference to this TokenUnfreezeTransaction object with the newly-set account ID.
   * @throws IllegalStateException If this TokenUnfreezeTransaction is frozen.
   */
  TokenUnfreezeTransaction& setAccountId(const AccountId& accountId);

  /**
   * Set the ID of the token to be unfrozen for the specified account.
   *
   * @param tokenId The ID of the token to be unfrozen for the specified account.
   * @return A reference to this TokenUnfreezeTransaction object with the newly-set token ID.
   * @throws IllegalStateException If this TokenUnfreezeTransaction is frozen.
   */
  TokenUnfreezeTransaction& setTokenId(const TokenId& tokenId);

  /**
   * Get the ID of the account to be unfrozen for the specified token.
   *
   * @return The ID of the account to be unfrozen for the specified token.
   */
  [[nodiscard]] inline AccountId getAccountId() const { return mAccountId; }

  /**
   * Get the ID of the token to be unfrozen for the specified account.
   *
   * @return The ID of the token to be unfrozen for the specified account.
   */
  [[nodiscard]] inline TokenId getTokenId() const { return mTokenId; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TokenUnfreezeTransaction object.
   *
   * @param client The Client trying to construct this TokenUnfreezeTransaction.
   * @param node   The Node to which this TokenUnfreezeTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TokenUnfreezeTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   * TokenUnfreezeTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TokenUnfreezeTransaction to a Node.
   *
   * @param client   The Client submitting this TokenUnfreezeTransaction.
   * @param deadline The deadline for submitting this TokenUnfreezeTransaction.
   * @param node     Pointer to the Node to which this TokenUnfreezeTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the TokenUnfreezeTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a TokenUnfreezeAccountTransactionBody protobuf object from this TokenUnfreezeTransaction object.
   *
   * @return A pointer to a TokenUnfreezeAccountTransactionBody protobuf object filled with this
   *         TokenUnfreezeTransaction object's data.
   */
  [[nodiscard]] proto::TokenUnfreezeAccountTransactionBody* build() const;

  /**
   * The ID of the account to be unfrozen for the specified token.
   */
  AccountId mAccountId;

  /**
   * The ID of the token to be unfrozen for the specified account.
   */
  TokenId mTokenId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_UNFREEZE_TRANSACTION_H_
