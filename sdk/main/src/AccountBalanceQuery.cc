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
#include "Status.h"
#include "impl/Node.h"

#include <proto/crypto_get_account_balance.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
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
proto::Query AccountBalanceQuery::makeRequest(const Client&, const std::shared_ptr<internal::Node>&) const
{
  proto::Query query;
  proto::CryptoGetAccountBalanceQuery* getAccountBalanceQuery = query.mutable_cryptogetaccountbalance();

  proto::QueryHeader* header = getAccountBalanceQuery->mutable_header();
  header->set_responsetype(proto::ANSWER_ONLY);
  // This is a free query, so no payment required

  if (mAccountId)
  {
    getAccountBalanceQuery->set_allocated_accountid(mAccountId->toProtobuf().release());
  }

  if (mContractId)
  {
    getAccountBalanceQuery->set_allocated_contractid(mContractId->toProtobuf().release());
  }

  return query;
}

//-----
AccountBalance AccountBalanceQuery::mapResponse(const proto::Response& response) const
{
  return AccountBalance::fromProtobuf(response.cryptogetaccountbalance());
}

//-----
Status AccountBalanceQuery::mapResponseStatus(const proto::Response& response) const
{
  return gProtobufResponseCodeToStatus.at(response.cryptogetaccountbalance().header().nodetransactionprecheckcode());
}

//-----
grpc::Status AccountBalanceQuery::submitRequest(const Client& client,
                                                const std::chrono::system_clock::time_point& deadline,
                                                const std::shared_ptr<internal::Node>& node,
                                                proto::Response* response) const
{
  return node->submitQuery(
    proto::Query::QueryCase::kCryptogetAccountBalance, makeRequest(client, node), deadline, response);
}

} // namespace Hedera
