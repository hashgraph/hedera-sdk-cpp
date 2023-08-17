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
#ifndef HEDERA_SDK_CPP_TOKEN_DELETE_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOKEN_DELETE_TRANSACTION_H_

#include "TokenId.h"
#include "Transaction.h"

#include <optional>

namespace proto
{
class TokenDeleteTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Deleting a token marks a token as deleted, though it will remain in the ledger. The operation must be signed by the
 * specified admin key of the token. If the admin key is not set, the transaction will result in TOKEN_IS_IMMUTABlE.
 * Once deleted, update, mint, burn, wipe, freeze, unfreeze, grant KYC, revoke KYC and token transfer transactions will
 * resolve to TOKEN_WAS_DELETED.
 *
 * For NFTs, you cannot delete a specific NFT. You can delete the class of the NFT specified by the token ID after you
 * have burned all associated NFTs associated with the token class
 *
 * Transaction Signing Requirements:
 *  - Admin key
 *  - Transaction fee payer account key
 */
class TokenDeleteTransaction : public Transaction<TokenDeleteTransaction>
{
public:
  TokenDeleteTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a TokenDelete transaction.
   */
  explicit TokenDeleteTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the token to delete.
   *
   * @param tokenId The ID of the token to delete.
   * @return A reference to this TokenDeleteTransaction object with the newly-set token ID.
   * @throws IllegalStateException If this TokenDeleteTransaction is frozen.
   */
  TokenDeleteTransaction& setTokenId(const TokenId& tokenId);

  /**
   * Get the ID of the token this TokenDeleteTransaction is currently configured to delete.
   *
   * @return The ID of the account this TokenDeleteTransaction is currently configured to delete. Uninitialized if no
   *         token ID has been set.
   */
  [[nodiscard]] inline std::optional<TokenId> getTokenId() const { return mTokenId; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TokenDeleteTransaction object.
   *
   * @param client The Client trying to construct this TokenDeleteTransaction.
   * @param node   The Node to which this TokenDeleteTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TokenDeleteTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this TokenDeleteTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TokenDeleteTransaction to a Node.
   *
   * @param client   The Client submitting this TokenDeleteTransaction.
   * @param deadline The deadline for submitting this TokenDeleteTransaction.
   * @param node     Pointer to the Node to which this TokenDeleteTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the TokenDeleteTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a TokenDeleteTransactionBody protobuf object from this TokenDeleteTransaction object.
   *
   * @return A pointer to a TokenDeleteTransactionBody protobuf object filled with this TokenDeleteTransaction
   *         object's data.
   */
  [[nodiscard]] proto::TokenDeleteTransactionBody* build() const;

  /**
   * The ID of the token to delete.
   */
  std::optional<TokenId> mTokenId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_DELETE_TRANSACTION_H_
