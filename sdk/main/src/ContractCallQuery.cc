/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "ContractCallQuery.h"
#include "ContractFunctionResult.h"
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
ContractCallQuery& ContractCallQuery::setFunction(std::string_view name, const ContractFunctionParameters& parameters)
{
  setFunctionParameters(parameters.toBytes(name));
  return *this;
}

//-----
ContractCallQuery& ContractCallQuery::setContractId(const ContractId& contractId)
{
  mContractId = contractId;
  return *this;
}

//-----
ContractCallQuery& ContractCallQuery::setGas(const uint64_t& gas)
{
  mGas = gas;
  return *this;
}

//-----
ContractCallQuery& ContractCallQuery::setFunctionParameters(const std::vector<std::byte>& functionParameters)
{
  mFunctionParameters = functionParameters;
  return *this;
}

//-----
ContractCallQuery& ContractCallQuery::setSenderAccountId(const AccountId& accountId)
{
  mSenderAccountId = accountId;
  return *this;
}

//-----
ContractFunctionResult ContractCallQuery::mapResponse(const proto::Response& response) const
{
  return ContractFunctionResult::fromProtobuf(response.contractcalllocal().functionresult());
}

//-----
grpc::Status ContractCallQuery::submitRequest(const proto::Query& request,
                                              const std::shared_ptr<internal::Node>& node,
                                              const std::chrono::system_clock::time_point& deadline,
                                              proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kContractCallLocal, request, deadline, response);
}

//-----
proto::Query ContractCallQuery::buildRequest(proto::QueryHeader* header) const
{
  auto contractCallQuery = std::make_unique<proto::ContractCallLocalQuery>();
  contractCallQuery->set_allocated_header(header);
  contractCallQuery->set_allocated_contractid(mContractId.toProtobuf().release());
  contractCallQuery->set_gas(static_cast<int64_t>(mGas));
  contractCallQuery->set_functionparameters(internal::Utilities::byteVectorToString(mFunctionParameters));

  if (mSenderAccountId.has_value())
  {
    contractCallQuery->set_allocated_sender_id(mSenderAccountId->toProtobuf().release());
  }

  proto::Query query;
  query.set_allocated_contractcalllocal(contractCallQuery.release());
  return query;
}

//-----
proto::ResponseHeader ContractCallQuery::mapResponseHeader(const proto::Response& response) const
{
  saveCostFromHeader(response.contractcalllocal().header());
  return response.contractcalllocal().header();
}

} // namespace Hedera
