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
#ifndef HEDERA_SDK_CPP_FILE_DELETE_TRANSACTION_H_
#define HEDERA_SDK_CPP_FILE_DELETE_TRANSACTION_H_

#include "FileId.h"
#include "Transaction.h"

namespace proto
{
class FileDeleteTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * A transaction that deletes a file from a Hedera network. When deleted, a file's contents are truncated to zero length
 * and it can no longer be updated or appended to, or its expiration time extended. When you request the contents or
 * info of a deleted file, the network will return FILE_DELETED.
 *
 * Transaction Signing Requirements:
 *  - The key(s) on the file are required to sign the transaction.
 *  - If you do not sign with the key(s) on the file, you will receive an INVALID_SIGNATURE network error.
 */
class FileDeleteTransaction : public Transaction<FileDeleteTransaction>
{
public:
  FileDeleteTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a FileDelete transaction.
   */
  explicit FileDeleteTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the file to delete.
   *
   * @param fileId The ID of the file to delete.
   * @return A reference to this FileDeleteTransaction object with the newly-set file ID.
   * @throws IllegalStateException If this FileDeleteTransaction is frozen.
   */
  FileDeleteTransaction& setFileId(const FileId& fileId);

  /**
   * Get the ID of the file to delete.
   *
   * @return The ID of the file to delete.
   */
  [[nodiscard]] inline FileId getFileId() const { return mFileId; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this FileDeleteTransaction object.
   *
   * @param client The Client trying to construct this FileDeleteTransaction.
   * @param node   The Node to which this FileDeleteTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this FileCreateTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                FileDeleteTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this FileDeleteTransaction to a Node.
   *
   * @param client   The Client submitting this FileDeleteTransaction.
   * @param deadline The deadline for submitting this FileDeleteTransaction.
   * @param node     Pointer to the Node to which this FileDeleteTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the FileDeleteTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a FileDeleteTransactionBody protobuf object from this FileDeleteTransaction object.
   *
   * @return A pointer to a FileDeleteTransactionBody protobuf object filled with this FileDeleteTransaction object's
   *         data.
   */
  [[nodiscard]] proto::FileDeleteTransactionBody* build() const;

  /**
   * The ID of the file to delete.
   */
  FileId mFileId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_FILE_DELETE_TRANSACTION_H_
