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
#include "TokenRejectTransaction.h"
#include "AccountId.h"
#include "NftId.h"
#include "TokenId.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <stdexcept>
#include <token_reject.pb.h>
#include <transaction.pb.h>
#include <transaction_body.pb.h>

namespace Hedera
{
//-----
TokenRejectTransaction::TokenRejectTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenRejectTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenRejectTransaction::TokenRejectTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenRejectTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenRejectTransaction& TokenRejectTransaction::setOwner(const AccountId& owner)
{
  requireNotFrozen();
  mOwner = owner;
  return *this;
}

//-----
TokenRejectTransaction& TokenRejectTransaction::setFts(const std::vector<TokenId>& fts)
{
  requireNotFrozen();
  mFts = fts;
  return *this;
}

//-----
TokenRejectTransaction& TokenRejectTransaction::setNfts(const std::vector<NftId>& nfts)
{
  requireNotFrozen();
  mNfts = nfts;
  return *this;
}

//-----
grpc::Status TokenRejectTransaction::submitRequest(const proto::Transaction& request,
                                                   const std::shared_ptr<internal::Node>& node,
                                                   const std::chrono::system_clock::time_point& deadline,
                                                   proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kTokenReject, request, deadline, response);
}

//-----
void TokenRejectTransaction::validateChecksums(const Client& client) const
{
  if (mOwner.has_value())
  {
    mOwner.value().validateChecksum(client);
  }

  std::for_each(mFts.cbegin(), mFts.cend(), [&client](const TokenId& tokenId) { tokenId.validateChecksum(client); });
  std::for_each(
    mNfts.cbegin(), mNfts.cend(), [&client](const NftId& nftId) { nftId.mTokenId.validateChecksum(client); });
}

//-----
void TokenRejectTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_tokenreject(build());
}

//-----
void TokenRejectTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_tokenreject())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenReject data");
  }

  const proto::TokenRejectTransactionBody& body = transactionBody.tokenreject();

  if (body.has_owner())
  {
    mOwner = AccountId::fromProtobuf(body.owner());
  }

  for (int i = 0; i < body.rejections_size(); i++)
  {
    if (body.rejections(i).has_fungible_token())
    {
      mFts.push_back(TokenId::fromProtobuf(body.rejections(i).fungible_token()));
    }

    if (body.rejections(i).has_nft())
    {
      mNfts.push_back(NftId::fromProtobuf(body.rejections(i).nft()));
    }
  }
}

//-----
proto::TokenRejectTransactionBody* TokenRejectTransaction::build() const
{
  auto body = std::make_unique<proto::TokenRejectTransactionBody>();

  if (mOwner.has_value())
  {
    body->set_allocated_owner(mOwner.value().toProtobuf().release());
  }

  for (const TokenId& ft : mFts)
  {
    auto tr = std::make_unique<proto::TokenReference>();
    tr->set_allocated_fungible_token(ft.toProtobuf().release());
    body->mutable_rejections()->AddAllocated(tr.release());
  }

  for (const NftId& nft : mNfts)
  {
    auto tr = std::make_unique<proto::TokenReference>();
    tr->set_allocated_nft(nft.toProtobuf().release());
    body->mutable_rejections()->AddAllocated(tr.release());
  }

  return body.release();
}

} // namespace Hedera