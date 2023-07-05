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
#ifndef HEDERA_SDK_CPP_TOKEN_DISSOCIATE_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOKEN_DISSOCIATE_TRANSACTION_H_

#include "AccountId.h"
#include "TokenId.h"
#include "Transaction.h"

#include <optional>
#include <vector>

namespace proto
{
class TokenDissociateTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Disassociates the provided Hedera account from the provided Hedera tokens. This transaction must be signed by the
 * provided account's key. Once the association is removed, no token related operation can be performed to that account.
 * AccountBalanceQuery and AccountInfoQuery will not return anything related to the token that was disassociated.
 *
 *  - If the provided account is not found, the transaction will resolve to INVALID_ACCOUNT_ID.
 *  - If the provided account has been deleted, the transaction will resolve to ACCOUNT_DELETED.
 *  - If any of the provided tokens is not found, the transaction will resolve to INVALID_TOKEN_REF.
 *  - If an association between the provided account and any of the tokens does not exist, the transaction will resolve
 *    to TOKEN_NOT_ASSOCIATED_TO_ACCOUNT.
 *  - If the provided account has a nonzero balance with any of the provided tokens, the transaction will resolve to
 *    TRANSACTION_REQUIRES_ZERO_TOKEN_BALANCES.
 *
 * On success, associations between the provided account and tokens are removed. The account is required to have a zero
 * balance of the token you wish to disassociate. If a token balance is present, you will receive a
 * TRANSACTION_REQUIRES_ZERO_TOKEN_BALANCES error.
 *
 * Transaction Signing Requirements:
 *  - The key of the account from which the token is being dissociated.
 *  - Transaction fee payer account key.
 */
class TokenDissociateTransaction : public Transaction<TokenDissociateTransaction>
{
public:
  TokenDissociateTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a TokenDissociate transaction.
   */
  explicit TokenDissociateTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the account to be dissociated from the provided tokens.
   *
   * @param accountId The ID of the account to be dissociated from the provided tokens.
   * @return A reference to this TokenDissociateTransaction object with the newly-set account ID.
   * @throws IllegalStateException If this TokenDissociateTransaction is frozen.
   */
  TokenDissociateTransaction& setAccountId(const AccountId& accountId);

  /**
   * Set the IDs of the tokens to be dissociated from the provided account.
   *
   * @param tokenIds The IDs of the tokens to be dissociated from the provided account.
   * @return A reference to this TokenDissociateTransaction object with the newly-set token IDs.
   * @throws IllegalStateException If this TokenDissociateTransaction is frozen.
   */
  TokenDissociateTransaction& setTokenIds(const std::vector<TokenId>& tokenIds);

  /**
   * Get the ID of the account to be dissociated from the provided tokens.
   *
   * @return The ID of the account to be dissociated from the provided tokens. Returns uninitialized if no account ID
   *         has been set.
   */
  [[nodiscard]] inline std::optional<AccountId> getAccountId() const { return mAccountId; }

  /**
   * Get the IDs of the tokens to be dissociated from the provided account.
   *
   * @return The IDs of the tokens to be dissociated from the provided account.
   */
  [[nodiscard]] inline std::vector<TokenId> getTokenIds() const { return mTokenIds; }

private:
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TokenDissociateTransaction object.
   *
   * @param client The Client trying to construct this TokenDissociateTransaction.
   * @param node   The Node to which this TokenDissociateTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TokenDissociateTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                TokenDissociateTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TokenDissociateTransaction to a Node.
   *
   * @param client   The Client submitting this TokenDissociateTransaction.
   * @param deadline The deadline for submitting this TokenDissociateTransaction.
   * @param node     Pointer to the Node to which this TokenDissociateTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;

  /**
   * Build a TokenDissociateTransactionBody protobuf object from this TokenDissociateTransaction object.
   *
   * @return A pointer to a TokenDissociateTransactionBody protobuf object filled with this TokenDissociateTransaction
   *         object's data.
   */
  [[nodiscard]] proto::TokenDissociateTransactionBody* build() const;

  /**
   * The ID of the account to be dissociated from the provided tokens.
   */
  std::optional<AccountId> mAccountId;

  /**
   * The IDs of the tokens to be dissociated from the provided account.
   */
  std::vector<TokenId> mTokenIds;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_DISSOCIATE_TRANSACTION_H_
