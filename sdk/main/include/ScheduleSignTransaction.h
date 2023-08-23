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
#ifndef HEDERA_SDK_CPP_SCHEDULE_SIGN_TRANSACTION_H_
#define HEDERA_SDK_CPP_SCHEDULE_SIGN_TRANSACTION_H_

#include "ScheduleId.h"
#include "Transaction.h"

#include <optional>

namespace proto
{
class ScheduleSignTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * A transaction that appends signatures to a scheduled transaction. You will need to know the schedule ID to reference
 * the scheduled transaction to submit signatures. A record will be generated for each ScheduleSign transaction that is
 * successful and the scheduled entity will subsequently update with the public keys that have signed the scheduled
 * transaction. To view the keys that have signed the scheduled transaction, you can query the network for the schedule
 * info. Once a scheduled transaction receives the last required signature, the scheduled transaction executes.
 *
 * Transaction Signing Requirements:
 *  - The key of the account paying for the transaction.
 */
class ScheduleSignTransaction : public Transaction<ScheduleSignTransaction>
{
public:
  ScheduleSignTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a ScheduleSign.
   */
  explicit ScheduleSignTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the schedule to sign.
   *
   * @param scheduleId The ID of the schedule to sign.
   * @return A reference to this ScheduleSignTransaction object with the newly-set schedule ID.
   * @throws IllegalStateException If this ScheduleSignTransaction is frozen.
   */
  ScheduleSignTransaction& setScheduleId(const ScheduleId& scheduleId);

  /**
   * Clear the currently configured schedule ID.
   *
   * @return A reference to this ScheduleSignTransaction object with the newly-cleared schedule ID.
   * @throws IllegalStateException If this ScheduleSignTransaction is frozen.
   */
  ScheduleSignTransaction& clearScheduleId();

  /**
   * Get the ID of the schedule this ScheduleSignTransaction is currently configured to sign.
   *
   * @return The ID of the schedule this ScheduleSignTransaction is currently configured to sign. Uninitialized if no
   *         schedule ID has been set.
   */
  [[nodiscard]] inline std::optional<ScheduleId> getScheduleId() const { return mScheduleId; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this ScheduleSignTransaction object.
   *
   * @param client The Client trying to construct this ScheduleSignTransaction.
   * @param node   The Node to which this ScheduleSignTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this ScheduleSignTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                ScheduleSignTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this ScheduleSignTransaction to a Node.
   *
   * @param client   The Client submitting this ScheduleSignTransaction.
   * @param deadline The deadline for submitting this ScheduleSignTransaction.
   * @param node     Pointer to the Node to which this ScheduleSignTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the ScheduleSignTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a ScheduleSignTransactionBody protobuf object from this ScheduleSignTransaction object.
   *
   * @return A pointer to a ScheduleSignTransactionBody protobuf object filled with this ScheduleSignTransaction
   *         object's data.
   */
  [[nodiscard]] proto::ScheduleSignTransactionBody* build() const;

  /**
   * The ID of the schedule to sign.
   */
  std::optional<ScheduleId> mScheduleId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_SCHEDULE_SIGN_TRANSACTION_H_
