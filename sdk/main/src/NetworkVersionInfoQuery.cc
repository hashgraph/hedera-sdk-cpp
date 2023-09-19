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
#include "NetworkVersionInfoQuery.h"
#include "Client.h"
#include "NetworkVersionInfo.h"
#include "Status.h"
#include "TransferTransaction.h"
#include "impl/Node.h"

#include <proto/network_get_version_info.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
NetworkVersionInfo NetworkVersionInfoQuery::mapResponse(const proto::Response& response) const
{
  return NetworkVersionInfo::fromProtobuf(response.networkgetversioninfo());
}

//-----
grpc::Status NetworkVersionInfoQuery::submitRequest(const proto::Query& request,
                                                    const std::shared_ptr<internal::Node>& node,
                                                    const std::chrono::system_clock::time_point& deadline,
                                                    proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kNetworkGetVersionInfo, request, deadline, response);
}

//-----
proto::Query NetworkVersionInfoQuery::buildRequest(proto::QueryHeader* header) const
{
  auto networkVersionInfoQuery = std::make_unique<proto::NetworkGetVersionInfoQuery>();
  networkVersionInfoQuery->set_allocated_header(header);

  proto::Query query;
  query.set_allocated_networkgetversioninfo(networkVersionInfoQuery.release());
  return query;
}

//-----
proto::ResponseHeader NetworkVersionInfoQuery::mapResponseHeader(const proto::Response& response) const
{
  saveCostFromHeader(response.networkgetversioninfo().header());
  return response.networkgetversioninfo().header();
}

} // namespace Hedera
