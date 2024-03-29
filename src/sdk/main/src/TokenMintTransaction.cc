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
#include "TokenMintTransaction.h"
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <grpcpp/client_context.h>
#include <proto/token_mint.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenMintTransaction::TokenMintTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenMintTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenMintTransaction::TokenMintTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenMintTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenMintTransaction& TokenMintTransaction::setTokenId(const TokenId& tokenId)
{
  requireNotFrozen();
  mTokenId = tokenId;
  return *this;
}

//-----
TokenMintTransaction& TokenMintTransaction::setAmount(const uint64_t& amount)
{
  requireNotFrozen();
  mAmount = amount;
  return *this;
}

//-----
TokenMintTransaction& TokenMintTransaction::setMetadata(const std::vector<std::vector<std::byte>>& metadata)
{
  requireNotFrozen();
  mMetadataList = metadata;
  return *this;
}

//-----
TokenMintTransaction& TokenMintTransaction::addMetadata(const std::vector<std::byte>& metadata)
{
  requireNotFrozen();
  mMetadataList.push_back(metadata);
  return *this;
}

//-----
grpc::Status TokenMintTransaction::submitRequest(const proto::Transaction& request,
                                                 const std::shared_ptr<internal::Node>& node,
                                                 const std::chrono::system_clock::time_point& deadline,
                                                 proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kTokenMint, request, deadline, response);
}

//-----
void TokenMintTransaction::validateChecksums(const Client& client) const
{
  mTokenId.validateChecksum(client);
}

//-----
void TokenMintTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_tokenmint(build());
}

//-----
void TokenMintTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_tokenmint())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenMint data");
  }

  const proto::TokenMintTransactionBody& body = transactionBody.tokenmint();

  if (body.has_token())
  {
    mTokenId = TokenId::fromProtobuf(body.token());
  }

  mAmount = body.amount();

  for (int i = 0; i < body.metadata_size(); ++i)
  {
    mMetadataList.push_back(internal::Utilities::stringToByteVector(body.metadata(i)));
  }
}

//-----
proto::TokenMintTransactionBody* TokenMintTransaction::build() const
{
  auto body = std::make_unique<proto::TokenMintTransactionBody>();

  if (!(mTokenId == TokenId()))
  {
    body->set_allocated_token(mTokenId.toProtobuf().release());
  }

  body->set_amount(mAmount);

  for (const auto& metadata : mMetadataList)
  {
    body->add_metadata(internal::Utilities::byteVectorToString(metadata));
  }

  return body.release();
}

} // namespace Hedera
