/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "ContractByteCodeQuery.h"
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <proto/contract_get_bytecode.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
ContractByteCodeQuery& ContractByteCodeQuery::setContractId(const ContractId& contractId)
{
  mContractId = contractId;
  return *this;
}

//-----
ContractByteCode ContractByteCodeQuery::mapResponse(const proto::Response& response) const
{
  return internal::Utilities::stringToByteVector(response.contractgetbytecoderesponse().bytecode());
}

//-----
grpc::Status ContractByteCodeQuery::submitRequest(const proto::Query& request,
                                                  const std::shared_ptr<internal::Node>& node,
                                                  const std::chrono::system_clock::time_point& deadline,
                                                  proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kContractGetBytecode, request, deadline, response);
}

//-----
void ContractByteCodeQuery::validateChecksums(const Client& client) const
{
  mContractId.validateChecksum(client);
}

//-----
proto::Query ContractByteCodeQuery::buildRequest(proto::QueryHeader* header) const
{
  auto contractByteCodeQuery = std::make_unique<proto::ContractGetBytecodeQuery>();
  contractByteCodeQuery->set_allocated_header(header);
  contractByteCodeQuery->set_allocated_contractid(mContractId.toProtobuf().release());

  proto::Query query;
  query.set_allocated_contractgetbytecode(contractByteCodeQuery.release());
  return query;
}

//-----
proto::ResponseHeader ContractByteCodeQuery::mapResponseHeader(const proto::Response& response) const
{
  saveCostFromHeader(response.contractgetbytecoderesponse().header());
  return response.contractgetbytecoderesponse().header();
}

} // namespace Hedera