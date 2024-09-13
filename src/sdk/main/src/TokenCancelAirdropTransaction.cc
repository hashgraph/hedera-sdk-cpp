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
#include "TokenCancelAirdropTransaction.h"
#include "AccountId.h"
#include "NftId.h"
#include "TokenId.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/token_cancel_airdrop.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenCancelAirdropTransaction::TokenCancelAirdropTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenCancelAirdropTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenCancelAirdropTransaction::TokenCancelAirdropTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenCancelAirdropTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
grpc::Status TokenCancelAirdropTransaction::submitRequest(const proto::Transaction& request,
                                                          const std::shared_ptr<internal::Node>& node,
                                                          const std::chrono::system_clock::time_point& deadline,
                                                          proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kTokenCancelAirdrop, request, deadline, response);
}

//-----
void TokenCancelAirdropTransaction::validateChecksums(const Client& client) const {}

//-----
void TokenCancelAirdropTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_tokencancelairdrop(build());
}

//-----
void TokenCancelAirdropTransaction::initFromSourceTransactionBody() {}

//-----
proto::TokenCancelAirdropTransactionBody* TokenCancelAirdropTransaction::build() const {}

} // namespace Hedera