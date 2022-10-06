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

/**
 * Library includes
 */
#include "AccountBalance.h"

/**
 * Protobuf includes
 */
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
    proto::AccountID* accountId = getAccountBalanceQuery->mutable_accountid();
    accountId->set_realmnum(mAccountId.getValue().mRealm);
    accountId->set_shardnum(mAccountId.getValue().mShard);

    if (mAccountId.getValue().mAccountNum.isValid())
    {
      accountId->set_accountnum(mAccountId.getValue().mAccountNum.getValue());
    }

    if (mAccountId.getValue().mAlias.isValid())
    {
      accountId->set_alias(mAccountId.getValue().mAlias.getValue());
    }
  }

  if (mContractId.isValid())
  {
    proto::ContractID* contractId =
      getAccountBalanceQuery->mutable_contractid();
    contractId->set_realmnum(mContractId.getValue().mRealm);
    contractId->set_shardnum(mContractId.getValue().mShard);

    if (mContractId.getValue().mContractNum.isValid())
    {
      contractId->set_contractnum(
        mContractId.getValue().mContractNum.getValue());
    }

    if (mContractId.getValue().mEvmAddress.isValid())
    {
      contractId->set_evm_address(
        mContractId.getValue().mEvmAddress.getValue());
    }
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

} // namespace Hedera