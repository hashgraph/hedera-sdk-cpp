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
#include "FileCreateTransaction.h"
#include "impl/Node.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <grpcpp/client_context.h>
#include <proto/file_create.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
FileCreateTransaction::FileCreateTransaction()
{
  setMaxTransactionFee(Hbar(5LL));
}

//-----
FileCreateTransaction::FileCreateTransaction(const proto::TransactionBody& transactionBody)
{
  if (!transactionBody.has_filecreate())
  {
    throw std::invalid_argument("Transaction body doesn't contain FileCreate data");
  }

  const proto::FileCreateTransactionBody& body = transactionBody.filecreate();

  if (body.has_expirationtime())
  {
    mExpirationTime = internal::TimestampConverter::fromProtobuf(body.expirationtime());
  }

  if (body.has_keys() && body.keys().keys_size() > 0)
  {
    mKey = PublicKey::fromProtobuf(body.keys().keys(0));
  }

  mContents = internal::Utilities::stringToByteVector(body.contents());
  mFileMemo = body.memo();
}

//-----
FileCreateTransaction& FileCreateTransaction::setExpirationTime(
  const std::chrono::system_clock::time_point& expirationTime)
{
  requireNotFrozen();
  mExpirationTime = expirationTime;
  return *this;
}

//-----
FileCreateTransaction& FileCreateTransaction::setKey(const std::shared_ptr<PublicKey>& key)
{
  requireNotFrozen();
  mKey = key;
  return *this;
}

//-----
FileCreateTransaction& FileCreateTransaction::setContents(const std::vector<std::byte>& contents)
{
  requireNotFrozen();
  mContents = contents;
  return *this;
}

//-----
FileCreateTransaction& FileCreateTransaction::setFileMemo(std::string_view memo)
{
  requireNotFrozen();
  mFileMemo = memo;
  return *this;
}

//-----
proto::Transaction FileCreateTransaction::makeRequest(const Client& client,
                                                      const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody transactionBody = generateTransactionBody(client);
  transactionBody.set_allocated_filecreate(build());

  return signTransaction(transactionBody, client);
}

//-----
grpc::Status FileCreateTransaction::submitRequest(const Client& client,
                                                  const std::chrono::system_clock::time_point& deadline,
                                                  const std::shared_ptr<internal::Node>& node,
                                                  proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kFileCreate, makeRequest(client, node), deadline, response);
}

//-----
proto::FileCreateTransactionBody* FileCreateTransaction::build() const
{
  auto body = std::make_unique<proto::FileCreateTransactionBody>();
  body->set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(mExpirationTime));

  if (mKey)
  {
    auto keyList = std::make_unique<proto::KeyList>();
    *keyList->add_keys() = *mKey->toProtobuf();
    body->set_allocated_keys(keyList.release());
  }

  body->set_contents(internal::Utilities::byteVectorToString(mContents));
  body->set_memo(mFileMemo);

  return body.release();
}

} // namespace Hedera
