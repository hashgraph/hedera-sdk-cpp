/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
  : Transaction<FileDeleteTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
FileDeleteTransaction::FileDeleteTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<FileDeleteTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
FileDeleteTransaction& FileDeleteTransaction::setFileId(const FileId& fileId)
{
  requireNotFrozen();
  mFileId = fileId;
  return *this;
}

//-----
grpc::Status FileDeleteTransaction::submitRequest(const proto::Transaction& request,
                                                  const std::shared_ptr<internal::Node>& node,
                                                  const std::chrono::system_clock::time_point& deadline,
                                                  proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kFileDelete, request, deadline, response);
}

//-----
void FileDeleteTransaction::validateChecksums(const Client& client) const
{
  mFileId.validateChecksum(client);
}

//-----
void FileDeleteTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_filedelete(build());
}

//-----
void FileDeleteTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

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
proto::FileDeleteTransactionBody* FileDeleteTransaction::build() const
{
  auto body = std::make_unique<proto::FileDeleteTransactionBody>();
  body->set_allocated_fileid(mFileId.toProtobuf().release());
  return body.release();
}

} // namespace Hedera
