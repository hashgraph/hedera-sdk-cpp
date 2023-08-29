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
#ifndef HEDERA_SDK_CPP_TOKEN_ASSOCIATE_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOKEN_ASSOCIATE_TRANSACTION_H_

#include "AccountId.h"
#include "TokenId.h"
#include "Transaction.h"

#include <optional>
#include <vector>

namespace proto
{
class TokenAssociateTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Associates the provided Hedera account with the provided Hedera token(s). Hedera accounts must be associated with a
 * fungible or non-fungible token first before you can transfer tokens to that account. When you transfer a custom
 * fungible or non-fungible token to the alias account ID, the token association step is skipped and the account will
 * automatically be associated with the token upon creation. In the case of a NON_FUNGIBLE token type, once an account
 * is associated, it can hold any number of NFTs (serial numbers) of that token type. The Hedera account that is
 * associated with a token is required to sign the transaction.
 *
 *  - If the provided account is not found, the transaction will resolve to INVALID_ACCOUNT_ID.
 *  - If the provided account has been deleted, the transaction will resolve to ACCOUNT_DELETED.
 *  - If any of the provided tokens is not found, the transaction will resolve to INVALID_TOKEN_REF.
 *  - If any of the provided tokens has been deleted, the transaction will resolve to TOKEN_WAS_DELETED.
 *  - If an association between the provided account and any of the tokens already exists, the transaction will resolve
 *    to TOKEN_ALREADY_ASSOCIATED_TO_ACCOUNT.
 *  - If the provided account's associations count exceeds the constraint of maximum token associations per account, the
 *    transaction will resolve to TOKENS_PER_ACCOUNT_LIMIT_EXCEEDED.
 *  - On success, associations between the provided account and tokens are made and the account is ready to interact
 *    with the tokens.
 *
 * There is currently no limit on the number of token IDs that can be associated with an account (reference HIP-367).
 * Still, you can see TOKENS_PER_ACCOUNT_LIMIT_EXCEEDED responses for pre-HIP-367 transactions.
 *
 * Transaction Signing Requirements:
 *  - The key of the account to which the token is being associated
 *  - Transaction fee payer account key
 */
class TokenAssociateTransaction : public Transaction<TokenAssociateTransaction>
{
public:
  TokenAssociateTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a TokenAssociate transaction.
   */
  explicit TokenAssociateTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the account to be associated with the provided tokens.
   *
   * @param accountId The ID of the account to be associated.
   * @return A reference to this TokenAssociateTransaction object with the newly-set account ID.
   * @throws IllegalStateException If this TokenAssociateTransaction is frozen.
   */
  TokenAssociateTransaction& setAccountId(const AccountId& accountId);

  /**
   * Set the IDs of the tokens to be associated with the provided account.
   *
   * @param tokenIds The IDs of the tokens to be associated.
   * @return A reference to this TokenAssociateTransaction object with the newly-set token IDs.
   * @throws IllegalStateException If this TokenAssociateTransaction is frozen.
   */
  TokenAssociateTransaction& setTokenIds(const std::vector<TokenId>& tokenIds);

  /**
   * Get the ID of the account to be associated with the provided tokens.
   *
   * @return The ID of the account to be associated with the provided tokens. Returns uninitialized if no account ID has
   *         been set.
   */
  [[nodiscard]] inline std::optional<AccountId> getAccountId() const { return mAccountId; }

  /**
   * Get the IDs of the tokens to be associated with the provided account.
   *
   * @return The IDs of the tokens to be associated with the provided account.
   */
  [[nodiscard]] inline std::vector<TokenId> getTokenIds() const { return mTokenIds; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TokenAssociateTransaction object.
   *
   * @param client The Client trying to construct this TokenAssociateTransaction.
   * @param node   The Node to which this TokenAssociateTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TokenAssociateTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                TokenAssociateTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TokenAssociateTransaction to a Node.
   *
   * @param client   The Client submitting this TokenAssociateTransaction.
   * @param deadline The deadline for submitting this TokenAssociateTransaction.
   * @param node     Pointer to the Node to which this TokenAssociateTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the TokenAssociateTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a TokenAssociateTransactionBody protobuf object from this TokenAssociateTransaction object.
   *
   * @return A pointer to a TokenAssociateTransactionBody protobuf object filled with this TokenAssociateTransaction
   *         object's data.
   */
  [[nodiscard]] proto::TokenAssociateTransactionBody* build() const;

  /**
   * The ID of the account to be associated with the provided tokens.
   */
  std::optional<AccountId> mAccountId;

  /**
   * The IDs of the tokens to be associated with the provided account.
   */
  std::vector<TokenId> mTokenIds;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_ASSOCIATE_TRANSACTION_H_
