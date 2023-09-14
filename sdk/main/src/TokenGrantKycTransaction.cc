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
#include "TokenGrantKycTransaction.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/token_grant_kyc.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenGrantKycTransaction::TokenGrantKycTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenGrantKycTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenGrantKycTransaction::TokenGrantKycTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenGrantKycTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenGrantKycTransaction& TokenGrantKycTransaction::setAccountId(const AccountId& accountId)
{
  requireNotFrozen();
  mAccountId = accountId;
  return *this;
}

//-----
TokenGrantKycTransaction& TokenGrantKycTransaction::setTokenId(const TokenId& tokenId)
{
  requireNotFrozen();
  mTokenId = tokenId;
  return *this;
}

//-----
grpc::Status TokenGrantKycTransaction::submitRequest(const proto::Transaction& request,
                                                     const std::shared_ptr<internal::Node>& node,
                                                     const std::chrono::system_clock::time_point& deadline,
                                                     proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kTokenGrantKyc, request, deadline, response);
}

//-----
void TokenGrantKycTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_tokengrantkyc(build());
}

//-----
void TokenGrantKycTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_tokengrantkyc())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenGrantKyc data");
  }

  const proto::TokenGrantKycTransactionBody& body = transactionBody.tokengrantkyc();

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
proto::TokenGrantKycTransactionBody* TokenGrantKycTransaction::build() const
{
  auto body = std::make_unique<proto::TokenGrantKycTransactionBody>();

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
