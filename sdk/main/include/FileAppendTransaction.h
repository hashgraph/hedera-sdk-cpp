/*-
 *
 * Hedera Java SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#ifndef FILE_APPEND_TRANSACTION_H_
#define FILE_APPEND_TRANSACTION_H_

#include "ChunkedTransaction.h"
#include "FileId.h"

#include "helper/InitType.h"

#include "file_append.pb.h"

#include <string>

namespace Hedera
{
class AccountId;
class TransactionId;
}

namespace proto
{
class TransactionBody;
}

namespace Hedera
{
/**
 * Append the given contents to the end of the specified file. If a file is too
 * big to create with a single FileCreateTransaction, then it can be created
 * with the first part of its contents, and then appended as many times as
 * necessary to create the entire file. This transaction must be signed by all
 * initial M-of-M KeyList keys. If keys contains additional KeyList or
 * ThresholdKey then M-of-M secondary KeyList or ThresholdKey signing
 * requirements must be meet.
 */
class FileAppendTransaction : public ChunkedTransaction<FileAppendTransaction>
{
public:
  /**
   * Constructor.
   */
  FileAppendTransaction();

  /**
   * Construct from a map of transaction ID's to their corresponding account
   * ID's and protobuf transactions.
   *
   * @param transactions Map of transaction IDs to their corresponding account
   *                     ID's and protobuf transactions.
   */
  explicit FileAppendTransaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Construct from a protobuf transaction object.
   *
   * @param transaction The protobuf transaction object from which to construct
   *                    this transaction.
   */
  explicit FileAppendTransaction(const proto::TransactionBody& transaction);

  /**
   * Derived from ChunkedTransaction. Validate the checksums.
   *
   * @param client The client with which to validate the checksums.
   */
  virtual void validateChecksums(const Client& client) const override;

  /**
   * Build an ethereum transaction protobuf message based on the data in
   * this class.
   *
   * @return An ethereum transaction protobuf message.
   */
  proto::FileAppendTransactionBody build() const;

  /**
   * Set the ID of the file to which contents should be appended.
   *
   * @param fileId The ID of the file to which should be appended.
   * @return Reference to this FileAppendTransaction object.
   */
  FileAppendTransaction& setFileId(const FileId& fileId);

  /**
   * Set the contents to append to the file.
   *
   * @param contents The contents to append to the file.
   * @return Reference to this FileAppendTransaction object.
   */
  FileAppendTransaction& setContents(const std::string& contents);

  /**
   * Extract the file ID.
   *
   * @return The file ID.
   */
  inline InitType<FileId> getFileId() const { return mFileId; }

  /**
   * Extract the contents to append to the file.
   *
   * @return The contents to append to the file.
   */
  inline InitType<std::string> getContents() const { return mContents; }

private:
  /**
   * Initialize from the transaction body.
   */
  void initFromTransactionBody();

  /**
   * The file to which the bytes will be appended.
   */
  InitType<FileId> mFileId;

  /**
   * The bytes that will be appended to the end of the specified file.
   */
  std::string mContents;
};

} // namespace Hedera

#endif // FILE_APPEND_TRANSACTION_H_