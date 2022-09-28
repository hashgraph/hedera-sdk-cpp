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
#include "AccountBalanceQuery.h"

#include "AccountBalance.h"

#include "basic_types.pb.h"
#include "crypto_get_account_balance.pb.h"
#include "query.pb.h"
#include "query_header.pb.h"
#include "response.pb.h"

namespace Hedera
{
//-----
AccountBalanceQuery::AccountBalanceQuery()
  : Query()
  , mAccountId()
  , mContractId()
{
}

//-----
AccountBalanceQuery::AccountBalanceQuery(const AccountId& accountId)
  : Query()
  , mContractId()
{
  mAccountId.setValue(accountId);
}

//-----
AccountBalanceQuery::AccountBalanceQuery(const ContractId& contractId)
  : Query()
  , mAccountId()
{
  mContractId.setValue(contractId);
}

//-----
void
AccountBalanceQuery::onMakeRequest(proto::Query* query,
                                   proto::QueryHeader* header) const
{
  proto::CryptoGetAccountBalanceQuery* getAccountBalanceQuery =
    query->mutable_cryptogetaccountbalance();

  if (mAccountId.isValid())
  {
    getAccountBalanceQuery->set_allocated_accountid(
      mAccountId.getValue().toProtobuf());
  }

  if (mContractId.isValid())
  {
    getAccountBalanceQuery->set_allocated_contractid(
      mContractId.getValue().toProtobuf());
  }

  getAccountBalanceQuery->set_allocated_header(header);
}

//-----
proto::ResponseHeader
AccountBalanceQuery::mapResponseHeader(proto::Response* response) const
{
  return response->cryptogetaccountbalance().header();
}

//-----
void
AccountBalanceQuery::validateChecksums(const Client& client) const
{
  if (mAccountId.isValid())
  {
    mAccountId.getValue().validateChecksum(client);
  }

  if (mContractId.isValid())
  {
    mContractId.getValue().validateChecksum(client);
  }
}

//-----
AccountBalance
AccountBalanceQuery::mapResponse(const proto::Response& response,
                                 const AccountId& accountId,
                                 const proto::Query& query) const
{
  return AccountBalance::fromProtobuf(response.cryptogetaccountbalance());
}

//-----
proto::QueryHeader
AccountBalanceQuery::mapRequestHeader(const proto::Query& query) const
{
  return query.cryptogetaccountbalance().header();
}

//-----
AccountBalanceQuery&
AccountBalanceQuery::setAccountId(const AccountId& accountId)
{
  mAccountId.setValue(accountId);
  return *this;
}

//-----
AccountBalanceQuery&
AccountBalanceQuery::setContractId(const ContractId& contractId)
{
  mContractId.setValue(contractId);
  return *this;
}

} // namespace Hedera