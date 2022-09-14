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

/**
 * Library includes
 */
#include "AccountId.h"
#include "ContractId.h"
#include "Query.h"
#include "helper/InitType.h"

/**
 * Protobuf forward declarations
 */
namespace proto
{
class Query;
class QueryHeader;
class Response;
class ResponseHeader;
} // namespace proto

/**
 * Hedera forward declarations
 */
namespace Hedera
{
class AccountBalance;
class Client;
} // namespace Hedera

namespace Hedera
{
/**
 * Get the balance of a Hedera crypto-currency account. This returns only the
 * balance, so it is a smaller and faster reply than AccountInfoQuery.
 *
 * This query is free.
 */
class AccountBalanceQuery : public Query<AccountBalance, AccountBalanceQuery>
{
public:
  /**
   * Constructor
   */
  AccountBalanceQuery();

  /**
   * Construct with an account ID.
   *
   * @param accountId  The account ID with which to initialize.
   */
  AccountBalanceQuery(const AccountId& accountId);

  /**
   * Construct with a contract ID.
   *
   * @param contractId The contract ID with which to initialize.
   */
  AccountBalanceQuery(const ContractId& contractId);

  /**
   * Derived from Query. Fills query with this class's data and attaches the
   * header.
   *
   * @param query  The query object to fill out.
   * @param header The header for the query.
   */
  virtual void onMakeRequest(proto::Query* query,
                             proto::QueryHeader* header) const override;

  /**
   * Derived from Query. Get the account balance header from the response.
   *
   * @param response The associated response to this query.
   * @return         The response header for the derived class's query.
   */
  virtual proto::ResponseHeader mapResponseHeader(
    proto::Response* response) const override;

  /**
   * Derived from Query. Extract the account balance data from the response
   * object.
   *
   * @param response  The received response from Hedera.
   * @param accountId The account ID that made the request.
   * @param query     The original query.
   * @return          The account balance data.
   */
  AccountBalance mapResponse(const proto::Response& response,
                             const AccountId& accountId,
                             const proto::Query& query) const override;

  /**
   * Derived from Query. Grab the account balance query header.
   *
   * @param query  The query of which to extract the header.
   * @return       The account balance query header.
   */
  virtual proto::QueryHeader mapRequestHeader(
    const proto::Query& query) const override;

  /**
   * Derived from Query. Validate the checksums of the account ID or contract
   * ID.
   *
   * @param client  The client with which to validate the checksums
   */
  virtual void validateChecksums(const Client& client) const override;

  /**
   * Determine if payment is required for this query.
   *
   * @return \c FALSE to indicate this query is free.
   */
  virtual inline bool isPaymentRequired() const override { return false; }

  /**
   * Return the account ID of the account for which this query is meant.
   *
   * @return The AccountId of the query.
   */
  inline InitType<AccountId> getAccountId() { return mAccountId; }

  /**
   * The account ID for which the balance is being requested. This is mutually
   * exclusive with setContractId().
   *
   * @param accountId The AccountId to set.
   * @return Reference to this AccountBalanceQuery object.
   */
  inline AccountBalanceQuery& setAccountId(const AccountId& accountId)
  {
    mAccountId.setValue(accountId);
    return *this;
  }

  /**
   * Extract the contract id with which this request is associated.
   *
   * @return The contract id with which this request is associated.
   */
  inline InitType<ContractId> getContractId() { return mContractId; }

  /**
   * The contract ID for which the balance is being requested.
   *
   * This is mutually exclusive with setAccountId();
   *
   * @param contractId The ContractId to set
   * @return Reference to this AccountBalanceQuery object.
   */
  inline AccountBalanceQuery& setContractId(const ContractId& contractId)
  {
    mContractId.setValue(contractId);
    return *this;
  }

private:
  /**
   * The account ID of the account for which this query is meant.
   */
  InitType<AccountId> mAccountId;

  /**
   * The contract ID with which this request is associated.
   */
  InitType<ContractId> mContractId;
};

} // namespace Hedera

#endif // ACCOUNT_BALANCE_QUERY_H_