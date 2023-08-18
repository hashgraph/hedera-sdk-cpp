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
#include "FileDeleteTransaction.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/file_delete.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
FileDeleteTransaction::FileDeleteTransaction(const proto::TransactionBody& transactionBody)
{
  if (!transactionBody.has_filedelete())
  {
    throw std::invalid_argument("Transaction body doesn't contain FileDelete data");
  }

  const proto::FileDeleteTransactionBody& body = transactionBody.filedelete();

  if (body.has_fileid())
  {
    mFileId = FileId::fromProtobuf(body.fileid());
  }
}

//-----
FileDeleteTransaction& FileDeleteTransaction::setFileId(const FileId& fileId)
{
  requireNotFrozen();
  mFileId = fileId;
  return *this;
}

//-----
proto::Transaction FileDeleteTransaction::makeRequest(const Client& client,
                                                      const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody txBody = generateTransactionBody(&client);
  addToBody(txBody);
  return signTransaction(txBody, client);
}

//-----
grpc::Status FileDeleteTransaction::submitRequest(const Client& client,
                                                  const std::chrono::system_clock::time_point& deadline,
                                                  const std::shared_ptr<internal::Node>& node,
                                                  proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kFileDelete, makeRequest(client, node), deadline, response);
}

//-----
void FileDeleteTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_filedelete(build());
}

//-----
proto::FileDeleteTransactionBody* FileDeleteTransaction::build() const
{
  auto body = std::make_unique<proto::FileDeleteTransactionBody>();
  body->set_allocated_fileid(mFileId.toProtobuf().release());
  return body.release();
}

} // namespace Hedera
