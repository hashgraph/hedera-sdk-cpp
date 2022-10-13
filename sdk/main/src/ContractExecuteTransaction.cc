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
#include "ContractExecuteTransaction.h"

#include "ContractFunctionParameters.h"

#include <proto/contract_call.pb.h>

namespace Hedera
{
//-----
ContractExecuteTransaction::ContractExecuteTransaction()
  : Transaction()
  , mContractId()
  , mGas(0LL)
  , mPayableAmount(0LL)
  , mFunctionParameters(std::string())
{
}

//-----
ContractExecuteTransaction::ContractExecuteTransaction(
  const std::unordered_map<
    TransactionId,
    std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  : Transaction(transactions)
{
  initFromTransactionBody();
}

//-----
ContractExecuteTransaction::ContractExecuteTransaction(
  const proto::TransactionBody& transaction)
{
  initFromTransactionBody();
}

//-----
void
ContractExecuteTransaction::validateChecksums(const Client& client) const
{
  if (mContractId.isValid())
  {
    mContractId.getValue().validateChecksum(client);
  }
}

//-----
proto::ContractCallTransactionBody
ContractExecuteTransaction::build() const
{
  proto::ContractCallTransactionBody body;

  if (mContractId.isValid())
  {
    body.set_allocated_contractid(mContractId.getValue().toProtobuf());
  }

  body.set_gas(mGas);
  body.set_amount(mPayableAmount.toTinybars());
  body.set_functionparameters(mFunctionParameters);

  return body;
}

//-----
ContractExecuteTransaction&
ContractExecuteTransaction::setContractId(const ContractId& contractId)
{
  requireNotFrozen();

  mContractId.setValue(contractId);
  return *this;
}

//-----
ContractExecuteTransaction&
ContractExecuteTransaction::setGas(const int64_t& gas)
{
  requireNotFrozen();

  mGas = gas;
  return *this;
}

//-----
ContractExecuteTransaction&
ContractExecuteTransaction::setPayableAmount(const Hbar& amount)
{
  requireNotFrozen();

  mPayableAmount = amount;
  return *this;
}

//-----
ContractExecuteTransaction&
ContractExecuteTransaction::setFunctionParameters(
  const std::string& functionParameters)
{
  requireNotFrozen();

  mFunctionParameters = functionParameters;
  return *this;
}

//-----
ContractExecuteTransaction&
ContractExecuteTransaction::setFunction(const std::string name)
{
  return setFunction(name, ContractFunctionParameters());
}

//-----
ContractExecuteTransaction&
ContractExecuteTransaction::setFunction(
  const std::string& name,
  const ContractFunctionParameters& params)
{
  return setFunctionParameters(params.toByteArray(name));
}

//-----
void
ContractExecuteTransaction::initFromTransactionBody()
{
  if (mSourceTransactionBody.has_contractcall())
  {
    const proto::ContractCallTransactionBody& body =
      mSourceTransactionBody.contractcall();

    if (body.has_contractid())
    {
      mContractId.setValue(ContractId::fromProtobuf(body.contractid()));
    }

    mGas = body.gas();
    mPayableAmount = Hbar::fromTinybars(body.amount());
    mFunctionParameters = body.functionparameters();
  }
}

} // namespace Hedera