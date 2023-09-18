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
#include "TokenNftInfoQuery.h"
#include "Client.h"
#include "TokenNftInfo.h"
#include "TransactionRecord.h"
#include "TransferTransaction.h"
#include "impl/Node.h"

#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>
#include <proto/token_get_nft_info.pb.h>

namespace Hedera
{
//-----
TokenNftInfoQuery& TokenNftInfoQuery::setNftId(const NftId& nft)
{
  mNftId = nft;
  return *this;
}

//-----
TokenNftInfo TokenNftInfoQuery::mapResponse(const proto::Response& response) const
{
  return TokenNftInfo::fromProtobuf(response.tokengetnftinfo().nft());
}

//-----
grpc::Status TokenNftInfoQuery::submitRequest(const proto::Query& request,
                                              const std::shared_ptr<internal::Node>& node,
                                              const std::chrono::system_clock::time_point& deadline,
                                              proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kTokenGetNftInfo, request, deadline, response);
}

//-----
proto::Query TokenNftInfoQuery::buildRequest(proto::QueryHeader* header) const
{
  auto tokenGetNftInfoQuery = std::make_unique<proto::TokenGetNftInfoQuery>();
  tokenGetNftInfoQuery->set_allocated_header(header);
  tokenGetNftInfoQuery->set_allocated_nftid(mNftId.toProtobuf().release());

  proto::Query query;
  query.set_allocated_tokengetnftinfo(tokenGetNftInfoQuery.release());
  return query;
}

//-----
proto::ResponseHeader TokenNftInfoQuery::mapResponseHeader(const proto::Response& response) const
{
  saveCostFromHeader(response.tokengetnftinfo().header());
  return response.tokengetnftinfo().header();
}

} // namespace Hedera
