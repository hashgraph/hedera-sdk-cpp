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
#include "ContractCallQuery.h"

#include "ContractFunctionResult.h"

#include <proto/contract_call.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
ContractCallQuery::ContractCallQuery()
  : mContractId()
  , mGas(0)
  , mFunctionParameters()
  , mSenderAccountId()
{
}

//-----
void
ContractCallQuery::validateChecksums(const Client& client) const
{
  if (mContractId.isValid())
  {
    mContractId.getValue().validateChecksum(client);
  }
}

//-----
void
ContractCallQuery::onMakeRequest(proto::Query* query,
                                 proto::QueryHeader* header) const
{
  proto::ContractCallLocalQuery* contractQuery =
    query->mutable_contractcalllocal();

  if (mContractId.isValid())
  {
    contractQuery->set_allocated_contractid(
      mContractId.getValue().toProtobuf());
  }

  contractQuery->set_gas(mGas);
  contractQuery->set_functionparameters(mFunctionParameters);

  if (mSenderAccountId.isValid())
  {
    contractQuery->set_allocated_sender_id(
      mSenderAccountId.getValue().toProtobuf());
  }
}

//-----
proto::ResponseHeader
ContractCallQuery::mapResponseHeader(proto::Response* response) const
{
  return response->contractcalllocal().header();
}

//-----
proto::QueryHeader
ContractCallQuery::mapRequestHeader(const proto::Query& query) const
{
  return query.contractcalllocal().header();
}

//-----
ContractFunctionResult
ContractCallQuery::mapResponse(const proto::Response& response,
                               const AccountId& accountId,
                               const proto::Query& query) const
{
  return ContractFunctionResult::fromProtobuf(
    response.contractcalllocal().functionresult());
}

//-----
ContractCallQuery&
ContractCallQuery::setContractId(const ContractId& contractId)
{
  mContractId.setValue(contractId);
  return *this;
}

//-----
ContractCallQuery&
ContractCallQuery::setGas(const int64_t& gas)
{
  mGas = gas;
  return *this;
}

//-----
ContractCallQuery&
ContractCallQuery::setFunctionParameters(const std::string& functionParameters)
{
  mFunctionParameters = functionParameters;
  return *this;
}

//-----
ContractCallQuery&
ContractCallQuery::setFunction(const std::string& name)
{
  return setFunction(name, ContractFunctionParameters());
}

//-----
ContractCallQuery&
ContractCallQuery::setFunction(const std::string& name,
                               const ContractFunctionParameters& params)
{
  setFunctionParameters(params.toByteArray(name));
  return *this;
}

//-----
ContractCallQuery&
ContractCallQuery::setSenderAccountId(const AccountId& senderAccountId)
{
  mSenderAccountId.setValue(senderAccountId);
  return *this;
}

} // namespace Hedera
