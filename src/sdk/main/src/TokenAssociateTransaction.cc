/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "TokenAssociateTransaction.h"
#include "impl/Node.h"

#include <proto/token_associate.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenAssociateTransaction::TokenAssociateTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenAssociateTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenAssociateTransaction::TokenAssociateTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenAssociateTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenAssociateTransaction& TokenAssociateTransaction::setAccountId(const AccountId& accountId)
{
  requireNotFrozen();
  mAccountId = accountId;
  return *this;
}

//-----
TokenAssociateTransaction& TokenAssociateTransaction::setTokenIds(const std::vector<TokenId>& tokenIds)
{
  requireNotFrozen();
  mTokenIds = tokenIds;
  return *this;
}

//-----
grpc::Status TokenAssociateTransaction::submitRequest(const proto::Transaction& request,
                                                      const std::shared_ptr<internal::Node>& node,
                                                      const std::chrono::system_clock::time_point& deadline,
                                                      proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kTokenAssociate, request, deadline, response);
}

//-----
void TokenAssociateTransaction::validateChecksums(const Client& client) const
{
  if (mAccountId.has_value())
  {
    mAccountId->validateChecksum(client);
  }

  std::for_each(
    mTokenIds.cbegin(), mTokenIds.cend(), [&client](const TokenId& tokenId) { tokenId.validateChecksum(client); });
}

//-----
void TokenAssociateTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_tokenassociate(build());
}

//-----
void TokenAssociateTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_tokenassociate())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenAssociate data");
  }

  const proto::TokenAssociateTransactionBody& body = transactionBody.tokenassociate();

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
proto::TokenAssociateTransactionBody* TokenAssociateTransaction::build() const
{
  auto body = std::make_unique<proto::TokenAssociateTransactionBody>();

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
