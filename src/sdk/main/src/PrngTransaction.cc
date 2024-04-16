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
#include "PrngTransaction.h"
#include "impl/Node.h"

#include <proto/transaction.pb.h>
#include <proto/util_prng.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
PrngTransaction::PrngTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<PrngTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
PrngTransaction::PrngTransaction(const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<PrngTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
PrngTransaction& PrngTransaction::setRange(int range)
{
  requireNotFrozen();
  mRange = range;
  return *this;
}

//-----
grpc::Status PrngTransaction::submitRequest(const proto::Transaction& request,
                                            const std::shared_ptr<internal::Node>& node,
                                            const std::chrono::system_clock::time_point& deadline,
                                            proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kUtilPrng, request, deadline, response);
}

//-----
void PrngTransaction::validateChecksums(const Client& client) const
{
  // Nothing to be validated.
}

//-----
void PrngTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_util_prng(build());
}

//-----
void PrngTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_util_prng())
  {
    throw std::invalid_argument("Transaction body doesn't contain UtilPrng data");
  }

  mRange = transactionBody.util_prng().range();
}

//-----
proto::UtilPrngTransactionBody* PrngTransaction::build() const
{
  auto body = std::make_unique<proto::UtilPrngTransactionBody>();
  body->set_range(mRange);
  return body.release();
}

} // namespace Hedera