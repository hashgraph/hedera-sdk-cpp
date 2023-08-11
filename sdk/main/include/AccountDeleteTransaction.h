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
#ifndef HEDERA_SDK_CPP_ACCOUNT_DELETE_TRANSACTION_H_
#define HEDERA_SDK_CPP_ACCOUNT_DELETE_TRANSACTION_H_

#include "AccountId.h"
#include "Transaction.h"

#include <optional>

namespace proto
{
class CryptoDeleteTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Mark an account as deleted, moving all its current hbars to another account. It will remain in the ledger, marked as
 * deleted, until it expires. Transfers into it a deleted account fail. But a deleted account can still have its
 * expiration extended in the normal way.
 */
class AccountDeleteTransaction : public Transaction<AccountDeleteTransaction>
{
public:
  AccountDeleteTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a CryptoDeleteAccount transaction.
   */
  explicit AccountDeleteTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Sets the ID of the account to delete.
   *
   * @param accountId The ID of the account to delete.
   * @return A reference to this AccountDeleteTransaction object with the newly-set account ID.
   * @throws IllegalStateException If this AccountDeleteTransaction is frozen.
   */
  AccountDeleteTransaction& setDeleteAccountId(const AccountId& accountId);

  /**
   * Set the ID of the account to which to transfer the deleted account's remaining Hbars.
   *
   * @param accountId The ID of the account to which to transfer the deleted account's remaining Hbars.
   * @return A reference to this AccountDeleteTransaction object with the newly-set transfer account ID.
   * @throws IllegalStateException If this AccountDeleteTransaction is frozen.
   */
  AccountDeleteTransaction& setTransferAccountId(const AccountId& accountId);

  /**
   * Get the ID of the account this AccountDeleteTransaction is currently configured to delete.
   *
   * @return The ID of the account this AccountDeleteTransaction is currently configured to delete. Uninitialized if no
   *         account ID has been set.
   */
  [[nodiscard]] inline std::optional<AccountId> getDeleteAccountId() const { return mDeleteAccountId; }

  /**
   * Get the ID of the account to which this AccountDeleteTransaction is currently configured to transfer the deleted
   * account's remaining Hbars.
   *
   * @return The ID of the account to which this AccountDeleteTransaction is currently configured to transfer the
   *         deleted account's remaining Hbars. Uninitialized if no account ID has been set.
   */
  [[nodiscard]] inline std::optional<AccountId> getTransferAccountId() const { return mTransferAccountId; }

private:
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this AccountDeleteTransaction object.
   *
   * @param client The Client trying to construct this AccountDeleteTransaction.
   * @param node   The Node to which this AccountDeleteTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this AccountDeleteTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                AccountDeleteTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this AccountDeleteTransaction to a Node.
   *
   * @param client   The Client submitting this AccountDeleteTransaction.
   * @param deadline The deadline for submitting this AccountDeleteTransaction.
   * @param node     Pointer to the Node to which this AccountDeleteTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the AccountCreateTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a CryptoDeleteTransactionBody protobuf object from this AccountDeleteTransaction object.
   *
   * @return A pointer to a CryptoDeleteTransactionBody protobuf object filled with this AccountDeleteTransaction
   *         object's data.
   */
  [[nodiscard]] proto::CryptoDeleteTransactionBody* build() const;

  /**
   * The ID of the account to delete.
   */
  std::optional<AccountId> mDeleteAccountId;

  /**
   * The ID of the account which will receive all the remaining Hbars from the deleted account.
   */
  std::optional<AccountId> mTransferAccountId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ACCOUNT_DELETE_TRANSACTION_H_
