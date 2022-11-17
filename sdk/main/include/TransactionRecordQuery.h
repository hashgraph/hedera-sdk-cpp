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
#ifndef HEDERA_SDK_CPP_TRANSACTION_RECORD_QUERY_H_
#define HEDERA_SDK_CPP_TRANSACTION_RECORD_QUERY_H_

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
  ~TransactionRecordQuery() override = default;

  /**
   * Derived from Executable. Create a clone of this TransactionRecordQuery.
   *
   * @return A pointer to the created clone.
   */
  [[nodiscard]] std::unique_ptr<Executable> clone() const override;

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

protected:
  /**
   * Derived from Executable. Construct a Query protobuf object from this TransactionRecordQuery object.
   *
   * @param client The Client trying to construct this TransactionRecordQuery.
   * @param node   The Node on which this TransactionRecordQuery will be sent.
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
   * Derived from Executable. Get the gRPC method to call to send this TransactionRecordQuery.
   *
   * @param node The Node to which this TransactionRecordQuery is being sent and from which to retrieve the gRPC
   *             method.
   * @return The gRPC method to call to execute this TransactionRecordQuery.
   */
  [[nodiscard]] std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)> getGrpcMethod(
    const std::shared_ptr<internal::Node>& node) const override;

private:
  /**
   * The ID of the transaction of which this query should get the record.
   */
  std::optional<TransactionId> mTransactionId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_RECORD_QUERY_H_
