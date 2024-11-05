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
#include "ContractDeleteTransaction.h"
#include "impl/Node.h"

#include <contract_delete.pb.h>
#include <grpcpp/client_context.h>
#include <transaction.pb.h>
#include <transaction_body.pb.h>

namespace Hedera
{
//-----
ContractDeleteTransaction::ContractDeleteTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<ContractDeleteTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
ContractDeleteTransaction::ContractDeleteTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<ContractDeleteTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
ContractDeleteTransaction& ContractDeleteTransaction::setContractId(const ContractId& contractId)
{
  requireNotFrozen();
  mContractId = contractId;
  return *this;
}

//-----
ContractDeleteTransaction& ContractDeleteTransaction::setTransferAccountId(const AccountId& accountId)
{
  requireNotFrozen();
  mTransferAccountId = accountId;
  mTransferContractId.reset();
  return *this;
}

//-----
ContractDeleteTransaction& ContractDeleteTransaction::setTransferContractId(const ContractId& contractId)
{
  requireNotFrozen();
  mTransferContractId = contractId;
  mTransferAccountId.reset();
  return *this;
}

//-----
grpc::Status ContractDeleteTransaction::submitRequest(const proto::Transaction& request,
                                                      const std::shared_ptr<internal::Node>& node,
                                                      const std::chrono::system_clock::time_point& deadline,
                                                      proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kContractDeleteInstance, request, deadline, response);
}

//-----
void ContractDeleteTransaction::validateChecksums(const Client& client) const
{
  mContractId.validateChecksum(client);

  if (mTransferContractId.has_value())
  {
    mTransferContractId->validateChecksum(client);
  }

  if (mTransferAccountId.has_value())
  {
    mTransferAccountId->validateChecksum(client);
  }
}

//-----
void ContractDeleteTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_contractdeleteinstance(build());
}

//-----
void ContractDeleteTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_contractdeleteinstance())
  {
    throw std::invalid_argument("Transaction body doesn't contain ContractDeleteInstance data");
  }

  const proto::ContractDeleteTransactionBody& body = transactionBody.contractdeleteinstance();

  if (body.has_contractid())
  {
    mContractId = ContractId::fromProtobuf(body.contractid());
  }

  if (body.has_transferaccountid())
  {
    mTransferAccountId = AccountId::fromProtobuf(body.transferaccountid());
  }

  else if (body.has_transfercontractid())
  {
    mTransferContractId = ContractId::fromProtobuf(body.transfercontractid());
  }
}

//-----
proto::ContractDeleteTransactionBody* ContractDeleteTransaction::build() const
{
  auto body = std::make_unique<proto::ContractDeleteTransactionBody>();

  body->set_allocated_contractid(mContractId.toProtobuf().release());

  if (mTransferAccountId.has_value())
  {
    body->set_allocated_transferaccountid(mTransferAccountId->toProtobuf().release());
  }

  else if (mTransferContractId.has_value())
  {
    body->set_allocated_transfercontractid(mTransferContractId->toProtobuf().release());
  }

  return body.release();
}

} // namespace Hedera
