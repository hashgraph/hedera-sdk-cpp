/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#include "Node.h"

#include <proto/crypto_get_account_balance.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
AccountBalanceQuery& AccountBalanceQuery::setAccountId(const AccountId& accountId)
{
  mAccountId = std::make_unique<AccountId>(accountId);
  mContractId.reset();

  return *this;
}

//-----
AccountBalanceQuery& AccountBalanceQuery::setContractId(const ContractId& contractId)
{
  mContractId = std::make_unique<ContractId>(contractId);
  mAccountId.reset();

  return *this;
}

//-----
std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)>
AccountBalanceQuery::getGrpcMethod(const std::shared_ptr<Node>& node) const
{
  return node->getGrpcQueryMethod(proto::Query::QueryCase::kCryptogetAccountBalance);
}

//-----
proto::Query AccountBalanceQuery::makeRequest(const Client&, const std::shared_ptr<Node>&) const
{
  proto::Query query;
  proto::CryptoGetAccountBalanceQuery* getAccountBalanceQuery = query.mutable_cryptogetaccountbalance();

  proto::QueryHeader* header = getAccountBalanceQuery->mutable_header();
  header->set_responsetype(proto::ANSWER_ONLY);
  // This is a free query, so no payment required

  if (mAccountId)
  {
    getAccountBalanceQuery->set_allocated_accountid(mAccountId->toProtobuf());
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
