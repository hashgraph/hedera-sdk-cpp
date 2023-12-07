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
#include "TokenDeletePartitionTransaction.h"

#include <grpcpp/client_context.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
TokenDeletePartitionTransaction::TokenDeletePartitionTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenDeletePartitionTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenDeletePartitionTransaction::TokenDeletePartitionTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenDeletePartitionTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenDeletePartitionTransaction& TokenDeletePartitionTransaction::setPartitionId(const TokenId& tokenId)
{
  requireNotFrozen();
  mPartitionId = tokenId;
  return *this;
}

//-----
grpc::Status TokenDeletePartitionTransaction::submitRequest(const proto::Transaction& request,
                                                            const std::shared_ptr<internal::Node>& node,
                                                            const std::chrono::system_clock::time_point& deadline,
                                                            proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kDeletePartition, request, deadline, response);
}

//-----
void TokenDeletePartitionTransaction::validateChecksums(const Client& client) const
{
  mPartitionId.validateChecksum(client);
}

//-----
void TokenDeletePartitionTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_deletepartition(build());
}

//-----
void TokenDeletePartitionTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_deletepartition())
  {
    throw std::invalid_argument("Transaction body doesn't contain DeletePartition data");
  }

  const proto::TokenDeletePartitionDefinitionTransactionBody& body = transactionBody.deletepartition();

  mPartitionId = TokenId::fromProtobuf(body.token());
}

//-----
proto::TokenDeletePartitionDefinitionTransactionBody* TokenDeletePartitionTransaction::build() const
{
  auto body = std::make_unique<proto::TokenDeletePartitionDefinitionTransactionBody>();
  body->set_allocated_token(mPartitionId.toProtobuf().release());
  return body.release();
}

} // namespace Hedera
