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
#ifndef TRANSACTION_RECEIPT_H_
#define TRANSACTION_RECEIPT_H_

#include "AccountId.h"
#include "ExchangeRateSet.h"
#include "Status.h"

#include <optional>

namespace proto
{
class TransactionReceipt;
}

namespace Hedera
{
/**
 * The summary of a transaction's result so far. If the transaction has not reached consensus, this
 * result will be necessarily incomplete.
 */
class TransactionReceipt
{
public:
  /**
   * Create a TransactionReceipt from a protobuf TransactionReceipt.
   *
   * @param proto The protobuf TransactionReceipt.
   * @return A TransactionReceipt with the protobuf TransactionReceipt data.
   */
  static std::unique_ptr<TransactionReceipt> fromProtobuf(const proto::TransactionReceipt& proto);

  /**
   * Extract the transaction status.
   *
   * @return The transaction status.
   */
  inline Status getStatus() const { return mStatus; }

  /**
   * Extract the created account ID.
   *
   * @return The created account ID.
   */
  inline std::optional<AccountId> getAccountId() const { return mAccountId; };

  /**
   * Extract the current and next exchange rates.
   *
   * @return The current and next exchange rates.
   */
  inline std::optional<ExchangeRateSet> getExchangeRates() const { return mExchangeRates; }

private:
  /**
   * The consensus status of the transaction; is UNKNOWN if consensus has not been reached, or if the associated
   * transaction did not have a valid payer signature
   */
  Status mStatus;

  /**
   * In the receipt of a CryptoCreate, the ID of the newly created account.
   */
  std::optional<AccountId> mAccountId;

  /**
   * The exchange rates in effect when the transaction reached consensus.
   */
  std::optional<ExchangeRateSet> mExchangeRates;
};

} // namespace Hedera

#endif // TRANSACTION_RECEIPT_H_