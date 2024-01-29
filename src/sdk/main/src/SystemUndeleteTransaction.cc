/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "SystemUndeleteTransaction.h"
#include "impl/Node.h"
#include "impl/TimestampConverter.h"

#include <grpcpp/client_context.h>
#include <proto/system_undelete.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
SystemUndeleteTransaction::SystemUndeleteTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<SystemUndeleteTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
SystemUndeleteTransaction::SystemUndeleteTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<SystemUndeleteTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
SystemUndeleteTransaction& SystemUndeleteTransaction::setFileId(const FileId& fileId)
{
  requireNotFrozen();
  mFileId = fileId;
  mContractId.reset();
  return *this;
}

//-----
SystemUndeleteTransaction& SystemUndeleteTransaction::setContractId(const ContractId& contractId)
{
  requireNotFrozen();
  mContractId = contractId;
  mFileId.reset();
  return *this;
}

//-----
grpc::Status SystemUndeleteTransaction::submitRequest(const proto::Transaction& request,
                                                      const std::shared_ptr<internal::Node>& node,
                                                      const std::chrono::system_clock::time_point& deadline,
                                                      proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kSystemUndelete, request, deadline, response);
}

//-----
void SystemUndeleteTransaction::validateChecksums(const Client& client) const
{
  if (mFileId.has_value())
  {
    mFileId->validateChecksum(client);
  }

  if (mContractId.has_value())
  {
    mContractId->validateChecksum(client);
  }
}

//-----
void SystemUndeleteTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_systemundelete(build());
}

//-----
void SystemUndeleteTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_systemundelete())
  {
    throw std::invalid_argument("Transaction body doesn't contain SystemUndelete data");
  }

  const proto::SystemUndeleteTransactionBody& body = transactionBody.systemundelete();

  if (body.has_fileid())
  {
    mFileId = FileId::fromProtobuf(body.fileid());
  }
  else if (body.has_contractid())
  {
    mContractId = ContractId::fromProtobuf(body.contractid());
  }
}

//-----
proto::SystemUndeleteTransactionBody* SystemUndeleteTransaction::build() const
{
  auto body = std::make_unique<proto::SystemUndeleteTransactionBody>();

  if (mFileId.has_value())
  {
    body->set_allocated_fileid(mFileId->toProtobuf().release());
  }
  else if (mContractId.has_value())
  {
    body->set_allocated_contractid(mContractId->toProtobuf().release());
  }

  return body.release();
}

} // namespace Hedera
