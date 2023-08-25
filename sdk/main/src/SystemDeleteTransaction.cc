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
#include "SystemDeleteTransaction.h"
#include "impl/Node.h"
#include "impl/TimestampConverter.h"

#include <grpcpp/client_context.h>
#include <proto/system_delete.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
SystemDeleteTransaction::SystemDeleteTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<SystemDeleteTransaction>(transactionBody)
{
  if (!transactionBody.has_systemdelete())
  {
    throw std::invalid_argument("Transaction body doesn't contain SystemDelete data");
  }

  const proto::SystemDeleteTransactionBody& body = transactionBody.systemdelete();

  if (body.has_fileid())
  {
    mFileId = FileId::fromProtobuf(body.fileid());
  }
  else if (body.has_contractid())
  {
    mContractId = ContractId::fromProtobuf(body.contractid());
  }

  if (body.has_expirationtime())
  {
    mExpirationTime = internal::TimestampConverter::fromProtobuf(body.expirationtime());
  }
}

//-----
SystemDeleteTransaction& SystemDeleteTransaction::setFileId(const FileId& fileId)
{
  requireNotFrozen();
  mFileId = fileId;
  mContractId.reset();
  return *this;
}

//-----
SystemDeleteTransaction& SystemDeleteTransaction::setContractId(const ContractId& contractId)
{
  requireNotFrozen();
  mContractId = contractId;
  mFileId.reset();
  return *this;
}

//-----
SystemDeleteTransaction& SystemDeleteTransaction::setExpirationTime(
  const std::chrono::system_clock::time_point& timestamp)
{
  requireNotFrozen();
  mExpirationTime = timestamp;
  return *this;
}

//-----
proto::Transaction SystemDeleteTransaction::makeRequest(const Client& client,
                                                        const std::shared_ptr<internal::Node>&) const
{
  return signTransaction(generateTransactionBody(&client), client);
}

//-----
grpc::Status SystemDeleteTransaction::submitRequest(const Client& client,
                                                    const std::chrono::system_clock::time_point& deadline,
                                                    const std::shared_ptr<internal::Node>& node,
                                                    proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kSystemDelete, makeRequest(client, node), deadline, response);
}

//-----
void SystemDeleteTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_systemdelete(build());
}

//-----
proto::SystemDeleteTransactionBody* SystemDeleteTransaction::build() const
{
  auto body = std::make_unique<proto::SystemDeleteTransactionBody>();

  if (mFileId.has_value())
  {
    body->set_allocated_fileid(mFileId->toProtobuf().release());
  }
  else if (mContractId.has_value())
  {
    body->set_allocated_contractid(mContractId->toProtobuf().release());
  }

  body->set_allocated_expirationtime(internal::TimestampConverter::toSecondsProtobuf(mExpirationTime));
  return body.release();
}

} // namespace Hedera
