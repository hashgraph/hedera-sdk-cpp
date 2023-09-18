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
#include "AccountRecordsQuery.h"
#include "AccountRecords.h"
#include "impl/Node.h"

#include <proto/crypto_get_account_records.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
AccountRecordsQuery& AccountRecordsQuery::setAccountId(const AccountId& accountId)
{
  mAccountId = accountId;
  return *this;
}

//-----
AccountRecords AccountRecordsQuery::mapResponse(const proto::Response& response) const
{
  return AccountRecords::fromProtobuf(response.cryptogetaccountrecords());
}

//-----
grpc::Status AccountRecordsQuery::submitRequest(const proto::Query& request,
                                                const std::shared_ptr<internal::Node>& node,
                                                const std::chrono::system_clock::time_point& deadline,
                                                proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kCryptoGetAccountRecords, request, deadline, response);
}

//-----
proto::Query AccountRecordsQuery::buildRequest(proto::QueryHeader* header) const
{
  auto accountRecordsQuery = std::make_unique<proto::CryptoGetAccountRecordsQuery>();
  accountRecordsQuery->set_allocated_header(header);
  accountRecordsQuery->set_allocated_accountid(mAccountId.toProtobuf().release());

  proto::Query query;
  query.set_allocated_cryptogetaccountrecords(accountRecordsQuery.release());
  return query;
}

//-----
proto::ResponseHeader AccountRecordsQuery::mapResponseHeader(const proto::Response& response) const
{
  saveCostFromHeader(response.cryptogetaccountrecords().header());
  return response.cryptogetaccountrecords().header();
}

} // namespace Hedera
