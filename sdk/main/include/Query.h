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
#ifndef QUERY_H_
#define QUERY_H_

/**
 * Hedera forward declarations
 */
namespace Hedera
{
class AccountId;
class Client;
} // namespace Hedera

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

namespace Hedera
{
/**
 * Base class for all queries that can be submitted to Hedera.
 *
 * @param <O> The output type of the query.
 * @param <T> The type of the query itself. Used to enable chaining.
 */
template<typename O, typename T>
class Query
{
public:
  /**
   * Called in makeRequest() just before the query is built. The intent
   * is for the derived class to assign their data variant to the query.
   *
   * @param query  The query object to fill out.
   * @param header The header for the query.
   */
  virtual void onMakeRequest(proto::Query* query,
                             proto::QueryHeader* header) const = 0;

  /**
   * Function to get a query's response header. Derived classes should access
   * their response header and return.
   *
   * @param response The associated response to this query.
   * @return         The response header for the derived class' query.
   */
  virtual proto::ResponseHeader mapResponseHeader(
    proto::Response* response) const = 0;

  /**
   * Called after receiving the query response from Hedera. The derived class
   * should map into its output type.
   *
   * @param response  The received response from Hedera.
   * @param accountId The account ID that made the request.
   * @param query     The original query.
   * @return          The class representing the response, containing the
   *                  response data.
   */
  virtual O mapResponse(const proto::Response& response,
                        const AccountId& accountId,
                        const proto::Query& query) const = 0;

  /**
   * The derived class should access its request header and return.
   *
   * @param query  The query of which to extract the header.
   * @return       The query header.
   */
  virtual proto::QueryHeader mapRequestHeader(
    const proto::Query& query) const = 0;

  /**
   * Validate the checksums.
   *
   * @param client The client with which to validate the checksums
   */
  virtual void validateChecksums(const Client& client) const = 0;

  /**
   * Does this query require a payment? Nearly all queries require payment,
   * so default to returning true. Queries that don't require payment can
   * overwrite this to return false.
   *
   * @return \c TRUE if this query requires a payment, otherwise \c FALSE.
   */
  inline virtual bool isPaymentRequired() const { return true; }

protected:
  Query() {}
};

} // namespace Hedera

#endif // QUERY_H_