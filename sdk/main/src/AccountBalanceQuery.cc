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

#include <proto/crypto_get_account_balance.pb.h>
#include <proto/query.pb.h>
#include <proto/response.pb.h>

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
proto::Query AccountBalanceQuery::makeRequest() const
{
  proto::Query query;
  proto::CryptoGetAccountBalanceQuery* getAccountBalanceQuery = query.mutable_cryptogetaccountbalance();

  if (mAccountId)
  {
    getAccountBalanceQuery->set_allocated_accountid(mAccountId->toProtobuf().get());
  }

  if (mContractId)
  {
    getAccountBalanceQuery->set_allocated_contractid(mContractId->toProtobuf());
  }

  return query;
}

//-----
AccountBalance AccountBalanceQuery::mapResponse(const proto::Response& response) const
{
  return AccountBalance::fromProtobuf(response.cryptogetaccountbalance());
}

} // namespace Hedera