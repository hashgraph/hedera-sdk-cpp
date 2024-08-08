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
#include "NodeCreateTransaction.h"
#include "TransactionId.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/node_create.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
NodeCreateTransaction::NodeCreateTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<NodeCreateTransaction>(transactionBody)
{
  // Function implementation will go here
}

//-----
NodeCreateTransaction::NodeCreateTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<NodeCreateTransaction>(transactions)
{
  // Function implementation will go here
}

//-----
grpc::Status NodeCreateTransaction::submitRequest(const proto::Transaction& request,
                                                  const std::shared_ptr<internal::Node>& node,
                                                  const std::chrono::system_clock::time_point& deadline,
                                                  proto::TransactionResponse* response) const
{
  // Function implementation will go here
}

//-----
void NodeCreateTransaction::validateChecksums(const Client& client) const
{
  // Function implementation will go here
}

//-----
void NodeCreateTransaction::addToBody(proto::TransactionBody& body) const
{
  // Function implementation will go here
}

//-----
void NodeCreateTransaction::initFromSourceTransactionBody()
{
  // Function implementation will go here
}

//-----
proto::NodeCreateTransactionBody* NodeCreateTransaction::build() const
{
  // Function implementation will go here
}

} // namespace Hedera
