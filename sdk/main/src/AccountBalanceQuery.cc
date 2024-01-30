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
#include "AccountBalanceQuery.h"
#include "AccountBalance.h"
#include "TokenId.h"
#include "exceptions/UninitializedException.h"
#include "impl/MirrorNodeGateway.h"
#include "impl/Node.h"

#include <proto/crypto_get_account_balance.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

#include <cstddef>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Hedera
{
//-----
AccountBalanceQuery& AccountBalanceQuery::setAccountId(const AccountId& accountId)
{
  mAccountId = accountId;
  mContractId.reset();

  return *this;
}

//-----
AccountBalanceQuery& AccountBalanceQuery::setContractId(const ContractId& contractId)
{
  mContractId = contractId;
  mAccountId.reset();

  return *this;
}

//-----
AccountBalance AccountBalanceQuery::mapResponse(const proto::Response& response) const
{
  AccountBalance accountBalance = AccountBalance::fromProtobuf(response.cryptogetaccountbalance());

  fetchTokenInformation(accountBalance);

  return accountBalance;
}

//-----
grpc::Status AccountBalanceQuery::submitRequest(const proto::Query& request,
                                                const std::shared_ptr<internal::Node>& node,
                                                const std::chrono::system_clock::time_point& deadline,
                                                proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kCryptogetAccountBalance, request, deadline, response);
}

//-----
void AccountBalanceQuery::validateChecksums(const Client& client) const
{
  if (mAccountId.has_value())
  {
    mAccountId->validateChecksum(client);
  }

  if (mContractId.has_value())
  {
    mContractId->validateChecksum(client);
  }
}

//-----
void AccountBalanceQuery::fetchTokenInformation(AccountBalance& accountBalance) const
{
  std::string param;
  if (mAccountId.has_value())
  {
    param = mAccountId.value().toString();
  }
  else if (mContractId.has_value())
  {
    param = mContractId.value().toString();
  }
  else
  {
    throw UninitializedException("Missing both accountId and contractId");
  }

  json tokens = internal::MirrorNodeGateway::MirrorNodeQuery(
    getMirrorNodeResolution(), { param }, internal::MirrorNodeGateway::TOKEN_RELATIONSHIPS_QUERY.data());

  if (!tokens["tokens"].empty())
  {
    for (const auto& token : tokens["tokens"])
    {
      std::string tokenIdStr = token["token_id"].dump();
      uint64_t tokenBalance = token["balance"];
      TokenId tokenId = TokenId::fromString(tokenIdStr.substr(1, tokenIdStr.length() - 2));
      accountBalance.mTokens.insert({ tokenId, tokenBalance });
    }
  }
}

//-----
proto::Query AccountBalanceQuery::buildRequest(proto::QueryHeader* header) const
{
  auto accountBalanceQuery = std::make_unique<proto::CryptoGetAccountBalanceQuery>();
  accountBalanceQuery->set_allocated_header(header);

  if (mAccountId)
  {
    accountBalanceQuery->set_allocated_accountid(mAccountId->toProtobuf().release());
  }

  if (mContractId)
  {
    accountBalanceQuery->set_allocated_contractid(mContractId->toProtobuf().release());
  }

  proto::Query query;
  query.set_allocated_cryptogetaccountbalance(accountBalanceQuery.release());
  return query;
}

//-----
proto::ResponseHeader AccountBalanceQuery::mapResponseHeader(const proto::Response& response) const
{
  saveCostFromHeader(response.cryptogetaccountbalance().header());
  return response.cryptogetaccountbalance().header();
}

} // namespace Hedera
