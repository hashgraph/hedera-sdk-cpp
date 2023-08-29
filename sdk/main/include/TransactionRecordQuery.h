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
#ifndef HEDERA_SDK_CPP_TRANSACTION_RECORD_QUERY_H_
#define HEDERA_SDK_CPP_TRANSACTION_RECORD_QUERY_H_

#include "Query.h"
#include "TransactionId.h"

#include <optional>

namespace Hedera
{
class TransactionRecord;
}

namespace Hedera
{
/**
 * Get the record for a transaction. If the transaction requested a record, then the record lasts for one hour, and a
 * state proof is available for it. If the transaction created an account, file, or smart contract instance, then the
 * record will contain the ID for what it created. If the transaction called a smart contract function, then the record
 * contains the result of that call. If the transaction was a cryptocurrency transfer, then the record includes the
 * TransferList which gives the details of that transfer. If the transaction didn't return anything that should be in
 * the record, then the results field will be set to nothing.
 */
class TransactionRecordQuery : public Query<TransactionRecordQuery, TransactionRecord>
{
public:
  /**
   * Set the ID of the transaction of which to request the record.
   *
   * @param transactionId The ID of the desired transaction of which to request the record.
   * @return A reference to this TransactionRecordQuery object with the newly-set transaction ID.
   */
  TransactionRecordQuery& setTransactionId(const TransactionId& transactionId);

  /**
   * Get the ID of the transaction of which this query is currently configured to get the record.
   *
   * @return The ID of the transaction for which this query is meant. Returns uninitialized if a value has not yet been
   *         set.
   */
  [[nodiscard]] inline std::optional<TransactionId> getTransactionId() const { return mTransactionId; }

private:
  /**
   * Derived from Executable. Construct a Query protobuf object from this TransactionRecordQuery object.
   *
   * @param client The Client trying to construct this TransactionRecordQuery.
   * @param node   The Node to which this TransactionRecordQuery will be sent.
   * @return A Query protobuf object filled with this TransactionRecordQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct a TransactionRecord object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct a TransactionRecord object.
   * @return An TransactionRecord object filled with the Response protobuf object's data
   */
  [[nodiscard]] TransactionRecord mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the status response code for a submitted TransactionRecordQuery from a Response
   * protobuf object.
   *
   * @param response The Response protobuf object from which to grab the TransactionRecordQuery status response code.
   * @return The TransactionRecordQuery status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Determine the ExecutionStatus of this TransactionRecordQuery after being submitted.
   *
   * @param status   The response status of the previous attempt.
   * @param client   The Client that attempted to submit this TransactionRecordQuery.
   * @param response The Response protobuf object received from the network in response to submitting this request.
   * @return The status of the submitted request.
   */
  [[nodiscard]]
  typename Executable<TransactionRecordQuery, proto::Query, proto::Response, TransactionRecord>::ExecutionStatus
  determineStatus(Status status,
                  [[maybe_unused]] const Client& client,
                  [[maybe_unused]] const proto::Response& response) override;

  /**
   * Derived from Executable. Submit this TransactionRecordQuery to a Node.
   *
   * @param client   The Client submitting this TransactionRecordQuery.
   * @param deadline The deadline for submitting this TransactionRecordQuery.
   * @param node     Pointer to the Node to which this TransactionRecordQuery should be submitted.
   * @param response Pointer to the Response protobuf object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::Response* response) const override;

  /**
   * The ID of the transaction of which this query should get the record.
   */
  std::optional<TransactionId> mTransactionId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_RECORD_QUERY_H_
