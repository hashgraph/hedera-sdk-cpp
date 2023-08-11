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
#ifndef HEDERA_SDK_CPP_FILE_APPEND_TRANSACTION_H_
#define HEDERA_SDK_CPP_FILE_APPEND_TRANSACTION_H_

#include "ChunkedTransaction.h"
#include "FileId.h"

#include <cstddef>
#include <string_view>
#include <vector>

namespace proto
{
class FileAppendTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * A transaction that appends new file content to the end of an existing file. The contents of the file can be viewed by
 * submitting a FileContentsQuery request.
 *
 * Transaction Signing Requirements:
 *  - The key on the file is required to sign the transaction if different than the client operator account key.
 */
class FileAppendTransaction : public ChunkedTransaction<FileAppendTransaction>
{
public:
  /**
   * The default chunk size for a FileAppendTransaction.
   */
  static constexpr unsigned int DEFAULT_CHUNK_SIZE = 4096U;

  /**
   * Default constructor. Sets the maximum transaction fee to 5 Hbars, chunk size to 2048 bytes, and sets the receipt
   * retrieval policy to always retrieve receipts between chunk submissions.
   */
  FileAppendTransaction();

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a FileAppend transaction.
   */
  explicit FileAppendTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the file to which to append.
   *
   * @param fileId The ID of the file to which to append.
   * @return A reference to this FileAppendTransaction object with the newly-set file ID.
   * @throws IllegalStateException If this FileAppendTransaction is frozen.
   */
  FileAppendTransaction& setFileId(const FileId& fileId);

  /**
   * Set the contents to append.
   *
   * @param contents The contents of the new file.
   * @return A reference to this FileAppendTransaction object with the newly-set contents.
   * @throws IllegalStateException If this FileAppendTransaction is frozen.
   */
  FileAppendTransaction& setContents(const std::vector<std::byte>& contents);
  FileAppendTransaction& setContents(std::string_view contents);

  /**
   * Get the ID of the file to which to append.
   *
   * @return The ID of the file to which to append.
   */
  [[nodiscard]] inline FileId getFileId() const { return mFileId; }

  /**
   * Get the contents to append.
   *
   * @return The contents to append.
   */
  [[nodiscard]] inline std::vector<std::byte> getContents() const { return getData(); }

private:
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this FileAppendTransaction object.
   *
   * @param client The Client trying to construct this FileAppendTransaction.
   * @param node   The Node to which this FileAppendTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this FileAppendTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                FileAppendTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this FileAppendTransaction to a Node.
   *
   * @param client   The Client submitting this FileAppendTransaction.
   * @param deadline The deadline for submitting this FileAppendTransaction.
   * @param node     Pointer to the Node to which this FileAppendTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the FileAppendTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a FileAppendTransactionBody protobuf object from this FileAppendTransaction object.
   *
   * @return A pointer to a FileAppendTransactionBody protobuf object filled with this FileAppendTransaction object's
   *         data.
   */
  [[nodiscard]] proto::FileAppendTransactionBody* build() const;

  /**
   * The ID of the file to which to append.
   */
  FileId mFileId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_FILE_APPEND_TRANSACTION_H_
