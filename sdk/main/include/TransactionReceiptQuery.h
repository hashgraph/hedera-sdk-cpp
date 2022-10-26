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
#ifndef TRANSACTION_RECEIPT_QUERY_H_
#define TRANSACTION_RECEIPT_QUERY_H_

#include "Query.h"
#include "TransactionId.h"

#include <memory>

namespace Hedera
{
class TransactionReceipt;
}

namespace Hedera
{
/**
 * Get the receipt of a transaction, given its transaction ID. Once a transaction reaches consensus, then information
 * about whether it succeeded or failed will be available until the end of the receipt period. Before and after the
 * receipt period, and for a transaction that was never submitted, the receipt is unknown. This query is free (the
 * payment field is left empty). No State proof is available for this response
 */
class TransactionReceiptQuery : public Query<TransactionReceiptQuery, TransactionReceipt>
{
public:
  /**
   * Default destructor.
   */
  ~TransactionReceiptQuery() override = default;

  /**
   * Derived from Query. Determine if payment is required for this TransactionReceiptQuery.
   *
   * @return \c FALSE to indicate this query is free.
   */
  inline bool isPaymentRequired() const override { return false; }

  /**
   * The transaction ID for which the receipt is being requested.
   *
   * @param transactionId The TransactionId to set.
   * @return Reference to this TransactionReceiptQuery object.
   */
  TransactionReceiptQuery& setTransactionId(const TransactionId& transactionId);

  /**
   * Extract the transaction ID of the transaction for which this query is meant.
   *
   * @return The transaction ID of the query.
   */
  inline std::optional<TransactionId> getTransactionId() { return mTransactionId; }

protected:
  /**
   * Derived from Query. Construct a query protobuf object from this TransactionReceiptQuery.
   *
   * @return The query protobuf object that contains this TransactionReceiptQuery information.
   */
  proto::Query makeRequest(const Client&) const override;

  /**
   * Derived from Query. Create a response object from a protobuf response object.
   *
   * @param response The protobuf response object.
   * @return The response object with the response data.
   */
  [[nodiscard]] std::unique_ptr<TransactionReceipt> interpretProtobufResponse(
    const proto::Response& response) const override;

private:
  /**
   * The ID of the transaction for which the receipt is requested.
   */
  std::optional<TransactionId> mTransactionId;
};

} // namespace Hedera

#endif // TRANSACTION_RECEIPT_QUERY_H_
