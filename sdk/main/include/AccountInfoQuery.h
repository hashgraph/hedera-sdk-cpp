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
#ifndef HEDERA_SDK_CPP_ACCOUNT_INFO_QUERY_H_
#define HEDERA_SDK_CPP_ACCOUNT_INFO_QUERY_H_

#include "AccountId.h"
#include "Query.h"

namespace Hedera
{
class AccountInfo;
class TransactionRecord;
}

namespace Hedera
{
/**
 * A query that returns the current state of the account. This query does not include the list of records associated
 * with the account. Anyone on the network can request account info for a given account. Queries do not change the state
 * of the account or require network consensus. The information is returned from a single node processing the query.
 */
class AccountInfoQuery : public Query<AccountInfoQuery, AccountInfo>
{
public:
  /**
   * Set the ID of the account of which to request the info.
   *
   * @param accountId The ID of the desired account of which to request the info.
   * @return A reference to this AccountInfoQuery object with the newly-set account ID.
   */
  AccountInfoQuery& setAccountId(const AccountId& accountId);

  /**
   * Get the ID of the account of which this query is currently configured to get the info.
   *
   * @return The ID of the account for which this query is meant.
   */
  [[nodiscard]] inline AccountId getAccountId() const { return mAccountId; }

private:
  /**
   * Derived from Executable. Construct a Query protobuf object from this AccountInfoQuery object.
   *
   * @param client The Client trying to construct this AccountInfoQuery.
   * @param node   The Node to which this AccountInfoQuery will be sent.
   * @return A Query protobuf object filled with this AccountInfoQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct an AccountInfo object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct an AccountInfo object.
   * @return An AccountInfo object filled with the Response protobuf object's data.
   */
  [[nodiscard]] AccountInfo mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the status response code for a submitted AccountInfoQuery from a Response protobuf
   * object.
   *
   * @param response The Response protobuf object from which to grab the AccountInfoQuery status response code.
   * @return The AccountInfoQuery status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit this AccountInfoQuery to a Node.
   *
   * @param client   The Client submitting this AccountInfoQuery.
   * @param deadline The deadline for submitting this AccountInfoQuery.
   * @param node     Pointer to the Node to which this AccountInfoQuery should be submitted.
   * @param response Pointer to the Response protobuf object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::Response* response) const override;

  /**
   * The ID of the account of which this query should get the info.
   */
  AccountId mAccountId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ACCOUNT_INFO_QUERY_H_
