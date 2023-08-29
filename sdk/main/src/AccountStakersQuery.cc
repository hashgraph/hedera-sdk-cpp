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
#include "AccountStakersQuery.h"

#include "AccountId.h"
#include "AccountInfo.h"
#include "ProxyStaker.h"

#include <proto/crypto_get_stakers.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
AccountStakersQuery::AccountStakersQuery()
  : mAccountId()
{
}

//-----
void
AccountStakersQuery::validateChecksums(const Client& client) const
{
  if (mAccountId.isValid())
  {
    mAccountId.getValue().validateChecksum(client);
  }
}

//-----
void
AccountStakersQuery::onMakeRequest(proto::Query* query,
                                   proto::QueryHeader* header) const
{
  proto::CryptoGetStakersQuery* getStakersQuery =
    query->mutable_cryptogetproxystakers();

  if (mAccountId.isValid())
  {
    getStakersQuery->set_allocated_accountid(
      mAccountId.getValue().toProtobuf());
  }

  getStakersQuery->set_allocated_header(header);
}

//-----
proto::ResponseHeader
AccountStakersQuery::mapResponseHeader(proto::Response* response) const
{
  return response->cryptogetproxystakers().header();
}

//-----
proto::QueryHeader
AccountStakersQuery::mapRequestHeader(const proto::Query& query) const
{
  return query.cryptogetproxystakers().header();
}

//-----
std::vector<ProxyStaker>
AccountStakersQuery::mapResponse(const proto::Response& response,
                                 const AccountId& accountId,
                                 const proto::Query& query) const
{
  std::vector<ProxyStaker> stakers;
  const proto::AllProxyStakers& stakerData =
    response.cryptogetproxystakers().stakers();
  for (size_t i = 0; i < stakerData.proxystaker_size(); ++i)
  {
    stakers.push_back(ProxyStaker::fromProtobuf(stakerData.proxystaker(i)));
  }

  return stakers;
}

//-----
AccountStakersQuery&
AccountStakersQuery::setAccountId(const AccountId& accountId)
{
  mAccountId.setValue(accountId);
  return *this;
}

} // namespace Hedera
