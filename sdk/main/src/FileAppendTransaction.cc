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
#include "FileAppendTransaction.h"
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <grpcpp/client_context.h>
#include <proto/file_append.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
FileAppendTransaction::FileAppendTransaction()
{
  setMaxTransactionFee(Hbar(5LL));
  setChunkSize(2048U);
}

//-----
FileAppendTransaction::FileAppendTransaction(const proto::TransactionBody& transactionBody)
{
  if (!transactionBody.has_fileappend())
  {
    throw std::invalid_argument("Transaction body doesn't contain FileAppend data");
  }

  const proto::FileAppendTransactionBody& body = transactionBody.fileappend();

  if (body.has_fileid())
  {
    mFileId = FileId::fromProtobuf(body.fileid());
  }

  setData(internal::Utilities::stringToByteVector(body.contents()));
}

//-----
FileAppendTransaction& FileAppendTransaction::setFileId(const FileId& fileId)
{
  requireNotFrozen();
  mFileId = fileId;
  return *this;
}

//-----
FileAppendTransaction& FileAppendTransaction::setContents(const std::vector<std::byte>& contents)
{
  requireNotFrozen();
  setData(contents);
  return *this;
}

//-----
FileAppendTransaction& FileAppendTransaction::setContents(std::string_view contents)
{
  requireNotFrozen();
  setData(internal::Utilities::stringToByteVector(contents));
  return *this;
}

//-----
proto::Transaction FileAppendTransaction::makeRequest(const Client& client,
                                                      const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody transactionBody = generateTransactionBody(client);
  transactionBody.set_allocated_fileappend(build());

  return signTransaction(transactionBody, client);
}

//-----
grpc::Status FileAppendTransaction::submitRequest(const Client& client,
                                                  const std::chrono::system_clock::time_point& deadline,
                                                  const std::shared_ptr<internal::Node>& node,
                                                  proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kFileAppend, makeRequest(client, node), deadline, response);
}

//-----
proto::FileAppendTransactionBody* FileAppendTransaction::build() const
{
  auto body = std::make_unique<proto::FileAppendTransactionBody>();
  body->set_allocated_fileid(mFileId.toProtobuf().release());
  body->set_contents(internal::Utilities::byteVectorToString(getData()));
  return body.release();
}

} // namespace Hedera
