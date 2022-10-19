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
#ifndef TRANSACTION_RESPONSE_H_
#define TRANSACTION_RESPONSE_H_

#include "TransactionId.h"

#include <chrono>
#include <cstdint>

namespace Hedera
{
class Client;
class TransactionReceipt;
}

namespace proto
{
class TransactionResponse;
}

namespace Hedera
{
/**
 * When the client sends the node a transaction of any kind, the node replies with this, which simply says that the
 * transaction passed the precheck (so the node will submit it to the network) or it failed (so it won't). To learn the
 * consensus result, the client should later obtain a receipt (free), or can buy a more detailed record (not free).
 */
class TransactionResponse
{
public:
  /**
   * Create a TransactionResponse object from a transaction response protobuf object.
   *
   * @param proto The protobuf transaction response object.
   * @return A TransactionResponse object containing the data of the input protobuf transaction response.
   */
  static TransactionResponse fromProtobuf(const proto::TransactionResponse& proto);

  /**
   * Get the receipt for this transaction.
   *
   * @param client The client with which this query will be executed.
   * @return The receipt for this transaction.
   */
  TransactionReceipt getReceipt(const Client& client) const;

  /**
   * Get the receipt for this transaction with a specified timeout.
   *
   * @param client  The client with which this query will be executed.
   * @param timeout The timeout for this query.
   * @return The receipt for this transaction.
   */
  TransactionReceipt getReceipt(const Client& client, const std::chrono::duration<double>& timeout) const;

  /**
   * Set the transaction ID to which this TransactionResponse is responding.
   *
   * @param transactionId The transaction ID to set.
   * @return Reference to this Transaction object.
   */
  TransactionResponse& setTransactionId(const TransactionId& transactionId);

  /**
   * Extract the price of the transaction.
   *
   * @return The price of the transaction.
   */
  inline uint64_t getCost() const { return mCost; }

  /**
   * Determine if the transaction pre-checks were a success.
   *
   * @return \c TRUE if the transaction pre-checks were a success, otherwise \c FALSE.
   */
  inline bool getValidateStatus() const { return mValidateStatus; }

  /**
   * Extract the transaction ID of the transaction.
   *
   * @return The transaction ID of the transaction.
   */
  inline TransactionId getTransactionId() const { return mTransactionId; }

private:
  /**
   * The price of the transaction.
   */
  uint64_t mCost;

  /**
   * The response of the transaction. \c TRUE if the precheck was a success, otherwise \c FALSE.
   */
  bool mValidateStatus;

  /**
   * The transaction ID of the transaction.
   */
  TransactionId mTransactionId;
};

} // namespace Hedera

#endif // TRANSACTION_RESPONSE_H_