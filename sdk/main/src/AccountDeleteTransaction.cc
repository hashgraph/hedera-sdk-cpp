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
#include "AccountDeleteTransaction.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/crypto_delete.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
AccountDeleteTransaction::AccountDeleteTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<AccountDeleteTransaction>(transactionBody)
{
  if (!transactionBody.has_cryptodelete())
  {
    throw std::invalid_argument("Transaction body doesn't contain CryptoDelete data");
  }

  const proto::CryptoDeleteTransactionBody& body = transactionBody.cryptodelete();

  if (body.has_deleteaccountid())
  {
    mDeleteAccountId = AccountId::fromProtobuf(body.deleteaccountid());
  }

  if (body.has_transferaccountid())
  {
    mTransferAccountId = AccountId::fromProtobuf(body.transferaccountid());
  }
}

//-----
AccountDeleteTransaction& AccountDeleteTransaction::setDeleteAccountId(const AccountId& accountId)
{
  requireNotFrozen();

  mDeleteAccountId = accountId;
  return *this;
}

//-----
AccountDeleteTransaction& AccountDeleteTransaction::setTransferAccountId(const AccountId& accountId)
{
  requireNotFrozen();

  mTransferAccountId = accountId;
  return *this;
}

//-----
proto::Transaction AccountDeleteTransaction::makeRequest(const Client& client,
                                                         const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody txBody = generateTransactionBody(&client);
  addToBody(txBody);
  return signTransaction(txBody, client);
}

//-----
grpc::Status AccountDeleteTransaction::submitRequest(const Client& client,
                                                     const std::chrono::system_clock::time_point& deadline,
                                                     const std::shared_ptr<internal::Node>& node,
                                                     proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kCryptoDelete, makeRequest(client, node), deadline, response);
}

//-----
void AccountDeleteTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_cryptodelete(build());
}

//-----
proto::CryptoDeleteTransactionBody* AccountDeleteTransaction::build() const
{
  auto body = std::make_unique<proto::CryptoDeleteTransactionBody>();

  if (mDeleteAccountId.has_value())
  {
    body->set_allocated_deleteaccountid(mDeleteAccountId->toProtobuf().release());
  }

  if (mTransferAccountId.has_value())
  {
    body->set_allocated_transferaccountid(mTransferAccountId->toProtobuf().release());
  }

  return body.release();
}

} // namespace Hedera