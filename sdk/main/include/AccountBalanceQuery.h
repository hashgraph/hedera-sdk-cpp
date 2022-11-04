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
#ifndef ACCOUNT_BALANCE_QUERY_H_
#define ACCOUNT_BALANCE_QUERY_H_

#include "AccountId.h"
#include "ContractId.h"
#include "Query.h"

#include <optional>

namespace Hedera
{
class AccountBalance;
class Client;
class Node;
}

namespace proto
{
class Query;
class Response;
}

namespace grpc
{
class ClientContext;
class Status;
}

namespace Hedera
{
/**
 * Get the balance of a Hedera crypto-currency account. This returns only the balance, so it is a smaller and faster
 * reply than AccountInfoQuery.
 *
 * This query is free.
 */
class AccountBalanceQuery : public Query<AccountBalanceQuery, AccountBalance>
{
public:
  /**
   * Default destructor.
   */
  ~AccountBalanceQuery() override = default;

  /**
   * The account ID for which the balance is being requested. This is mutually exclusive with setContractId().
   *
   * @param accountId The account ID to set.
   * @return Reference to this AccountBalanceQuery object.
   */
  AccountBalanceQuery& setAccountId(const AccountId& accountId);

  /**
   * The contract ID for which the balance is being requested. This is mutually exclusive with setAccountId().
   *
   * @param contractId The ContractId to set.
   * @return Reference to this AccountBalanceQuery object.
   */
  AccountBalanceQuery& setContractId(const ContractId& contractId);

  /**
   * Extract the account ID of the account for which this query is meant.
   *
   * @return The account ID of the query.
   */
  inline std::optional<AccountId> getAccountId() { return mAccountId; }

  /**
   * Extract the contract ID of the contract for which this query is meant.
   *
   * @return The contract ID of the contract for which this query is meant.
   */
  inline std::optional<ContractId> getContractId() { return mContractId; }

protected:
  /**
   * Derived from Executable. Get the gRPC method to call to retrieve an account balance.
   *
   * @param node The Node from which to retrieve the function.
   * @return The gRPC method to call to execute this AccountBalanceQuery.
   */
  std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)> getGrpcMethod(
    const std::shared_ptr<Node>& node) const override;

  /**
   * Derived from Executable. Construct a query protobuf object from this AccountBalanceQuery.
   *
   * @return The query protobuf object that contains this AccountBalanceQuery information.
   */
  proto::Query makeRequest(const Client&, const std::shared_ptr<Node>&) const override;

  /**
   * Derived from Executable. Create an AccountBalance object from a protobuf response object.
   *
   * @param response The protobuf response object.
   * @return The response object with the AccountBalance data.
   */
  AccountBalance mapResponse(const proto::Response& response) const override;

private:
  /**
   * The account ID of the account for which this query is meant.
   */
  std::optional<AccountId> mAccountId;

  /**
   * The contract ID with which this request is associated.
   */
  std::optional<ContractId> mContractId;
};

} // namespace Hedera

#endif // ACCOUNT_BALANCE_QUERY_H_