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
#ifndef CONTRACT_INFO_QUERY_H_
#define CONTRACT_INFO_QUERY_H_

#include "ContractId.h"
#include "Query.h"

#include "helper/InitType.h"

namespace proto
{
class Query;
class QueryHeader;
class Response;
class ResponseHeader;
}

namespace Hedera
{
class Client;
class ContractInfo;
}

namespace Hedera
{
/**
 * Get all the information about an account, including the balance.
 * This does not get the list of account records.
 */
class ContractInfoQuery : public Query<ContractInfo, ContractInfoQuery>
{
public:
  /**
   * Constructor
   */
  ContractInfoQuery();

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
   * Derived from Query. Get the contract info header from the response.
   *
   * @param response The associated response to this query.
   * @return         The response header for the derived class's query.
   */
  virtual proto::ResponseHeader mapResponseHeader(
    proto::Response* response) const override;

  /**
   * Derived from Query. Grab the contract info query header.
   *
   * @param query  The query of which to extract the header.
   * @return       The contract info query header.
   */
  virtual proto::QueryHeader mapRequestHeader(
    const proto::Query& query) const override;

  /**
   * Derived from Query. Extract the contract info data from the response
   * object.
   *
   * @param response  The received response from Hedera.
   * @param accountId The account ID that made the request.
   * @param query     The original query.
   * @return          The contract info data.
   */
  virtual ContractInfo mapResponse(const proto::Response& response,
                                   const AccountId& accountId,
                                   const proto::Query& query) const override;

  /**
   * Sets the contract ID for which information is requested.
   *
   * @param accountId The contract ID to be set.
   * @return          Reference to this ContractInfoQuery object.
   */
  ContractInfoQuery& setContractId(const ContractId& accountId);

  /**
   * Extract the contract ID.
   *
   * @return The contract ID.
   */
  inline InitType<ContractId> getContractId() { return mContractId; }

private:
  /**
   * The contract ID for which information is requested.
   */
  InitType<ContractId> mContractId;
};

} // namespace Hedera

#endif // CONTRACT_INFO_QUERY_H_