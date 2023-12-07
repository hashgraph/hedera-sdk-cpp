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
#include "TokenCreatePartitionTransaction.h"

#include <grpcpp/client_context.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
TokenCreatePartitionTransaction::TokenCreatePartitionTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenCreatePartitionTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenCreatePartitionTransaction::TokenCreatePartitionTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenCreatePartitionTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenCreatePartitionTransaction& TokenCreatePartitionTransaction::setParentTokenId(const TokenId& tokenId)
{
  requireNotFrozen();
  mParentTokenId = tokenId;
  return *this;
}

//-----
TokenCreatePartitionTransaction& TokenCreatePartitionTransaction::setPartitionName(std::string_view name)
{
  requireNotFrozen();
  mPartitionName = name;
  return *this;
}

//-----
TokenCreatePartitionTransaction& TokenCreatePartitionTransaction::setPartitionMemo(std::string_view memo)
{
  requireNotFrozen();
  mPartitionMemo = memo;
  return *this;
}

//-----
grpc::Status TokenCreatePartitionTransaction::submitRequest(const proto::Transaction& request,
                                                            const std::shared_ptr<internal::Node>& node,
                                                            const std::chrono::system_clock::time_point& deadline,
                                                            proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kCreatePartition, request, deadline, response);
}

//-----
void TokenCreatePartitionTransaction::validateChecksums(const Client& client) const
{
  mParentTokenId.validateChecksum(client);
}

//-----
void TokenCreatePartitionTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_createpartition(build());
}

//-----
void TokenCreatePartitionTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_createpartition())
  {
    throw std::invalid_argument("Transaction body doesn't contain CreatePartition data");
  }

  const proto::TokenCreatePartitionDefinitionTransactionBody& body = transactionBody.createpartition();

  mParentTokenId = TokenId::fromProtobuf(body.parent_token_id());
  mPartitionName = body.name();
  mPartitionMemo = body.memo();
}

//-----
proto::TokenCreatePartitionDefinitionTransactionBody* TokenCreatePartitionTransaction::build() const
{
  auto body = std::make_unique<proto::TokenCreatePartitionDefinitionTransactionBody>();
  body->set_allocated_parent_token_id(mParentTokenId.toProtobuf().release());
  body->set_name(mPartitionName);
  body->set_memo(mPartitionMemo);
  return body.release();
}

} // namespace Hedera
