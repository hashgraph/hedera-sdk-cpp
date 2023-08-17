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
#ifndef HEDERA_SDK_CPP_TOKEN_FEE_SCHEDULE_UPDATE_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOKEN_FEE_SCHEDULE_UPDATE_TRANSACTION_H_

#include "CustomFee.h"
#include "TokenId.h"
#include "Transaction.h"

#include <memory>
#include <vector>

namespace proto
{
class TokenFeeScheduleUpdateTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Update the custom fees for a given token. If the token does not have a fee schedule, the network response returned
 * will be CUSTOM_SCHEDULE_ALREADY_HAS_NO_FEES. You will need to sign the transaction with the fee schedule key to
 * update the fee schedule for the token. If you do not have a fee schedule key set for the token, you will not be able
 * to update the fee schedule.
 *
 * Transaction Signing Requirements:
 *  - Fee schedule key.
 *  - Transaction fee payer account key.
 */
class TokenFeeScheduleUpdateTransaction : public Transaction<TokenFeeScheduleUpdateTransaction>
{
public:
  TokenFeeScheduleUpdateTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a TokenFeeScheduleUpdate transaction.
   */
  explicit TokenFeeScheduleUpdateTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the token whose fee schedule is to be updated.
   *
   * @param tokenId The ID of the token whose fee schedule is to be updated.
   * @return A reference to this TokenFeeScheduleUpdateTransaction object with the newly-set token ID.
   * @throws IllegalStateException If this TokenFeeScheduleUpdateTransaction is frozen.
   */
  TokenFeeScheduleUpdateTransaction& setTokenId(const TokenId& tokenId);

  /**
   * Set the new custom fees to be assessed during transfers of the token.
   *
   * @param fees The new custom fees to be assessed during transfers of the token.
   * @return A reference to this TokenFeeScheduleUpdateTransaction object with the newly-set fees.
   * @throws IllegalStateException If this TokenFeeScheduleUpdateTransaction is frozen.
   */
  TokenFeeScheduleUpdateTransaction& setCustomFees(const std::vector<std::shared_ptr<CustomFee>>& fees);

  /**
   * Get the ID of the token whose fee schedule is to be updated.
   *
   * @return The ID of the token whose fee schedule is to be updated.
   */
  [[nodiscard]] inline TokenId getTokenId() const { return mTokenId; }

  /**
   * Get the new custom fees to be assessed during transfers of the token.
   *
   * @return The new custom fees to be assessed during transfers of the token.
   */
  [[nodiscard]] inline std::vector<std::shared_ptr<CustomFee>> getCustomFees() const { return mCustomFees; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TokenFeeScheduleUpdateTransaction
   * object.
   *
   * @param client The Client trying to construct this TokenFeeScheduleUpdateTransaction.
   * @param node   The Node to which this TokenFeeScheduleUpdateTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TokenFeeScheduleUpdateTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                TokenFeeScheduleUpdateTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TokenFeeScheduleUpdateTransaction to a Node.
   *
   * @param client   The Client submitting this TokenFeeScheduleUpdateTransaction.
   * @param deadline The deadline for submitting this TokenFeeScheduleUpdateTransaction.
   * @param node     Pointer to the Node to which this TokenFeeScheduleUpdateTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the TokenFeeScheduleUpdateTransaction protobuf representation to the
   * Transaction protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a TokenFeeScheduleUpdateTransactionBody protobuf object from this TokenFeeScheduleUpdateTransaction object.
   *
   * @return A pointer to a TokenFeeScheduleUpdateTransactionBody protobuf object filled with this
   * TokenFeeScheduleUpdateTransaction object's data.
   */
  [[nodiscard]] proto::TokenFeeScheduleUpdateTransactionBody* build() const;

  /**
   * The ID of the token whose fee schedule is to be updated.
   */
  TokenId mTokenId;

  /**
   * The new custom fees to be assessed during transfers of the token.
   */
  std::vector<std::shared_ptr<CustomFee>> mCustomFees;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_FEE_SCHEDULE_UPDATE_TRANSACTION_H_
