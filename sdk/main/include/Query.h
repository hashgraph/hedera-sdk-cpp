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

#include "Executable.h"

namespace Hedera
{
class AccountId;
class Client;
}

namespace proto
{
class Query;
class QueryHeader;
class Response;
class ResponseHeader;
}

namespace Hedera
{
/**
 * Base class for all queries that can be submitted to Hedera.
 *
 * @tparam SdkRequestType  The SDK request type.
 * @tparam SdkResponseType The SDK response type.
 */
template<typename SdkRequestType, typename SdkResponseType>
class Query : public Executable<SdkRequestType, proto::Query, proto::Response, SdkResponseType>
{
public:
  /**
   * Does this query require a payment? Nearly all queries require payment,
   * so default to returning true. Queries that don't require payment can
   * overwrite this to return false.
   *
   * @return \c TRUE if this query requires a payment, otherwise \c FALSE.
   */
  inline virtual bool isPaymentRequired() const { return true; }

protected:
  /**
   * Default destructor.
   */
  virtual ~Query() = default;

  /**
   * Derived from Executable. Construct a query protobuf object from this query.
   *
   * @return The query protobuf object that contains this query information.
   */
  virtual proto::Query makeRequest() const = 0;

  /**
   * Derived from Executable. Create a response object from a protobuf response object.
   *
   * @param response The protobuf response object.
   * @return The response object with the response data.
   */
  virtual SdkResponseType mapResponse(const proto::Response& response) const = 0;
};

} // namespace Hedera

#endif // QUERY_H_