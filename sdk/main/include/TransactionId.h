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
#ifndef HEDERA_SDK_CPP_TRANSACTION_ID_H_
#define HEDERA_SDK_CPP_TRANSACTION_ID_H_

#include "AccountId.h"

#include <chrono>

namespace proto
{
class TransactionID;
}

namespace Hedera
{
/**
 * The ID for a transaction. This is used for retrieving receipts and records for a transaction, for appending to a file
 * right after creating it, for instantiating a smart contract with bytecode in a file just created, and internally by
 * the network for detecting when duplicate transactions are submitted. A user might get a transaction processed faster
 * by submitting it to N nodes, each with a different node account, but all with the same TransactionID. Then, the
 * transaction will take effect when the first of all those nodes submits the transaction and it reaches consensus. The
 * other transactions will not take effect. So this could make the transaction take effect faster, if any given node
 * might be slow. However, the full transaction fee is charged for each transaction, so the total fee is N times as much
 * if the transaction is sent to N nodes.
 *
 * Applicable to Scheduled Transactions:
 *  - The ID of a Scheduled Transaction has transactionValidStart and accountIDs inherited from the
 *    ScheduleCreate transaction that created it. That is to say that they are equal
 *  - The scheduled property is true for Scheduled Transactions
 *  - transactionValidStart, accountID and scheduled properties should be omitted
 */
class TransactionId
{
public:
  /**
   * Generate a new TransactionId.
   *
   * @param accountId The ID of the account to be charged for the execution of the transaction with which this ID will
   *                  be associated.
   * @return A generated TransactionId to be used for any transaction submitted by the account with the input ID.
   */
  [[nodiscard]] static TransactionId generate(const AccountId& accountId);

  /**
   * Construct a TransactionId object from a TransactionID protobuf object.
   *
   * @param proto The TransactionID protobuf object from which to construct a TransactionId object.
   * @return The constructed TransactionId object.
   */
  [[nodiscard]] static TransactionId fromProtobuf(const proto::TransactionID& proto);

  /**
   * Compare this TransactionId to another TransactionId and determine if they represent the same Transaction.
   *
   * @param other The other TransactionId with which to compare this TransactionId.
   * @return \c TRUE if this TransactionId is the same as the input TransactionId, otherwise \c FALSE.
   */
  bool operator==(const TransactionId&) const;

  /**
   * Construct a TransactionID protobuf object from this TransactionId object.
   *
   * @return A pointer to the created TransactionID protobuf object filled with this TransactionId object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::TransactionID> toProtobuf() const;

  /**
   * Get the point in time when the transaction represented by this TransactionId becomes (or became) valid.
   *
   * @return The valid start time of the transaction.
   */
  [[nodiscard]] inline std::chrono::system_clock::time_point getValidTransactionTime() const
  {
    return mValidTransactionTime;
  }

  /**
   * Get the ID of the account submitting the transaction represented by this TransactionId.
   *
   * @return A pointer to the account ID associated with this TransactionId.
   */
  [[nodiscard]] inline AccountId getAccountId() const { return mAccountId; }

private:
  /**
   * The time at which the transaction associated with this TransactionId is considered "valid".
   *
   * When a transaction is submitted there is additionally a validDuration (defaults to 120s) and together they define a
   * time window in which the transaction may be processed.
   */
  std::chrono::system_clock::time_point mValidTransactionTime;

  /**
   * The ID of the account that is paying for this transaction associated with this TransactionId.
   */
  AccountId mAccountId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_ID_H_
