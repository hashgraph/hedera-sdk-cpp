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
#include "TokenInfoQuery.h"
#include "TokenInfo.h"
#include "impl/Node.h"

#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>
#include <proto/token_get_info.pb.h>

namespace Hedera
{
//-----
TokenInfoQuery& TokenInfoQuery::setTokenId(const TokenId& tokenId)
{
  mTokenId = tokenId;
  return *this;
}

//-----
TokenInfo TokenInfoQuery::mapResponse(const proto::Response& response) const
{
  return TokenInfo::fromProtobuf(response.tokengetinfo().tokeninfo());
}

//-----
grpc::Status TokenInfoQuery::submitRequest(const proto::Query& request,
                                           const std::shared_ptr<internal::Node>& node,
                                           const std::chrono::system_clock::time_point& deadline,
                                           proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kTokenGetInfo, request, deadline, response);
}

//-----
proto::Query TokenInfoQuery::buildRequest(proto::QueryHeader* header) const
{
  auto tokenGetInfoQuery = std::make_unique<proto::TokenGetInfoQuery>();
  tokenGetInfoQuery->set_allocated_header(header);
  tokenGetInfoQuery->set_allocated_token(mTokenId.toProtobuf().release());

  proto::Query query;
  query.set_allocated_tokengetinfo(tokenGetInfoQuery.release());
  return query;
}

//-----
proto::ResponseHeader TokenInfoQuery::mapResponseHeader(const proto::Response& response) const
{
  Query<TokenInfoQuery, TokenInfo>::saveCostFromHeader(response.tokengetinfo().header());
  return response.tokengetinfo().header();
}

} // namespace Hedera
