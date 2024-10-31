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
#include "NodeDeleteTransaction.h"
#include "TransactionId.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <node_delete.pb.h>
#include <stdexcept>
#include <transaction.pb.h>
#include <transaction_body.pb.h>

namespace Hedera
{
//-----
NodeDeleteTransaction::NodeDeleteTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<NodeDeleteTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
NodeDeleteTransaction::NodeDeleteTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<NodeDeleteTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
NodeDeleteTransaction& NodeDeleteTransaction::setNodeId(uint64_t nodeId)
{
  requireNotFrozen();
  mNodeId = nodeId;
  return *this;
}

//-----
grpc::Status NodeDeleteTransaction::submitRequest(const proto::Transaction& request,
                                                  const std::shared_ptr<internal::Node>& node,
                                                  const std::chrono::system_clock::time_point& deadline,
                                                  proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kNodeDelete, request, deadline, response);
}

//-----
void NodeDeleteTransaction::validateChecksums(const Client& client) const
{
  // No checksum to validate.
}

//-----
void NodeDeleteTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_nodedelete(build());
}

//-----
void NodeDeleteTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_nodedelete())
  {
    throw std::invalid_argument("Transaction body doesn't contain NodeDelete data");
  }

  const aproto::NodeDeleteTransactionBody& body = transactionBody.nodedelete();

  mNodeId = body.node_id();
}

//-----
aproto::NodeDeleteTransactionBody* NodeDeleteTransaction::build() const
{
  auto body = std::make_unique<aproto::NodeDeleteTransactionBody>();
  body->set_node_id(mNodeId);

  return body.release();
}

} // namespace Hedera
