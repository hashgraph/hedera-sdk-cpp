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
#ifndef TRANSACTION_RECORD_H_
#define TRANSACTION_RECORD_H_

#include "AccountId.h"
#include "Hbar.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"

#include <optional>
#include <string>
#include <utility>

namespace proto
{
class TransactionRecord;
}

namespace Hedera
{
/**
 * The complete record for a transaction on Hedera that has reached consensus. This is not-free to request and is
 * available for 1 hour after a transaction reaches consensus. A TransactionReceipt can be thought of as a light-weight
 * record which is free to ask for if you just need what it contains. A receipt however lasts for only 180 seconds.
 */
class TransactionRecord
{
public:
  /**
   * Construct a TransactionRecord from a protobuf TransactionRecord.
   *
   * @param proto The protobuf TransactionRecord.
   * @return A TransactionRecord containing the protobuf TransactionRecord data.
   */
  static TransactionRecord fromProtobuf(const proto::TransactionRecord& proto);

  /**
   * Extract the transaction receipt.
   *
   * @return The transaction receipt.
   */
  [[nodiscard]] inline TransactionReceipt getReceipt() const { return mReceipt; }

  /**
   * Extract the transaction hash.
   *
   * @return The transaction hash.
   */
  [[nodiscard]] inline std::string getTransactionHash() const { return mTransactionHash; }

  /**
   * Extract the consensus timestamp.
   *
   * @return The consensus timestamp.
   */
  [[nodiscard]] inline std::optional<std::chrono::sys_time<std::chrono::duration<double>>> getConsensusTimestamp() const
  {
    return mConsensusTimestamp;
  }

  /**
   * Extract the transaction ID.
   *
   * @return The transaction ID.
   */
  [[nodiscard]] inline std::optional<TransactionId> getTransactionId() const { return mTransactionID; }

  /**
   * Extract the transaction memo.
   *
   * @return The transaction memo.
   */
  [[nodiscard]] inline std::string getTransactionMemo() const { return mMemo; }

  /**
   * Extract the transaction fee.
   *
   * @return The transaction fee.
   */
  [[nodiscard]] inline uint64_t getTransactionFee() const { return mTransactionFee; }

  /**
   * Extract the transfer list.
   *
   * @return The transfer list.
   */
  [[nodiscard]] inline std::vector<std::pair<AccountId, Hbar>> getTransferList() const { return mTransferList; }

private:
  /**
   * The status (reach consensus, or failed, or is unknown) and the ID of any new account/file/instance created.
   */
  TransactionReceipt mReceipt;

  /**
   * The hash of the Transaction that executed (not the hash of any Transaction that failed for having a duplicate
   * TransactionID).
   */
  std::string mTransactionHash;

  /**
   * The consensus timestamp (or null if didn't reach consensus yet).
   */
  std::optional<std::chrono::sys_time<std::chrono::duration<double>>> mConsensusTimestamp;

  /**
   * The ID of the transaction this record represents.
   */
  TransactionId mTransactionID;

  /**
   * The memo that was submitted as part of the transaction (max 100 bytes).
   */
  std::string mMemo;

  /**
   * The actual transaction fee charged, not the original transactionFee value from TransactionBody.
   */
  uint64_t mTransactionFee;

  /**
   * All Hbar transfers as a result of this transaction, such as fees, or transfers performed by the transaction, or by
   * a smart contract it calls, or by the creation of threshold records that it triggers.
   */
  std::vector<std::pair<AccountId, Hbar>> mTransferList;
};

} // namespace Hedera

#endif // TRANSACTION_RECORD_H_