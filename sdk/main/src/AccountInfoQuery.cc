/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#include "AccountInfoQuery.h"

#include "AccountId.h"
#include "AccountInfo.h"

#include <proto/crypto_get_info.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
AccountInfoQuery::AccountInfoQuery()
  : mAccountId()
{
}

//-----
void
AccountInfoQuery::validateChecksums(const Client& client) const
{
  if (mAccountId.isValid())
  {
    mAccountId.getValue().validateChecksum(client);
  }
}

//-----
void
AccountInfoQuery::onMakeRequest(proto::Query* query,
                                proto::QueryHeader* header) const
{
  proto::CryptoGetInfoQuery* getInfoQuery = query->mutable_cryptogetinfo();

  if (mAccountId.isValid())
  {
    getInfoQuery->set_allocated_accountid(mAccountId.getValue().toProtobuf());
  }

  getInfoQuery->set_allocated_header(header);
}

//-----
proto::ResponseHeader
AccountInfoQuery::mapResponseHeader(proto::Response* response) const
{
  return response->cryptogetinfo().header();
}

//-----
proto::QueryHeader
AccountInfoQuery::mapRequestHeader(const proto::Query& query) const
{
  return query.cryptogetinfo().header();
}

//-----
AccountInfo
AccountInfoQuery::mapResponse(const proto::Response& response,
                              const AccountId& accountId,
                              const proto::Query& query) const
{
  return AccountInfo::fromProtobuf(response.cryptogetinfo().accountinfo());
}

//-----
AccountInfoQuery&
AccountInfoQuery::setAccountId(const AccountId& accountId)
{
  mAccountId.setValue(accountId);
  return *this;
}

} // namespace Hedera
