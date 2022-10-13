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
#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include "Hbar.h"

#include <proto/transaction_body.pb.h>

#include <chrono>
#include <unordered_map>

namespace Hedera
{
class AccountId;
class Client;
class TransactionId;
}

namespace proto
{
class SchedulableTransactionBody;
}

namespace Hedera
{
/**
 * Base class for all transactions that may be built and submitted to Hedera.
 *
 * @param <T> The type of the transaction. Used to enable chaining.
 */
template<typename T>
class Transaction
{
public:
  virtual ~Transaction() = default;

protected:
  Transaction() = default;
  Transaction(const Transaction&) = default;
  Transaction& operator=(const Transaction&) = default;
  Transaction(const Transaction&&) = delete;
  Transaction& operator=(const Transaction&&) = delete;

  Transaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  {
    (void)transactions;
  }

  Transaction(const proto::TransactionBody& transaction) { (void)transaction; }

  /**
   * Validate the checksums.
   *
   * @param client The client with which to validate the checksums
   */
  virtual void validateChecksums(const Client& client) const = 0;

  /**
   * Called in freezeWith(Client) just before the transaction body is built. The
   * intent is for the derived class to assign their data variant to the
   * transaction body.
   */
  virtual void onFreeze(proto::TransactionBody* body) const = 0;

  /**
   * Called in schedule() when converting transaction into a scheduled version.
   */
  virtual void onScheduled(proto::SchedulableTransactionBody* body) const = 0;

  void requireNotFrozen() {}

  proto::TransactionBody mSourceTransactionBody;

  Hbar mDefaultMaxTransactionFee;

  std::chrono::seconds mDefaultAutoRenewPeriod;
};

} // namespace Hedera

#endif // TRANSACTION_H_