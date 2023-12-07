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
#include "TokenUpdatePartitionTransaction.h"

#include <grpcpp/client_context.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
TokenUpdatePartitionTransaction::TokenUpdatePartitionTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenUpdatePartitionTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenUpdatePartitionTransaction::TokenUpdatePartitionTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenUpdatePartitionTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenUpdatePartitionTransaction& TokenUpdatePartitionTransaction::setPartitionId(const TokenId& partitionId)
{
  requireNotFrozen();
  mPartitionId = partitionId;
  return *this;
}

//-----
TokenUpdatePartitionTransaction& TokenUpdatePartitionTransaction::setPartitionName(std::string_view name)
{
  requireNotFrozen();
  mPartitionName = name;
  return *this;
}

//-----
TokenUpdatePartitionTransaction& TokenUpdatePartitionTransaction::setPartitionMemo(std::string_view memo)
{
  requireNotFrozen();
  mPartitionMemo = memo;
  return *this;
}

//-----
grpc::Status TokenUpdatePartitionTransaction::submitRequest(const proto::Transaction& request,
                                                            const std::shared_ptr<internal::Node>& node,
                                                            const std::chrono::system_clock::time_point& deadline,
                                                            proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kUpdatePartition, request, deadline, response);
}

//-----
void TokenUpdatePartitionTransaction::validateChecksums(const Client& client) const
{
  mPartitionId.validateChecksum(client);
}

//-----
void TokenUpdatePartitionTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_updatepartition(build());
}

//-----
void TokenUpdatePartitionTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_updatepartition())
  {
    throw std::invalid_argument("Transaction body doesn't contain UpdatePartition data");
  }

  const proto::TokenUpdatePartitionDefinitionTransactionBody& body = transactionBody.updatepartition();

  mPartitionId = TokenId::fromProtobuf(body.token());
  mPartitionName = body.name();
  mPartitionMemo = body.memo();
}

//-----
proto::TokenUpdatePartitionDefinitionTransactionBody* TokenUpdatePartitionTransaction::build() const
{
  auto body = std::make_unique<proto::TokenUpdatePartitionDefinitionTransactionBody>();
  body->set_allocated_token(mPartitionId.toProtobuf().release());
  body->set_name(mPartitionName);
  body->set_memo(mPartitionMemo);
  return body.release();
}

} // namespace Hedera
