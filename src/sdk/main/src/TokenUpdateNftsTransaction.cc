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

#include "TokenUpdateNftsTransaction.h"
#include "impl/DurationConverter.h"
#include "impl/Node.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <grpcpp/client_context.h>
#include <proto/token_update_nfts.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>
#include <stdexcept>

namespace Hedera
{

//-----
TokenUpdateNftsTransaction::TokenUpdateNftsTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenUpdateNftsTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenUpdateNftsTransaction::TokenUpdateNftsTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenUpdateNftsTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenUpdateNftsTransaction& TokenUpdateNftsTransaction::setTokenId(const TokenId& tokenId)
{
  requireNotFrozen();
  mTokenId = tokenId;
  return *this;
}

//-----
TokenUpdateNftsTransaction& TokenUpdateNftsTransaction::setSerials(const std::vector<uint64_t>& serials)
{
  requireNotFrozen();
  mSerials = serials;
  return *this;
}

//-----
TokenUpdateNftsTransaction& TokenUpdateNftsTransaction::setMetadata(const std::vector<std::byte>& metadata)
{
  requireNotFrozen();
  mMetadata = metadata;
  return *this;
}

//-----
grpc::Status TokenUpdateNftsTransaction::submitRequest(const proto::Transaction& request,
                                                       const std::shared_ptr<internal::Node>& node,
                                                       const std::chrono::system_clock::time_point& deadline,
                                                       proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kTokenUpdate, request, deadline, response);
}

//-----
void TokenUpdateNftsTransaction::validateChecksums(const Client& client) const
{
  mTokenId.validateChecksum(client);
}

//-----
void TokenUpdateNftsTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_token_update_nfts(build());
}

//-----
void TokenUpdateNftsTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_token_update_nfts())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenUpdateNfts data");
  }

  const proto::TokenUpdateNftsTransactionBody& body = transactionBody.token_update_nfts();

  if (body.has_token())
  {
    mTokenId = TokenId::fromProtobuf(body.token());
  }

  for (int i = 0; i < body.serial_numbers_size(); i++)
  {
    mSerials.push_back(static_cast<uint64_t>(body.serial_numbers(i)));
  }

  mMetadata = internal::Utilities::stringToByteVector(body.metadata().value());
}

//-----
proto::TokenUpdateNftsTransactionBody* TokenUpdateNftsTransaction::build() const
{
  auto body = std::make_unique<proto::TokenUpdateNftsTransactionBody>();
  body->set_allocated_token(mTokenId.toProtobuf().release());

  for (const uint64_t& serialNumber : mSerials)
  {
    body->add_serial_numbers(static_cast<int64_t>(serialNumber));
  }

  body->mutable_metadata()->set_value(internal::Utilities::byteVectorToString(mMetadata));

  return body.release();
}

} // namespace Hedera