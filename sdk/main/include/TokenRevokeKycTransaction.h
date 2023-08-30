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
#ifndef HEDERA_SDK_CPP_TOKEN_REVOKE_KYC_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOKEN_REVOKE_KYC_TRANSACTION_H_

#include "AccountId.h"
#include "TokenId.h"
#include "Transaction.h"

#include <optional>
#include <vector>

namespace proto
{
class TokenRevokeKycTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Revokes the KYC flag to the Hedera account for the given Hedera token. This transaction must be signed by the token's
 * KYC Key. If this key is not set, you can submit a TokenUpdateTransaction to provide the token with this key.
 *
 *  - If the provided account is not found, the transaction will resolve to INVALID_ACCOUNT_ID.
 *  - If the provided account has been deleted, the transaction will resolve to ACCOUNT_DELETED.
 *  - If the provided token is not found, the transaction will resolve to INVALID_TOKEN_ID.
 *  - If the provided token has been deleted, the transaction will resolve to TOKEN_WAS_DELETED.
 *  - If an Association between the provided token and account is not found, the transaction will resolve to
 *    TOKEN_NOT_ASSOCIATED_TO_ACCOUNT.
 *  - If no KYC Key is defined, the transaction will resolve to TOKEN_HAS_NO_KYC_KEY.
 *
 * Once executed, the account is marked as KYC Revoked.
 *
 * Transaction Signing Requirements:
 *  - KYC key.
 *  - Transaction fee payer account key.
 */
class TokenRevokeKycTransaction : public Transaction<TokenRevokeKycTransaction>
{
public:
  TokenRevokeKycTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a TokenRevokeKyc transaction.
   */
  explicit TokenRevokeKycTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the account to have its KYC revoked for this token.
   *
   * @param accountId The ID of the account to have its KYC revoked for this token.
   * @return A reference to this TokenRevokeKycTransaction object with the newly-set account ID.
   * @throws IllegalStateException If this TokenRevokeKycTransaction is frozen.
   */
  TokenRevokeKycTransaction& setAccountId(const AccountId& accountId);

  /**
   * Set the ID of the token for which the account has revoked KYC.
   *
   * @param tokenId The ID of the token for which the account has revoked KYC.
   * @return A reference to this TokenRevokeKycTransaction object with the newly-set token ID.
   * @throws IllegalStateException If this TokenRevokeKycTransaction is frozen.
   */
  TokenRevokeKycTransaction& setTokenId(const TokenId& tokenId);

  /**
   * Get the ID of the account to have its KYC revoked for this token.
   *
   * @return The ID of the account to have its KYC revoked for this token.
   */
  [[nodiscard]] inline AccountId getAccountId() const { return mAccountId; }

  /**
   * Get the ID of the token for which the account has revoked KYC.
   *
   * @return The ID of the token for which the account has revoked KYC.
   */
  [[nodiscard]] inline TokenId getTokenId() const { return mTokenId; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TokenRevokeKycTransaction object.
   *
   * @param client The Client trying to construct this TokenRevokeKycTransaction.
   * @param node   The Node to which this TokenRevokeKycTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TokenRevokeKycTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                TokenRevokeKycTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TokenRevokeKycTransaction to a Node.
   *
   * @param client   The Client submitting this TokenRevokeKycTransaction.
   * @param deadline The deadline for submitting this TokenRevokeKycTransaction.
   * @param node     Pointer to the Node to which this TokenRevokeKycTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the TokenRevokeKycTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a TokenRevokeKycTransactionBody protobuf object from this TokenRevokeKycTransaction object.
   *
   * @return A pointer to a TokenRevokeKycTransactionBody protobuf object filled with this TokenRevokeKycTransaction
   *         object's data.
   */
  [[nodiscard]] proto::TokenRevokeKycTransactionBody* build() const;

  /**
   * The ID of the account to have its KYC revoked for this token.
   */
  AccountId mAccountId;

  /**
   * The ID of the token for which the account has revoked KYC.
   */
  TokenId mTokenId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_REVOKE_KYC_TRANSACTION_H_
