/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "Client.h"
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <proto/contract_call.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_response.pb.h>

namespace Hedera
{
//-----
ContractExecuteTransaction::ContractExecuteTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<ContractExecuteTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
ContractExecuteTransaction::ContractExecuteTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<ContractExecuteTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
ContractExecuteTransaction& ContractExecuteTransaction::setContractId(const ContractId& contractId)
{
  requireNotFrozen();
  mContractId = contractId;
  return *this;
}

//-----
ContractExecuteTransaction& ContractExecuteTransaction::setGas(const uint64_t& gas)
{
  requireNotFrozen();
  mGas = gas;
  return *this;
}

//-----
ContractExecuteTransaction& ContractExecuteTransaction::setPayableAmount(const Hbar& amount)
{
  requireNotFrozen();
  mPayableAmount = amount;
  return *this;
}

//-----
ContractExecuteTransaction& ContractExecuteTransaction::setFunctionParameters(const std::vector<std::byte>& parameters)
{
  requireNotFrozen();
  mFunctionParameters = parameters;
  return *this;
}

//-----
ContractExecuteTransaction& ContractExecuteTransaction::setFunction(std::string_view name,
                                                                    const ContractFunctionParameters& parameters)
{
  requireNotFrozen();
  mFunctionParameters = parameters.toBytes(name);
  return *this;
}

//-----
grpc::Status ContractExecuteTransaction::submitRequest(const proto::Transaction& request,
                                                       const std::shared_ptr<internal::Node>& node,
                                                       const std::chrono::system_clock::time_point& deadline,
                                                       proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kContractCall, request, deadline, response);
}

//-----
void ContractExecuteTransaction::validateChecksums(const Client& client) const
{
  mContractId.validateChecksum(client);
}

//-----
void ContractExecuteTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_contractcall(build());
}

//-----
void ContractExecuteTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_contractcall())
  {
    throw std::invalid_argument("Transaction body doesn't contain ContractCall data");
  }

  const proto::ContractCallTransactionBody& body = transactionBody.contractcall();

  if (body.has_contractid())
  {
    mContractId = ContractId::fromProtobuf(body.contractid());
  }

  mGas = static_cast<uint64_t>(body.gas());
  mPayableAmount = Hbar(body.amount(), HbarUnit::TINYBAR());
  mFunctionParameters = internal::Utilities::stringToByteVector(body.functionparameters());
}

//-----
proto::ContractCallTransactionBody* ContractExecuteTransaction::build() const
{
  auto body = std::make_unique<proto::ContractCallTransactionBody>();
  body->set_allocated_contractid(mContractId.toProtobuf().release());
  body->set_gas(static_cast<int64_t>(mGas));
  body->set_amount(mPayableAmount.toTinybars());
  body->set_functionparameters(internal::Utilities::byteVectorToString(mFunctionParameters));
  return body.release();
}

} // namespace Hedera
