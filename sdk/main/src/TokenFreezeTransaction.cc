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
#include "TokenFreezeTransaction.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/token_freeze_account.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenFreezeTransaction::TokenFreezeTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenFreezeTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenFreezeTransaction::TokenFreezeTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenFreezeTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenFreezeTransaction& TokenFreezeTransaction::setAccountId(const AccountId& accountId)
{
  requireNotFrozen();
  mAccountId = accountId;
  return *this;
}

//-----
TokenFreezeTransaction& TokenFreezeTransaction::setTokenId(const TokenId& tokenId)
{
  requireNotFrozen();
  mTokenId = tokenId;
  return *this;
}

//-----
grpc::Status TokenFreezeTransaction::submitRequest(const proto::Transaction& request,
                                                   const std::shared_ptr<internal::Node>& node,
                                                   const std::chrono::system_clock::time_point& deadline,
                                                   proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kTokenFreeze, request, deadline, response);
}

//-----
void TokenFreezeTransaction::validateChecksums(const Client& client) const
{
  mTokenId.validateChecksum(client);
  mAccountId.validateChecksum(client);
}

//-----
void TokenFreezeTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_tokenfreeze(build());
}

//-----
void TokenFreezeTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_tokenfreeze())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenFreeze data");
  }

  const proto::TokenFreezeAccountTransactionBody& body = transactionBody.tokenfreeze();

  if (body.has_account())
  {
    mAccountId = AccountId::fromProtobuf(body.account());
  }

  if (body.has_token())
  {
    mTokenId = TokenId::fromProtobuf(body.token());
  }
}

//-----
proto::TokenFreezeAccountTransactionBody* TokenFreezeTransaction::build() const
{
  auto body = std::make_unique<proto::TokenFreezeAccountTransactionBody>();

  if (!(mAccountId == AccountId()))
  {
    body->set_allocated_account(mAccountId.toProtobuf().release());
  }

  if (!(mTokenId == TokenId()))
  {
    body->set_allocated_token(mTokenId.toProtobuf().release());
  }

  return body.release();
}

} // namespace Hedera
