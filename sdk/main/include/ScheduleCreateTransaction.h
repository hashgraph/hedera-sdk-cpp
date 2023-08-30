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
#ifndef HEDERA_SDK_CPP_SCHEDULE_CREATE_TRANSACTION_H_
#define HEDERA_SDK_CPP_SCHEDULE_CREATE_TRANSACTION_H_

#include "AccountId.h"
#include "Key.h"
#include "Transaction.h"

#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace proto
{
class SchedulableTransactionBody;
class ScheduleCreateTransactionBody;
class TransactionBody;
class WrappedTransaction;
}

namespace Hedera
{
/**
 * A transaction that creates a schedule entity on a Hedera network. The entity ID for a schedule transaction is
 * referred to as the ScheduleId. After successfully executing a schedule create transaction, you can obtain the
 * ScheduleId by requesting the receipt of the transaction immediately after the transaction was executed. The receipt
 * also contains the scheduled transaction ID. The scheduled transaction ID is used to to request the record of the
 * scheduled transaction if it is successfully executed.
 *
 * If two users submit the same schedule create transaction, the first one to reach consensus will create the schedule
 * ID and the second one will have the schedule ID returned in the receipt of the transaction. The receipt status of the
 * second identical schedule transaction will return an "IDENTICAL_SCHEDULE_ALREADY_CREATED" response from the network.
 * The user who submits the second transaction would need to submit a ScheduleSign transaction to add their signature to
 * the schedule transaction.
 *
 * To retain the ability to delete a schedule transaction, you will need to populate the admin key field when creating a
 * schedule transaction. The admin key will be required to sign the ScheduleDelete transaction to delete the scheduled
 * transaction from the network. If you do not assign an admin key during the creation of the schedule transaction, you
 * will have an immutable schedule transaction.
 *
 * Transaction Signing Requirements:
 *  - The key of the account paying for the creation of the schedule transaction.
 *  - The key of the payer account ID paying for the execution of the scheduled transaction.
 *  - If the payer account is not specified, the operator account will be used to pay for the execution by default.
 *  - The admin key if set.
 *  - You can optionally sign with any of the required signatures for the scheduled (inner) transaction. Freeze the
 *    schedule transaction and call the sign() method to add signatures.
 */
class ScheduleCreateTransaction : public Transaction<ScheduleCreateTransaction>
{
public:
  ScheduleCreateTransaction();
  ~ScheduleCreateTransaction() override;
  ScheduleCreateTransaction(const ScheduleCreateTransaction&);
  ScheduleCreateTransaction& operator=(const ScheduleCreateTransaction&);
  ScheduleCreateTransaction(ScheduleCreateTransaction&&) noexcept;
  ScheduleCreateTransaction& operator=(ScheduleCreateTransaction&&) noexcept;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a ScheduleCreate transaction.
   */
  explicit ScheduleCreateTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the Transaction to schedule.
   *
   * @param transaction The Transaction to schedule.
   * @return A reference to this ScheduleCreateTransaction object with the newly-set scheduled transaction.
   * @throws IllegalStateException  If this ScheduleCreateTransaction is frozen.
   * @throws UninitializedException If the input WrappedTransaction does not contain a Transaction.
   */
  ScheduleCreateTransaction& setScheduledTransaction(const WrappedTransaction& transaction);

  /**
   * Set the memo of the schedule entity.
   *
   * @param memo The memo of the schedule entity.
   * @return A reference to this ScheduleCreateTransaction object with the newly-set memo.
   */
  ScheduleCreateTransaction& setScheduleMemo(std::string_view memo);

  /**
   * Set the admin key of the schedule entity.
   *
   * @param key The admin key of the schedule entity.
   * @return A reference to this ScheduleCreateTransaction object with the newly-set admin key.
   */
  ScheduleCreateTransaction& setAdminKey(const std::shared_ptr<Key>& key);

  /**
   * Set the ID of the account that will pay for the scheduled transaction when it executes. Defaults to the payer of
   * this ScheduleCreateTransaction if not provided.
   *
   * @param accountId The ID of the account that will pay for the scheduled transaction when it executes.
   * @return A reference to this ScheduleCreateTransaction object with the newly-set payer account ID.
   */
  ScheduleCreateTransaction& setPayerAccountId(const AccountId& accountId);

  /**
   * Set the expiration time of the schedule entity.
   *
   * @param time The expiration time of the schedule entity.
   * @return A reference to this ScheduleCreateTransaction object with the newly-set expiration time.
   */
  ScheduleCreateTransaction& setExpirationTime(const std::chrono::system_clock::time_point& time);

  /**
   * Set the schedule entity's wait-for-expiry policy.
   *
   * @param wait \c TRUE if the transaction should be executed at time of expiration, \c FALSE if it should execute when
   *             it receives all signatures.
   * @return A reference to this ScheduleCreateTransaction object with the newly-set expiration time.
   */
  ScheduleCreateTransaction& setWaitForExpiry(bool wait);

  /**
   * Get the Transaction (wrapped) to schedule.
   *
   * @return The Transaction (wrapped) to schedule.
   */
  [[nodiscard]] WrappedTransaction getScheduledTransaction() const;

  /**
   * Get the memo of the schedule entity.
   *
   * @return The memo of the schedule entity.
   */
  [[nodiscard]] std::string getScheduleMemo() const;

  /**
   * Get the admin key of the schedule entity.
   *
   * @return The admin key of the schedule entity.
   */
  [[nodiscard]] std::shared_ptr<Key> getAdminKey() const;

  /**
   * Get the ID of the account that will pay for the scheduled transaction when it executes.
   *
   * @return The ID of the account that will pay for the scheduled transaction when it executes. Returns uninitialized
   *         if no payer account ID has been set.
   */
  [[nodiscard]] std::optional<AccountId> getPayerAccountId() const;

  /**
   * Get the expiration time of the schedule entity.
   *
   * @return The expiration time of the schedule entity. Returns uninitialized if no expiration time has been set.
   */
  [[nodiscard]] std::optional<std::chrono::system_clock::time_point> getExpirationTime() const;

  /**
   * Get the schedule entity's wait-for-expiry policy.
   *
   * @return The schedule entity's wait-for-expiry policy. Returns \c TRUE if the transaction will be executed at time
   *         of expiration, \c FALSE if it will execute when it receives all signatures.
   */
  [[nodiscard]] bool isWaitForExpiry() const;

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this ScheduleCreateTransaction object.
   *
   * @param client The Client trying to construct this ScheduleCreateTransaction.
   * @param node   The Node to which this ScheduleCreateTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this ScheduleCreateTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                ScheduleCreateTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this ScheduleCreateTransaction to a Node.
   *
   * @param client   The Client submitting this ScheduleCreateTransaction.
   * @param deadline The deadline for submitting this ScheduleCreateTransaction.
   * @param node     Pointer to the Node to which this ScheduleCreateTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the ScheduleCreateTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a ScheduleCreateTransactionBody protobuf object from this ScheduleCreateTransaction object.
   *
   * @return A pointer to a ScheduleCreateTransactionBody protobuf object filled with this ScheduleCreateTransaction
   *         object's data.
   */
  [[nodiscard]] proto::ScheduleCreateTransactionBody* build() const;

  /**
   * Initialize this ScheduleCreateTransaction's implementation object.
   */
  void initImpl();

  /**
   * Implementation object used to hide implementation details and internal headers.
   */
  struct ScheduleCreateTransactionImpl;
  std::unique_ptr<ScheduleCreateTransactionImpl> mImpl;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_SCHEDULE_CREATE_TRANSACTION_H_
