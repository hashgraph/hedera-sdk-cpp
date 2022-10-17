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
#ifndef ACCOUNT_BALANCE_QUERY_H_
#define ACCOUNT_BALANCE_QUERY_H_

#include "AccountId.h"
#include "ContractId.h"
#include "Query.h"

namespace proto
{
class Query;
class Response;
}

namespace Hedera
{
class AccountBalance;
class Client;
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
   * Default constructor.
   */
  AccountBalanceQuery() = default;

  /**
   * Default destructor.
   */
  ~AccountBalanceQuery() = default;

  /**
   * Derived from Query. Determine if payment is required for this AccountBalanceQuery.
   *
   * @return \c FALSE to indicate this query is free.
   */
  inline bool isPaymentRequired() const override { return false; }

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
  inline std::shared_ptr<AccountId> getAccountId() { return mAccountId; }

  /**
   * Extract the contract ID of the contract for which this query is meant.
   *
   * @return The contract ID of the contract for which this query is meant.
   */
  inline std::shared_ptr<ContractId> getContractId() { return mContractId; }

protected:
  /**
   * Derived from Query. Construct a query protobuf object from this AccountBalanceQuery.
   *
   * @return The query protobuf object that contains this AccountBalanceQuery information.
   */
  proto::Query makeRequest() const override;

  /**
   * Derived from Query. Create an AccountBalance object from a protobuf response object.
   *
   * @param response The protobuf response object.
   * @return The response object with the AccountBalance data.
   */
  AccountBalance mapResponse(const proto::Response& response) const override;

private:
  /**
   * The account ID of the account for which this query is meant.
   */
  std::shared_ptr<AccountId> mAccountId;

  /**
   * The contract ID with which this request is associated.
   */
  std::shared_ptr<ContractId> mContractId;
};

} // namespace Hedera

#endif // ACCOUNT_BALANCE_QUERY_H_