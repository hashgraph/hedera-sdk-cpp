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
#include "TokenRevokeKycTransaction.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/token_revoke_kyc.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenRevokeKycTransaction::TokenRevokeKycTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenRevokeKycTransaction>(transactionBody)
{
  if (!transactionBody.has_tokenrevokekyc())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenRevokeKyc data");
  }

  const proto::TokenRevokeKycTransactionBody& body = transactionBody.tokenrevokekyc();

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
TokenRevokeKycTransaction& TokenRevokeKycTransaction::setAccountId(const AccountId& accountId)
{
  requireNotFrozen();
  mAccountId = accountId;
  return *this;
}

//-----
TokenRevokeKycTransaction& TokenRevokeKycTransaction::setTokenId(const TokenId& tokenId)
{
  requireNotFrozen();
  mTokenId = tokenId;
  return *this;
}

//-----
proto::Transaction TokenRevokeKycTransaction::makeRequest(const Client& client,
                                                          const std::shared_ptr<internal::Node>&) const
{
  return signTransaction(generateTransactionBody(&client), client);
}

//-----
grpc::Status TokenRevokeKycTransaction::submitRequest(const Client& client,
                                                      const std::chrono::system_clock::time_point& deadline,
                                                      const std::shared_ptr<internal::Node>& node,
                                                      proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kTokenRevokeKyc, makeRequest(client, node), deadline, response);
}

//-----
void TokenRevokeKycTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_tokenrevokekyc(build());
}

//-----
proto::TokenRevokeKycTransactionBody* TokenRevokeKycTransaction::build() const
{
  auto body = std::make_unique<proto::TokenRevokeKycTransactionBody>();

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
