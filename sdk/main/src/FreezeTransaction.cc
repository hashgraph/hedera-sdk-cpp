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
#include "FreezeTransaction.h"
#include "impl/Node.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <grpcpp/client_context.h>
#include <proto/freeze.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
FreezeTransaction::FreezeTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<FreezeTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
FreezeTransaction::FreezeTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<FreezeTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
FreezeTransaction& FreezeTransaction::setFileId(const FileId& fileId)
{
  requireNotFrozen();
  mFileId = fileId;
  return *this;
}

//-----
FreezeTransaction& FreezeTransaction::setFileHash(const std::vector<std::byte>& hash)
{
  requireNotFrozen();
  mFileHash = hash;
  return *this;
}

//-----
FreezeTransaction& FreezeTransaction::setStartTime(const std::chrono::system_clock::time_point& time)
{
  requireNotFrozen();
  mStartTime = time;
  return *this;
}

//-----
FreezeTransaction& FreezeTransaction::setFreezeType(const FreezeType& type)
{
  requireNotFrozen();
  mFreezeType = type;
  return *this;
}

//-----
grpc::Status FreezeTransaction::submitRequest(const proto::Transaction& request,
                                              const std::shared_ptr<internal::Node>& node,
                                              const std::chrono::system_clock::time_point& deadline,
                                              proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kFreeze, request, deadline, response);
}

//-----
void FreezeTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_freeze(build());
}

//-----
void FreezeTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_freeze())
  {
    throw std::invalid_argument("Transaction body doesn't contain Freeze data");
  }

  const proto::FreezeTransactionBody& body = transactionBody.freeze();

  if (body.has_update_file())
  {
    mFileId = FileId::fromProtobuf(body.update_file());
  }

  mFileHash = internal::Utilities::stringToByteVector(body.file_hash());

  if (body.has_start_time())
  {
    mStartTime = internal::TimestampConverter::fromProtobuf(body.start_time());
  }

  mFreezeType = gProtobufFreezeTypeToFreezeType.at(body.freeze_type());
}

//-----
proto::FreezeTransactionBody* FreezeTransaction::build() const
{
  auto body = std::make_unique<proto::FreezeTransactionBody>();

  if (mFileId.has_value())
  {
    body->set_allocated_update_file(mFileId->toProtobuf().release());
  }

  body->set_file_hash(internal::Utilities::byteVectorToString(mFileHash));

  if (mStartTime.has_value())
  {
    body->set_allocated_start_time(internal::TimestampConverter::toProtobuf(mStartTime.value()));
  }

  body->set_freeze_type(gFreezeTypeToProtobufFreezeType.at(mFreezeType));

  return body.release();
}

} // namespace Hedera
