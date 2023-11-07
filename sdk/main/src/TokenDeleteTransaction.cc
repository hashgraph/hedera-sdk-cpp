/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "TokenDeleteTransaction.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/token_delete.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenDeleteTransaction::TokenDeleteTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenDeleteTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenDeleteTransaction::TokenDeleteTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenDeleteTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenDeleteTransaction& TokenDeleteTransaction::setTokenId(const TokenId& tokenId)
{
  requireNotFrozen();
  mTokenId = tokenId;
  return *this;
}

//-----
grpc::Status TokenDeleteTransaction::submitRequest(const proto::Transaction& request,
                                                   const std::shared_ptr<internal::Node>& node,
                                                   const std::chrono::system_clock::time_point& deadline,
                                                   proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kTokenDeletion, request, deadline, response);
}

//-----
void TokenDeleteTransaction::validateChecksums(const Client& client) const
{
  if (mTokenId.has_value())
  {
    mTokenId->validateChecksum(client);
  }
}

//-----
void TokenDeleteTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_tokendeletion(build());
}

//-----
void TokenDeleteTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_tokendeletion())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenDelete data");
  }

  const proto::TokenDeleteTransactionBody& body = transactionBody.tokendeletion();

  if (body.has_token())
  {
    mTokenId = TokenId::fromProtobuf(body.token());
  }
}

//-----
proto::TokenDeleteTransactionBody* TokenDeleteTransaction::build() const
{
  auto body = std::make_unique<proto::TokenDeleteTransactionBody>();

  if (mTokenId.has_value())
  {
    body->set_allocated_token(mTokenId->toProtobuf().release());
  }

  return body.release();
}

} // namespace Hedera