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
#ifndef HEDERA_SDK_CPP_SYSTEM_UNDELETE_TRANSACTION_H_
#define HEDERA_SDK_CPP_SYSTEM_UNDELETE_TRANSACTION_H_

#include "ContractId.h"
#include "FileId.h"
#include "Transaction.h"

#include <optional>

namespace proto
{
class SystemUndeleteTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Undelete a file or smart contract that was deleted by SystemDelete; requires a Hedera administrative multisignature.
 *
 * Transaction Signing Requirements:
 *  - The Hedera administrative multisignature.
 */
class SystemUndeleteTransaction : public Transaction<SystemUndeleteTransaction>
{
public:
  SystemUndeleteTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a SystemUndelete.
   */
  explicit SystemUndeleteTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the file to undelete. This is mutually exclusive with mContractId, and will reset the value of
   * mContractId if it is set.
   *
   * @param fileId The ID of the file to undelete.
   * @return A reference to this SystemUndeleteTransaction object with the newly-set file ID.
   * @throws IllegalStateException If this SystemUndeleteTransaction is frozen.
   */
  SystemUndeleteTransaction& setFileId(const FileId& fileId);

  /**
   * Set the ID of the contract to undelete. This is mutually exclusive with mFileId, and will reset the value of
   * mFileId if it is set.
   *
   * @param contractId The ID of the contract to undelete.
   * @return A reference to this SystemUndeleteTransaction object with the newly-set contract ID.
   * @throws IllegalStateException If this SystemUndeleteTransaction is frozen.
   */
  SystemUndeleteTransaction& setContractId(const ContractId& contractId);

  /**
   * Get the ID of the file this SystemUndeleteTransaction is currently configured to undelete.
   *
   * @return The ID of the file this SystemUndeleteTransaction is currently configured to undelete. Returns
   * uninitialized if a value has not yet been set, or if a contract ID has been set most recently.
   */
  [[nodiscard]] inline std::optional<FileId> getFileId() const { return mFileId; }

  /**
   * Get the ID of the contract this SystemUndeleteTransaction is currently configured to undelete.
   *
   * @return The ID of the contract this SystemUndeleteTransaction is currently configured to undelete. Returns
   *         uninitialized if a value has not yet been set, or if a file ID has been set most recently.
   */
  [[nodiscard]] inline std::optional<ContractId> getContractId() const { return mContractId; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this SystemUndeleteTransaction object.
   *
   * @param client The Client trying to construct this SystemUndeleteTransaction.
   * @param node   The Node to which this SystemUndeleteTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this SystemUndeleteTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                SystemUndeleteTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this SystemUndeleteTransaction to a Node.
   *
   * @param client   The Client submitting this SystemUndeleteTransaction.
   * @param deadline The deadline for submitting this SystemUndeleteTransaction.
   * @param node     Pointer to the Node to which this SystemUndeleteTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the SystemUndeleteTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a SystemUndeleteTransactionBody protobuf object from this SystemUndeleteTransaction object.
   *
   * @return A pointer to a SystemUndeleteTransactionBody protobuf object filled with this SystemUndeleteTransaction
   *         object's data.
   */
  [[nodiscard]] proto::SystemUndeleteTransactionBody* build() const;

  /**
   * The ID of the file to undelete. Mutually exclusive with mContractId.
   */
  std::optional<FileId> mFileId;

  /**
   * The ID of the contract to undelete. Mutually exclusive with mFileId.
   */
  std::optional<ContractId> mContractId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_SYSTEM_UNDELETE_TRANSACTION_H_