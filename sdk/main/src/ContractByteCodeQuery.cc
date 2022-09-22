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
#include "ContractByteCodeQuery.h"

#include "ContractId.h"

#include "contract_get_bytecode.pb.h"
#include "query.pb.h"
#include "query_header.pb.h"
#include "response.pb.h"

namespace Hedera
{
//-----
ContractByteCodeQuery::ContractByteCodeQuery()
  : mContractId()
{
}

//-----
void
ContractByteCodeQuery::validateChecksums(const Client& client) const
{
  if (mContractId.isValid())
  {
    mContractId.getValue().validateChecksum(client);
  }
}

//-----
void
ContractByteCodeQuery::onMakeRequest(proto::Query* query,
                                     proto::QueryHeader* header) const
{
  proto::ContractGetBytecodeQuery* getByteCodeQuery =
    query->mutable_contractgetbytecode();

  if (mContractId.isValid())
  {
    getByteCodeQuery->set_allocated_contractid(
      mContractId.getValue().toProtobuf());
  }

  getByteCodeQuery->set_allocated_header(header);
}

//-----
proto::ResponseHeader
ContractByteCodeQuery::mapResponseHeader(proto::Response* response) const
{
  return response->contractgetbytecoderesponse().header();
}

//-----
proto::QueryHeader
ContractByteCodeQuery::mapRequestHeader(const proto::Query& query) const
{
  return query.contractgetbytecode().header();
}

//-----
std::string
ContractByteCodeQuery::mapResponse(const proto::Response& response,
                                   const AccountId& accountId,
                                   const proto::Query& query) const
{
  return response.contractgetbytecoderesponse().bytecode();
}

//-----
ContractByteCodeQuery&
ContractByteCodeQuery::setContractId(const ContractId& contractId)
{
  mContractId.setValue(contractId);
  return *this;
}

} // namespace Hedera