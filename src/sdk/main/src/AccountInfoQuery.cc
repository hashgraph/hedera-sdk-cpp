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
#include <proto/crypto_get_info.pb.h>

#include "AccountInfo.h"
#include "AccountInfoQuery.h"
#include "TokenId.h"
#include "TokenRelationship.h"
#include "impl/MirrorNodeGateway.h"
#include "impl/Node.h"

#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Hedera
{
//-----
AccountInfoQuery& AccountInfoQuery::setAccountId(const AccountId& accountId)
{
  mAccountId = accountId;
  return *this;
}

//-----
AccountInfo AccountInfoQuery::mapResponse(const proto::Response& response) const
{
  AccountInfo accountInfo = AccountInfo::fromProtobuf(response.cryptogetinfo().accountinfo());

  fetchTokenInformation(accountInfo);

  return accountInfo;
}

//-----
grpc::Status AccountInfoQuery::submitRequest(const proto::Query& request,
                                             const std::shared_ptr<internal::Node>& node,
                                             const std::chrono::system_clock::time_point& deadline,
                                             proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kCryptoGetInfo, request, deadline, response);
}

//-----
void AccountInfoQuery::validateChecksums(const Client& client) const
{
  mAccountId.validateChecksum(client);
}

//-----
void AccountInfoQuery::fetchTokenInformation(AccountInfo& accountInfo) const
{
  json tokens =
    internal::MirrorNodeGateway::MirrorNodeQuery(getMirrorNodeResolution(),
                                                 { mAccountId.toString() },
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

      accountInfo.mTokenRelationships.insert({ tokenId, tokenRelationship });
    }
  }
}

//-----
proto::Query AccountInfoQuery::buildRequest(proto::QueryHeader* header) const
{
  auto accountInfoQuery = std::make_unique<proto::CryptoGetInfoQuery>();
  accountInfoQuery->set_allocated_header(header);
  accountInfoQuery->set_allocated_accountid(mAccountId.toProtobuf().release());

  proto::Query query;
  query.set_allocated_cryptogetinfo(accountInfoQuery.release());
  return query;
}

//-----
proto::ResponseHeader AccountInfoQuery::mapResponseHeader(const proto::Response& response) const
{
  saveCostFromHeader(response.cryptogetinfo().header());
  return response.cryptogetinfo().header();
}

} // namespace Hedera
