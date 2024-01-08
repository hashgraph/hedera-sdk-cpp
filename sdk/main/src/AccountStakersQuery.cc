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
#include "AccountStakersQuery.h"
#include "Client.h"
#include "ProxyStaker.h"
#include "TransactionRecord.h"
#include "TransferTransaction.h"
#include "impl/Node.h"

#include <proto/crypto_get_stakers.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
AccountStakersQuery& AccountStakersQuery::setAccountId(const AccountId& accountId)
{
  mAccountId = accountId;
  return *this;
}

//-----
AccountStakers AccountStakersQuery::mapResponse(const proto::Response& response) const
{
  AccountStakers accountStakers;
  for (int i = 0; i < response.cryptogetproxystakers().stakers().proxystaker_size(); ++i)
  {
    accountStakers.push_back(ProxyStaker::fromProtobuf(response.cryptogetproxystakers().stakers().proxystaker(i)));
  }

  return accountStakers;
}

//-----
grpc::Status AccountStakersQuery::submitRequest(const proto::Query& request,
                                                const std::shared_ptr<internal::Node>& node,
                                                const std::chrono::system_clock::time_point& deadline,
                                                proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kCryptoGetProxyStakers, request, deadline, response);
}

//-----
void AccountStakersQuery::validateChecksums(const Client& client) const
{
  mAccountId.validateChecksum(client);
}

//-----
proto::Query AccountStakersQuery::buildRequest(proto::QueryHeader* header) const
{
  auto accountStakersQuery = std::make_unique<proto::CryptoGetStakersQuery>();
  accountStakersQuery->set_allocated_header(header);
  accountStakersQuery->set_allocated_accountid(mAccountId.toProtobuf().release());

  proto::Query query;
  query.set_allocated_cryptogetproxystakers(accountStakersQuery.release());
  return query;
}

//-----
proto::ResponseHeader AccountStakersQuery::mapResponseHeader(const proto::Response& response) const
{
  saveCostFromHeader(response.cryptogetproxystakers().header());
  return response.cryptogetproxystakers().header();
}

} // namespace Hedera
