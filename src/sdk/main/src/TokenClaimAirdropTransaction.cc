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
#include "TokenClaimAirdropTransaction.h"
#include "AccountId.h"
#include "NftId.h"
#include "TokenId.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/token_claim_airdrop.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenClaimAirdropTransaction::TokenClaimAirdropTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenClaimAirdropTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenClaimAirdropTransaction::TokenClaimAirdropTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenClaimAirdropTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
grpc::Status TokenClaimAirdropTransaction::submitRequest(const proto::Transaction& request,
                                                         const std::shared_ptr<internal::Node>& node,
                                                         const std::chrono::system_clock::time_point& deadline,
                                                         proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kTokenReject, request, deadline, response);
}

//-----
void TokenClaimAirdropTransaction::validateChecksums(const Client& client) const {}

//-----
void TokenClaimAirdropTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_tokenclaimairdrop(build());
}

//-----
void TokenClaimAirdropTransaction::initFromSourceTransactionBody() {}

//-----
proto::TokenClaimAirdropTransactionBody* TokenClaimAirdropTransaction::build() const {}

} // namespace Hedera