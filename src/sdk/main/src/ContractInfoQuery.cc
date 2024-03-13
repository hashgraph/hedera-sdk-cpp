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
#include "impl/MirrorNodeGateway.h"
#include "impl/Node.h"

#include <proto/contract_get_info.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
  ContractInfo contractInfo = ContractInfo::fromProtobuf(response.contractgetinfo().contractinfo());

  fetchTokenInformation(contractInfo);

  return contractInfo;
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
void ContractInfoQuery::fetchTokenInformation(ContractInfo& contractInfo) const
{
  json tokens =
    internal::MirrorNodeGateway::MirrorNodeQuery(getMirrorNodeResolution(),
                                                 { mContractId.toString() },
                                                 internal::MirrorNodeGateway::TOKEN_RELATIONSHIPS_QUERY.data());
  if (!tokens["tokens"].empty())
  {
    for (const auto& token : tokens["tokens"])
    {
      bool automaticAssociation = token["automatic_association"];
      uint64_t balance = token["balance"];
      uint32_t decimals = token["decimals"];
      std::string kycStatus = token["kyc_status"].dump().substr(1, token["kyc_status"].dump().length() - 2);
      std::string freezeStatus = token["freeze_status"].dump().substr(1, token["freeze_status"].dump().length() - 2);
      TokenId tokenId = TokenId::fromString(token["token_id"].dump().substr(1, token["token_id"].dump().length() - 2));

      TokenRelationship tokenRelationship(
        tokenId, "", balance, decimals, kycStatus, freezeStatus, automaticAssociation);

      contractInfo.mTokenRelationships.insert({ tokenId, tokenRelationship });
    }
  }
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
