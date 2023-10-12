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
#include "TokenUnpauseTransaction.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/token_unpause.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenUnpauseTransaction::TokenUnpauseTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenUnpauseTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenUnpauseTransaction::TokenUnpauseTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenUnpauseTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenUnpauseTransaction& TokenUnpauseTransaction::setTokenId(const TokenId& tokenId)
{
  requireNotFrozen();
  mTokenId = tokenId;
  return *this;
}

//-----
grpc::Status TokenUnpauseTransaction::submitRequest(const proto::Transaction& request,
                                                    const std::shared_ptr<internal::Node>& node,
                                                    const std::chrono::system_clock::time_point& deadline,
                                                    proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kTokenUnpause, request, deadline, response);
}

//-----
void TokenUnpauseTransaction::validateChecksums(const Client& client) const
{
  mTokenId.validateChecksum(client);
}

//-----
void TokenUnpauseTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_token_unpause(build());
}

//-----
void TokenUnpauseTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_token_unpause())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenUnpause data");
  }

  const proto::TokenUnpauseTransactionBody& body = transactionBody.token_unpause();

  if (body.has_token())
  {
    mTokenId = TokenId::fromProtobuf(body.token());
  }
}

//-----
proto::TokenUnpauseTransactionBody* TokenUnpauseTransaction::build() const
{
  auto body = std::make_unique<proto::TokenUnpauseTransactionBody>();

  if (!(mTokenId == TokenId()))
  {
    body->set_allocated_token(mTokenId.toProtobuf().release());
  }

  return body.release();
}

} // namespace Hedera
