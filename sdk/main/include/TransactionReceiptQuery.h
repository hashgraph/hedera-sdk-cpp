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
#ifndef HEDERA_SDK_CPP_TRANSACTION_RECEIPT_QUERY_H_
#define HEDERA_SDK_CPP_TRANSACTION_RECEIPT_QUERY_H_

#include "Query.h"
#include "TransactionId.h"

#include <functional>
#include <memory>
#include <optional>

namespace Hedera
{
namespace internal
{
class Node;
}
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
  ~TransactionReceiptQuery() override = default;

  /**
   * Derived from Executable. Create a clone of this TransactionReceiptQuery.
   *
   * @return A pointer to the created clone.
   */
  [[nodiscard]] std::unique_ptr<Executable> clone() const override;

  /**
   * Set the ID of the transaction of which to request the receipt.
   *
   * @param transactionId The ID of the desired transaction of which to request the receipt.
   * @return A reference to this TransactionReceiptQuery object with the newly-set transaction ID.
   */
  TransactionReceiptQuery& setTransactionId(const TransactionId& transactionId);

  /**
   * Get the ID of the transaction of which this query is currently configured to get the receipt.
   *
   * @return The ID of the transaction for which this query is meant. Returns uninitialized if a value has not yet been
   *         set.
   */
  [[nodiscard]] inline std::optional<TransactionId> getTransactionId() const { return mTransactionId; }

protected:
  /**
   * Derived from Executable. Construct a Query protobuf object from this TransactionReceiptQuery object.
   *
   * @param client The Client trying to construct this TransactionReceiptQuery.
   * @param node   The Node on which this TransactionReceiptQuery will be sent.
   * @return A Query protobuf object filled with this TransactionReceiptQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct a TransactionReceipt object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct a TransactionReceipt object.
   * @return An TransactionReceipt object filled with the Response protobuf object's data
   */
  [[nodiscard]] TransactionReceipt mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the gRPC method to call to send this TransactionReceiptQuery.
   *
   * @param node The Node to which this TransactionReceiptQuery is being sent and from which to retrieve the gRPC
   *             method.
   * @return The gRPC method to call to execute this TransactionReceiptQuery.
   */
  [[nodiscard]] std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)> getGrpcMethod(
    const std::shared_ptr<internal::Node>& node) const override;

private:
  /**
   * The ID of the transaction of which this query should get the receipt.
   */
  std::optional<TransactionId> mTransactionId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_RECEIPT_QUERY_H_
