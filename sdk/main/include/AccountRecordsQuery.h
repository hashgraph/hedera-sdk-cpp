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
#ifndef HEDERA_SDK_CPP_ACCOUNT_RECORDS_QUERY_H_
#define HEDERA_SDK_CPP_ACCOUNT_RECORDS_QUERY_H_

#include "AccountId.h"
#include "Query.h"

#include <vector>

namespace Hedera
{
class AccountRecords;
class TransactionRecord;
}

namespace Hedera
{
/**
 * Get all the records for an account for any transfers into it and out of it, that were above the threshold, during the
 * last 25 hours.
 */
class AccountRecordsQuery : public Query<AccountRecordsQuery, AccountRecords>
{
public:
  /**
   * Set the ID of the account of which to request the records.
   *
   * @param accountId The ID of the desired account of which to request the records.
   * @return A reference to this AccountRecordsQuery object with the newly-set account ID.
   */
  AccountRecordsQuery& setAccountId(const AccountId& accountId);

  /**
   * Get the ID of the account of which this query is currently configured to get the records.
   *
   * @return The ID of the account for which this query is meant.
   */
  [[nodiscard]] inline AccountId getAccountId() const { return mAccountId; }

private:
  /**
   * Derived from Executable. Construct a Query protobuf object from this AccountRecordsQuery object.
   *
   * @param client The Client trying to construct this AccountRecordsQuery.
   * @param node   The Node to which this AccountRecordsQuery will be sent.
   * @return A Query protobuf object filled with this AccountRecordsQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct an AccountRecords object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct an AccountRecords object.
   * @return An AccountRecords object filled with the Response protobuf object's data.
   */
  [[nodiscard]] AccountRecords mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the status response code for a submitted AccountRecordsQuery from a Response protobuf
   * object.
   *
   * @param response The Response protobuf object from which to grab the AccountRecordsQuery status response code.
   * @return The AccountRecordsQuery status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit this AccountRecordsQuery to a Node.
   *
   * @param client   The Client submitting this AccountRecordsQuery.
   * @param deadline The deadline for submitting this AccountRecordsQuery.
   * @param node     Pointer to the Node to which this AccountRecordsQuery should be submitted.
   * @param response Pointer to the Response protobuf object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::Response* response) const override;

  /**
   * The ID of the account of which this query should get the records.
   */
  AccountId mAccountId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ACCOUNT_RECORDS_QUERY_H_
