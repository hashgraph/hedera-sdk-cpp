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
#include "TokenDissociateTransaction.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/token_dissociate.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenDissociateTransaction::TokenDissociateTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenDissociateTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenDissociateTransaction::TokenDissociateTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenDissociateTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenDissociateTransaction& TokenDissociateTransaction::setAccountId(const AccountId& accountId)
{
  requireNotFrozen();
  mAccountId = accountId;
  return *this;
}

//-----
TokenDissociateTransaction& TokenDissociateTransaction::setTokenIds(const std::vector<TokenId>& tokenIds)
{
  requireNotFrozen();
  mTokenIds = tokenIds;
  return *this;
}

//-----
grpc::Status TokenDissociateTransaction::submitRequest(const proto::Transaction& request,
                                                       const std::shared_ptr<internal::Node>& node,
                                                       const std::chrono::system_clock::time_point& deadline,
                                                       proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kTokenDissociate, request, deadline, response);
}

//-----
void TokenDissociateTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_tokendissociate(build());
}

//-----
void TokenDissociateTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_tokendissociate())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenDissociate data");
  }

  const proto::TokenDissociateTransactionBody& body = transactionBody.tokendissociate();

  if (body.has_account())
  {
    mAccountId = AccountId::fromProtobuf(body.account());
  }

  for (int i = 0; i < body.tokens_size(); ++i)
  {
    mTokenIds.push_back(TokenId::fromProtobuf(body.tokens(i)));
  }
}

//-----
proto::TokenDissociateTransactionBody* TokenDissociateTransaction::build() const
{
  auto body = std::make_unique<proto::TokenDissociateTransactionBody>();

  if (mAccountId.has_value())
  {
    body->set_allocated_account(mAccountId->toProtobuf().release());
  }

  for (const auto& token : mTokenIds)
  {
    *body->add_tokens() = *token.toProtobuf();
  }

  return body.release();
}

} // namespace Hedera
