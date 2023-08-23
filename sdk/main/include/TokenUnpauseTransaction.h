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
#ifndef HEDERA_SDK_CPP_TOKEN_UNPAUSE_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOKEN_UNPAUSE_TRANSACTION_H_

#include "TokenId.h"
#include "Transaction.h"

namespace proto
{
class TokenUnpauseTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * A token unpause transaction is a transaction that unpauses the token that was previously disabled from participating
 * in transactions. The token's pause key is required to sign the transaction. Once the unpause transaction is submitted
 * the token pause status is updated to unpause.
 *
 * Transaction Signing Requirements:
 *  - The pause key of the token.
 *  - Transaction fee payer account key.
 */
class TokenUnpauseTransaction : public Transaction<TokenUnpauseTransaction>
{
public:
  TokenUnpauseTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a TokenUnpause transaction.
   */
  explicit TokenUnpauseTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the token to unpause.
   *
   * @param tokenId The ID of the token to unpause.
   * @return A reference to this TokenUnpauseTransaction object with the newly-set token ID.
   * @throws IllegalStateException If this TokenUnpauseTransaction is frozen.
   */
  TokenUnpauseTransaction& setTokenId(const TokenId& tokenId);

  /**
   * Get the ID of the token to unpause.
   *
   * @return The ID of the token to unpause.
   */
  [[nodiscard]] inline TokenId getTokenId() const { return mTokenId; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TokenUnpauseTransaction object.
   *
   * @param client The Client trying to construct this TokenUnpauseTransaction.
   * @param node   The Node to which this TokenUnpauseTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TokenUnpauseTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                TokenUnpauseTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TokenUnpauseTransaction to a Node.
   *
   * @param client   The Client submitting this TokenUnpauseTransaction.
   * @param deadline The deadline for submitting this TokenUnpauseTransaction.
   * @param node     Pointer to the Node to which this TokenUnpauseTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the TokenUnpauseTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a TokenUnpauseTransactionBody protobuf object from this TokenUnpauseTransaction object.
   *
   * @return A pointer to a TokenUnpauseTransactionBody protobuf object filled with this TokenUnpauseTransaction
   *         object's data.
   */
  [[nodiscard]] proto::TokenUnpauseTransactionBody* build() const;

  /**
   * The ID of the token to unpause.
   */
  TokenId mTokenId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_UNPAUSE_TRANSACTION_H_
