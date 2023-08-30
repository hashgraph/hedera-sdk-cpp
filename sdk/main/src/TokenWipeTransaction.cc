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
#include "TokenWipeTransaction.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/token_wipe_account.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenWipeTransaction::TokenWipeTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenWipeTransaction>(transactionBody)
{
  if (!transactionBody.has_tokenwipe())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenWipe data");
  }

  const proto::TokenWipeAccountTransactionBody& body = transactionBody.tokenwipe();

  if (body.has_token())
  {
    mTokenId = TokenId::fromProtobuf(body.token());
  }

  if (body.has_account())
  {
    mAccountId = AccountId::fromProtobuf(body.account());
  }

  mAmount = body.amount();

  for (int i = 0; i < body.serialnumbers_size(); ++i)
  {
    mSerialNumbers.push_back(static_cast<uint64_t>(body.serialnumbers(i)));
  }
}

//-----
TokenWipeTransaction& TokenWipeTransaction::setTokenId(const TokenId& tokenId)
{
  requireNotFrozen();
  mTokenId = tokenId;
  return *this;
}

//-----
TokenWipeTransaction& TokenWipeTransaction::setAccountId(const AccountId& accountId)
{
  requireNotFrozen();
  mAccountId = accountId;
  return *this;
}

//-----
TokenWipeTransaction& TokenWipeTransaction::setAmount(uint64_t amount)
{
  requireNotFrozen();
  mAmount = amount;
  return *this;
}

//-----
TokenWipeTransaction& TokenWipeTransaction::setSerialNumbers(const std::vector<uint64_t>& serialNumbers)
{
  requireNotFrozen();
  mSerialNumbers = serialNumbers;
  return *this;
}

//-----
proto::Transaction TokenWipeTransaction::makeRequest(const Client& client, const std::shared_ptr<internal::Node>&) const
{
  return signTransaction(generateTransactionBody(&client), client);
}

//-----
grpc::Status TokenWipeTransaction::submitRequest(const Client& client,
                                                 const std::chrono::system_clock::time_point& deadline,
                                                 const std::shared_ptr<internal::Node>& node,
                                                 proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kTokenWipe, makeRequest(client, node), deadline, response);
}

//-----
void TokenWipeTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_tokenwipe(build());
}

//-----
proto::TokenWipeAccountTransactionBody* TokenWipeTransaction::build() const
{
  auto body = std::make_unique<proto::TokenWipeAccountTransactionBody>();

  if (!(mTokenId == TokenId()))
  {
    body->set_allocated_token(mTokenId.toProtobuf().release());
  }

  if (!(mAccountId == AccountId()))
  {
    body->set_allocated_account(mAccountId.toProtobuf().release());
  }

  body->set_amount(mAmount);

  for (const auto& num : mSerialNumbers)
  {
    body->add_serialnumbers(static_cast<int64_t>(num));
  }

  return body.release();
}

} // namespace Hedera