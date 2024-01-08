/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "ContractInfoQuery.h"
#include "ContractInfo.h"
#include "impl/Node.h"

#include <proto/contract_get_info.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
ContractInfoQuery& ContractInfoQuery::setContractId(const ContractId& contractId)
{
  mContractId = contractId;
  return *this;
}

//-----
ContractInfo ContractInfoQuery::mapResponse(const proto::Response& response) const
{
  return ContractInfo::fromProtobuf(response.contractgetinfo().contractinfo());
}

//-----
grpc::Status ContractInfoQuery::submitRequest(const proto::Query& request,
                                              const std::shared_ptr<internal::Node>& node,
                                              const std::chrono::system_clock::time_point& deadline,
                                              proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kContractGetInfo, request, deadline, response);
}

//-----
void ContractInfoQuery::validateChecksums(const Client& client) const
{
  mContractId.validateChecksum(client);
}

//-----
proto::Query ContractInfoQuery::buildRequest(proto::QueryHeader* header) const
{
  auto contractGetInfoQuery = std::make_unique<proto::ContractGetInfoQuery>();
  contractGetInfoQuery->set_allocated_header(header);
  contractGetInfoQuery->set_allocated_contractid(mContractId.toProtobuf().release());

  proto::Query query;
  query.set_allocated_contractgetinfo(contractGetInfoQuery.release());
  return query;
}

//-----
proto::ResponseHeader ContractInfoQuery::mapResponseHeader(const proto::Response& response) const
{
  saveCostFromHeader(response.contractgetinfo().header());
  return response.contractgetinfo().header();
}

} // namespace Hedera
