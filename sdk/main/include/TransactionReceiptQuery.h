/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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

#include <optional>

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

private:
  /**
   * Derived from Executable. Construct a TransactionReceipt object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct a TransactionReceipt object.
   * @return A TransactionReceipt object filled with the Response protobuf object's data
   */
  [[nodiscard]] TransactionReceipt mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit a Query protobuf object which contains this TransactionReceiptQuery's data to a
   * Node.
   *
   * @param request  The Query protobuf object to submit.
   * @param node     The Node to which to submit the request.
   * @param deadline The deadline for submitting the request.
   * @param response Pointer to the ProtoResponseType object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const proto::Query& request,
                                           const std::shared_ptr<internal::Node>& node,
                                           const std::chrono::system_clock::time_point& deadline,
                                           proto::Response* response) const override;

  /**
   * Derived from Executable. Determine the ExecutionStatus of this TransactionReceiptQuery after being submitted.
   *
   * @param status   The response status from the network.
   * @param client   The Client that submitted this TransactionReceiptQuery.
   * @param response The Response protobuf object received from the network in response to submitting this
   *                 TransactionReceiptQuery.
   * @return The status of the submitted TransactionReceiptQuery.
   */
  [[nodiscard]]
  typename Executable<TransactionReceiptQuery, proto::Query, proto::Response, TransactionReceipt>::ExecutionStatus
  determineStatus(Status status,
                  [[maybe_unused]] const Client& client,
                  [[maybe_unused]] const proto::Response& response) override;
  /**
   * Derived from Query. Build a Query protobuf object with this TransactionReceiptQuery's data, with the input
   * QueryHeader protobuf object.
   *
   * @param header A pointer to the QueryHeader protobuf object to add to the Query protobuf object.
   * @return The constructed Query protobuf object.
   */
  [[nodiscard]] proto::Query buildRequest(proto::QueryHeader* header) const override;

  /**
   * Derived from Query. Get the ResponseHeader protobuf object from the input Response protobuf object.
   *
   * @param response The Response protobuf object from which to get the ResponseHeader protobuf object.
   * @return The ResponseHeader protobuf object of the input Response protobuf object for this derived Query.
   */
  [[nodiscard]] proto::ResponseHeader mapResponseHeader(const proto::Response& response) const override;

  /**
   * The ID of the transaction of which this query should get the receipt.
   */
  std::optional<TransactionId> mTransactionId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_RECEIPT_QUERY_H_
