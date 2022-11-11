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
#ifndef TRANSACTION_RECORD_QUERY_H_
#define TRANSACTION_RECORD_QUERY_H_

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
   * Default destructor.
   */
  ~TransactionRecordQuery() override = default;

  /**
   * The transaction ID for which the record is being requested.
   *
   * @param transactionId The TransactionId to set.
   * @return Reference to this TransactionRecordQuery object.
   */
  TransactionRecordQuery& setTransactionId(const TransactionId& transactionId);

  /**
   * Extract the transaction ID of the transaction for which this query is meant.
   *
   * @return The transaction ID of the query.
   */
  [[nodiscard]] inline TransactionId getTransactionId() const { return mTransactionId; }

protected:
  /**
   * Derived from Executable. Get the gRPC method to call to retrieve a transaction record.
   *
   * @param node The Node from which to retrieve the function.
   * @return The gRPC method to call to execute this TransactionRecordQuery.
   */
  std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)> getGrpcMethod(
    const std::shared_ptr<Node>& node) const override;

  /**
   * Derived from Executable. Construct a query protobuf object from this TransactionRecordQuery.
   *
   * @param client The Client submitting this TransactionRecordQuery.
   * @param node   The Node to which this TransactionRecordQuery is being submitted.
   * @return The query protobuf object that contains this TransactionRecordQuery information.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client, const std::shared_ptr<Node>& node) const override;

  /**
   * Derived from Query. Create an TransactionRecord object from a protobuf response object.
   *
   * @param response The protobuf response object.
   * @return The response object with the TransactionRecord data.
   */
  [[nodiscard]] TransactionRecord mapResponse(const proto::Response& response) const override;

private:
  /**
   * The ID of the transaction for which the record is requested.
   */
  TransactionId mTransactionId;
};

} // namespace Hedera

#endif // TRANSACTION_RECORD_QUERY_H_
