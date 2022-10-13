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
#include "ContractInfoQuery.h"

#include "ContractInfo.h"

#include <proto/contract_get_info.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
ContractInfoQuery::ContractInfoQuery()
  : mContractId()
{
}

//-----
void
ContractInfoQuery::validateChecksums(const Client& client) const
{
  if (mContractId.isValid())
  {
    mContractId.getValue().validateChecksum(client);
  }
}

//-----
void
ContractInfoQuery::onMakeRequest(proto::Query* query,
                                 proto::QueryHeader* header) const
{
  proto::ContractGetInfoQuery* getInfoQuery = query->mutable_contractgetinfo();

  if (mContractId.isValid())
  {
    getInfoQuery->set_allocated_contractid(mContractId.getValue().toProtobuf());
  }

  getInfoQuery->set_allocated_header(header);
}

//-----
proto::ResponseHeader
ContractInfoQuery::mapResponseHeader(proto::Response* response) const
{
  return response->contractgetinfo().header();
}

//-----
proto::QueryHeader
ContractInfoQuery::mapRequestHeader(const proto::Query& query) const
{
  return query.contractgetinfo().header();
}

//-----
ContractInfo
ContractInfoQuery::mapResponse(const proto::Response& response,
                               const AccountId& accountId,
                               const proto::Query& query) const
{
  return ContractInfo::fromProtobuf(response.contractgetinfo().contractinfo());
}

//-----
ContractInfoQuery&
ContractInfoQuery::setContractId(const ContractId& contractId)
{
  mContractId.setValue(contractId);
  return *this;
}

} // namespace Hedera
