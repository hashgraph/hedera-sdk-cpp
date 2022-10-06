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
#ifndef ACCOUNT_RECORDS_QUERY_H_
#define ACCOUNT_RECORDS_QUERY_H_

/**
 * Library includes
 */
#include "AccountId.h"
#include "Query.h"
#include "helper/InitType.h"

/**
 * STL includes
 */
#include <vector>

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
class Client;
class TransactionRecord;
} // namespace Hedera

namespace Hedera
{
/**
 * Get all the records for an account for any transfers into it and out of it,
 * that were above the threshold, during the last 25 hours.
 */
class AccountRecordsQuery
  : public Query<std::vector<TransactionRecord>, AccountRecordsQuery>
{
public:
  /**
   * Constructor
   */
  AccountRecordsQuery();

  /**
   * Derived from Query. Validate the checksums of the account ID.
   *
   * @param client  The client with which to validate the checksums
   */
  virtual void validateChecksums(const Client& client) const override;

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
   * Derived from Query. Get the account info header from the response.
   *
   * @param response The associated response to this query.
   * @return         The response header for the derived class's query.
   */
  virtual proto::ResponseHeader mapResponseHeader(
    proto::Response* response) const override;

  /**
   * Derived from Query. Grab the account info query header.
   *
   * @param query  The query of which to extract the header.
   * @return       The account info query header.
   */
  virtual proto::QueryHeader mapRequestHeader(
    const proto::Query& query) const override;

  /**
   * Derived from Query. Extract the account info data from the response
   * object.
   *
   * @param response  The received response from Hedera.
   * @param accountId The account ID that made the request.
   * @param query     The original query.
   * @return          The account info data.
   */
  virtual std::vector<TransactionRecord> mapResponse(
    const proto::Response& response,
    const AccountId& accountId,
    const proto::Query& query) const override;

  /**
   * Sets the account ID for which information is requested.
   *
   * @param accountId The AccountId to be set
   * @return {@code this}
   */
  AccountRecordsQuery& setAccountId(const AccountId& accountId);

  /**
   * Extract the account id.
   *
   * @return                          the account id
   */
  inline InitType<AccountId> getAccountId() { return mAccountId; }

private:
  /**
   * The account ID of the account of which to get the account information.
   */
  InitType<AccountId> mAccountId;
};
} // namespace Hedera

#endif // ACCOUNT_RECORDS_QUERY_H_