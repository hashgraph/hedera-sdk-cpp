/*-
 *
 * Hedera C++ SDK
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
#ifndef FILE_CREATE_TRANSACTION_H_
#define FILE_CREATE_TRANSACTION_H_

#include "PublicKey.h"
#include "KeyList.h"
#include "Transaction.h"

#include "helper/InitType.h"

#include <proto/file_create.pb.h>

#include <chrono>
#include <unordered_map>

namespace Hedera
{
class AccountId;
class FileId;
class TransactionId;
}

namespace proto
{
class TransactionBody;
}

namespace Hedera
{
/**
 * Create a new file, containing the given contents.
 * After the file is created, the FileID for it can be found in the receipt, or
 * record, or retrieved with a GetByKey query.
 *
 * The file contains the specified contents (possibly empty). The file will
 * automatically disappear at the expirationTime, unless its expiration is
 * extended by another transaction before that time. If the file is deleted,
 * then its contents will become empty and it will be marked as deleted until it
 * expires, and then it will cease to exist.
 *
 * The keys field is a list of keys. All keys within the top-level key list must
 * sign (M-M) to create or modify a file. However, to delete the file, only one
 * key (1-M) is required to sign from the top-level key list.  Each of those
 * "keys" may itself be threshold key containing other keys (including other
 * threshold keys). In other words, the behavior is an AND for create/modify, OR
 * for delete. This is useful for acting as a revocation server. If it is
 * desired to have the behavior be AND for all 3 operations (or OR for all 3),
 * then the list should have only a single Key, which is a threshold key, with
 * N=1 for OR, N=M for AND.
 *
 * If a file is created without ANY keys in the keys field, the file is
 * immutable and ONLY the expirationTime of the file can be changed with a
 * FileUpdate transaction. The file contents or its keys cannot be changed.
 *
 * An entity (account, file, or smart contract instance) must be created in a
 * particular realm. If the realmID is left null, then a new realm will be
 * created with the given admin key. If a new realm has a null adminKey, then
 * anyone can create/modify/delete entities in that realm. But if an admin key
 * is given, then any transaction to create/modify/delete an entity in that
 * realm must be signed by that key, though anyone can still call functions on
 * smart contract instances that exist in that realm. A realm ceases to exist
 * when everything within it has expired and no longer exists.
 *
 * The current API ignores shardID, realmID, and newRealmAdminKey, and creates
 * everything in shard 0 and realm 0, with a null key. Future versions of the
 * API will support multiple realms and multiple shards.
 */
class FileCreateTransaction : public Transaction<FileCreateTransaction>
{
public:
  /**
   * Constructor.
   */
  FileCreateTransaction();

  /**
   * Construct from a map of transaction ID's to their corresponding account
   * ID's and protobuf transactions.
   *
   * @param transactions Map of transaction IDs to their corresponding account
   *                     ID's and protobuf transactions.
   */
  explicit FileCreateTransaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Construct from a protobuf transaction object.
   *
   * @param transaction The protobuf transaction object from which to construct
   *                    this transaction.
   */
  explicit FileCreateTransaction(const proto::TransactionBody& transaction);

  /**
   * Derived from Transaction. Validate the checksums.
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
  proto::FileCreateTransactionBody build() const;

  /**
   * Set the instant at which this file will expire, after which its contents
   * will no longer be available.
   *
   * Defaults to 1/4 of a Julian year from the instant FileCreateTransaction()
   * was invoked.
   *
   * May be extended using setExpirationTime(Instant).
   *
   * @param expirationTime The nanoseconds from the epoch that the file will
   *                       expire.
   * @return Reference to this FileCreateTransaction object.
   */
  FileCreateTransaction& setExpirationTime(
    const std::chrono::nanoseconds& expirationTime);

  /**
   * Set the keys which must sign any transactions modifying this file.
   *
   * All keys must sign to modify the file's contents or keys. No key is
   * required to sign for extending the expiration time (except the one for the
   * operator account paying for the transaction). Only one key must sign to
   * delete the file, however.
   *
   * To require more than one key to sign to delete a file, add them to a
   * KeyList and pass that here.
   *
   * The network currently requires a file to have at least one key (or key
   * list or threshold key) but this requirement may be lifted in the future.
   *
   * @param keys The keys to be set.
   * @return Reference to this FileCreateTransaction object.
   */
  FileCreateTransaction& setKeys(const KeyList& keys);

  /**
   * Set the given byte array as the file's contents.
   *
   * This may be omitted to create an empty file.
   *
   * Note that total size for a given transaction is limited to 6KiB (as of
   * March 2020) by the network; if you exceed this you may receive a
   * PrecheckStatusException with TRANSACTION_OVERSIZE.
   *
   * In this case, you can use FileAppendTransaction, which automatically breaks
   * the contents into chunks for you, to append contents of arbitrary size.
   *
   * @param bytes The contents of the file.
   * @return Reference to this FileCreateTransaction object.
   */
  FileCreateTransaction& setContents(const std::string& contents);

  /**
   * Assign a memo to the file (100 bytes max).
   *
   * @param memo The memo string to set.
   * @return Reference to this FileCreateTransaction object.
   */
  FileCreateTransaction& setFileMemo(const std::string& memo);

  /**
   * Extract the expiration time.
   *
   * @return The expiration time.
   */
  inline InitType<std::chrono::nanoseconds> getExpirationTime() const
  {
    return mExpirationTime;
  }

  /**
   * Extract the list of keys.
   *
   * @return The list of keys.
   */
  inline InitType<KeyList> getKeys() const { return mKeys; }

  /**
   * Extract the file contents.
   *
   * @return The file contents.
   */
  inline std::string getContents() const { return mContents; }

  /**
   * Extract the file's memo.
   *
   * @return The file's memo.
   */
  inline std::string getFileMemo() const { return mMemo; }

private:
  /**
   * Initialize from the transaction body.
   */
  void initFromTransactionBody();

  /**
   * The time at which this file should expire (unless FileUpdateTransaction is
   * used before then to extend its life)
   */
  InitType<std::chrono::nanoseconds> mExpirationTime;

  /**
   * All keys at the top level of a key list must sign to create or modify the
   * file. Any one of the keys at the top level key list can sign to delete the
   * file.
   */
  InitType<KeyList> mKeys;

  /**
   * The bytes that are the contents of the file.
   */
  std::string mContents;

  /**
   * The memo associated with the file (UTF-8 encoding max 100 bytes).
   */
  std::string mMemo;
};

} // namespace Hedera

#endif // FILE_CREATE_TRANSACTION_H_