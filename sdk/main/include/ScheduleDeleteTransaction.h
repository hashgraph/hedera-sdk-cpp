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
#ifndef HEDERA_SDK_CPP_SCHEDULE_DELETE_TRANSACTION_H_
#define HEDERA_SDK_CPP_SCHEDULE_DELETE_TRANSACTION_H_

#include "ScheduleId.h"
#include "Transaction.h"

#include <optional>

namespace proto
{
class ScheduleDeleteTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * A transaction that deletes a scheduled transaction from the network. You can delete a scheduled transaction if only
 * the admin key was set during the creation of the scheduled transaction. If an admin key was not set, the attempted
 * deletion will result in "SCHEDULE_IS_IMMUTABLE" response from the network. Once the scheduled transaction is deleted,
 * the scheduled transaction will be marked as deleted with the consensus timestamp the scheduled transaction was
 * deleted at.
 *
 * Transaction Signing Requirements:
 *  - The admin key of the scheduled transaction.
 */
class ScheduleDeleteTransaction : public Transaction<ScheduleDeleteTransaction>
{
public:
  ScheduleDeleteTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a ScheduleDelete.
   */
  explicit ScheduleDeleteTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the schedule to delete.
   *
   * @param scheduleId The ID of the schedule to delete.
   * @return A reference to this ScheduleDeleteTransaction object with the newly-set schedule ID.
   * @throws IllegalStateException If this ScheduleDeleteTransaction is frozen.
   */
  ScheduleDeleteTransaction& setScheduleId(const ScheduleId& scheduleId);

  /**
   * Get the ID of the schedule this ScheduleDeleteTransaction is currently configured to delete.
   *
   * @return The ID of the schedule this ScheduleDeleteTransaction is currently configured to delete. Uninitialized if
   *         no schedule ID has been set.
   */
  [[nodiscard]] inline std::optional<ScheduleId> getScheduleId() const { return mScheduleId; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this ScheduleDeleteTransaction object.
   *
   * @param client The Client trying to construct this ScheduleDeleteTransaction.
   * @param node   The Node to which this ScheduleDeleteTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this ScheduleDeleteTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                ScheduleDeleteTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this ScheduleDeleteTransaction to a Node.
   *
   * @param client   The Client submitting this ScheduleDeleteTransaction.
   * @param deadline The deadline for submitting this ScheduleDeleteTransaction.
   * @param node     Pointer to the Node to which this ScheduleDeleteTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the ScheduleDeleteTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a ScheduleDeleteTransactionBody protobuf object from this ScheduleDeleteTransaction object.
   *
   * @return A pointer to a ScheduleDeleteTransactionBody protobuf object filled with this ScheduleDeleteTransaction
   *         object's data.
   */
  [[nodiscard]] proto::ScheduleDeleteTransactionBody* build() const;

  /**
   * The ID of the schedule to delete.
   */
  std::optional<ScheduleId> mScheduleId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_SCHEDULE_DELETE_TRANSACTION_H_
