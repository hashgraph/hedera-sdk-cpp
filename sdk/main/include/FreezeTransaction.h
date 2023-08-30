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
#ifndef HEDERA_SDK_CPP_FREEZE_TRANSACTION_H_
#define HEDERA_SDK_CPP_FREEZE_TRANSACTION_H_

#include "FileId.h"
#include "FreezeType.h"
#include "Transaction.h"

#include <chrono>
#include <cstddef>
#include <optional>
#include <vector>

namespace proto
{
class FreezeTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 *
 * At consensus, sets the consensus time at which the platform should stop creating events and accepting transactions,
 * and enter a maintenance window.
 *
 * Transaction Signing Requirements:
 *  - The key(s) on the file are required to sign the transaction.
 */
class FreezeTransaction : public Transaction<FreezeTransaction>
{
public:
  FreezeTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a Freeze transaction.
   */
  explicit FreezeTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the file to use for the network software upgrade.
   *
   * @param fileId The ID of the file to use for the network software upgrade.
   * @return A reference to this FreezeTransaction object with the newly-set file ID.
   * @throws IllegalStateException If this FreezeTransaction is frozen.
   */
  FreezeTransaction& setFileId(const FileId& fileId);

  /**
   * Set the expected hash of the contents of the update file.
   *
   * @param hash The expected hash of the contents of the update file.
   * @return A reference to this FreezeTransaction object with the newly-set file hash.
   * @throws IllegalStateException If this FreezeTransaction is frozen.
   */
  FreezeTransaction& setFileHash(const std::vector<std::byte>& hash);

  /**
   * Set the consensus time at which the maintenance window should begin.
   *
   * @param time The consensus time at which the maintenance window should begin.
   * @return A reference to this FreezeTransaction object with the newly-set start time.
   * @throws IllegalStateException If this FreezeTransaction is frozen.
   */
  FreezeTransaction& setStartTime(const std::chrono::system_clock::time_point& time);

  /**
   * Set the type of network freeze or upgrade operation to perform.
   *
   * @param type The type of network freeze or upgrade operation to perform.
   * @return A reference to this FreezeTransaction object with the newly-set freeze type.
   * @throws IllegalStateException If this FreezeTransaction is frozen.
   */
  FreezeTransaction& setFreezeType(const FreezeType& type);

  /**
   * Get the ID of the file to use for the network software upgrade.
   *
   * @return The ID of the file to use for the network software upgrade. Returns uninitialized if the file ID has not
   *         been set.
   */
  [[nodiscard]] inline std::optional<FileId> getFileId() const { return mFileId; }

  /**
   * Get the expected hash of the contents of the update file.
   *
   * @return The expected hash of the contents of the update file.
   */
  [[nodiscard]] inline std::vector<std::byte> getFileHash() const { return mFileHash; }

  /**
   * Get the consensus time at which the maintenance window should begin.
   *
   * @return The consensus time at which the maintenance window should begin.Returns uninitialized if the start time has
   *         not been set.
   */
  [[nodiscard]] inline std::optional<std::chrono::system_clock::time_point> getStartTime() const { return mStartTime; }

  /**
   * Get the type of network freeze or upgrade operation to perform.
   *
   * @return The type of network freeze or upgrade operation to perform.
   */
  [[nodiscard]] inline FreezeType getFreezeType() const { return mFreezeType; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this FreezeTransaction object.
   *
   * @param client The Client trying to construct this FreezeTransaction.
   * @param node   The Node to which this FreezeTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this FreezeTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                FreezeTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this FreezeTransaction to a Node.
   *
   * @param client   The Client submitting this FreezeTransaction.
   * @param deadline The deadline for submitting this FreezeTransaction.
   * @param node     Pointer to the Node to which this FreezeTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the FreezeTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a FreezeTransactionBody protobuf object from this FreezeTransaction object.
   *
   * @return A pointer to a FreezeTransactionBody protobuf object filled with this FreezeTransaction object's
   *         data.
   */
  [[nodiscard]] proto::FreezeTransactionBody* build() const;

  /**
   * The ID of the file to use for the network software upgrade.
   */
  std::optional<FileId> mFileId;

  /**
   * The expected hash of the contents of the update file.
   */
  std::vector<std::byte> mFileHash;

  /**
   * The consensus time at which the maintenance window should begin.
   */
  std::optional<std::chrono::system_clock::time_point> mStartTime;

  /**
   * The type of network freeze or upgrade operation to perform.
   */
  FreezeType mFreezeType = FreezeType::UNKNOWN_FREEZE_TYPE;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_FREEZE_TRANSACTION_H_
