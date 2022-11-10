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
#ifndef TRANSACTION_ID_H_
#define TRANSACTION_ID_H_

#include "AccountId.h"

#include <chrono>
#include <memory>
#include <optional>

namespace proto
{
class TransactionID;
}

namespace Hedera
{
class TransactionId
{
public:
  /**
   * Generate a transaction ID for a transaction.
   *
   * @param accountId The ID of the account that will be charged for this transaction.
   * @return A new TransactionId.
   */
  static TransactionId generate(const AccountId& accountId);

  /**
   * Construct a TransactionId from a protobuf TransactionID.
   *
   * @param proto The TransactionID protobuf object.
   * @return A TransactionId object with the protobuf TransactionID data.
   */
  static TransactionId fromProtobuf(const proto::TransactionID& proto);

  /**
   * Default comparator operator.
   *
   * @param other The other TransactionId against which to compare.
   * @return \c TRUE if these TransactionIds are the same, otherwise \c FALSE.
   */
  bool operator==(const TransactionId& other) const = default;

  /**
   * Convert this TransactionId to its corresponding protobuf TransactionID.
   *
   * @return Pointer to the created protobuf TransactionID.
   */
  proto::TransactionID* toProtobuf() const;

  /**
   * Extract the valid transaction time.
   *
   * @return The valid transaction time.
   */
  inline std::optional<std::chrono::sys_time<std::chrono::duration<double>>> getValidTransactionTime() const
  {
    return mValidTransactionTime;
  }

  /**
   * Extract the account ID.
   *
   * @return The account ID.
   */
  inline std::optional<AccountId> getAccountId() const { return mAccountId; }

private:
  /**
   * The time the transaction is considered "valid".
   *
   * When a transaction is submitted there is additionally a validDuration (defaults to 120s) and together they define a
   * time window in which a transaction may be processed.
   */
  std::optional<std::chrono::sys_time<std::chrono::duration<double>>> mValidTransactionTime;

  /**
   * The account ID of the account that is paying for this transaction.
   */
  std::optional<AccountId> mAccountId;
};

} // namespace Hedera

#endif // TRANSACTION_ID_H_
