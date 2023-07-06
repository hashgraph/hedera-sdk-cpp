/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#include "TokenFeeScheduleUpdateTransaction.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/token_fee_schedule_update.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
TokenFeeScheduleUpdateTransaction::TokenFeeScheduleUpdateTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenFeeScheduleUpdateTransaction>(transactionBody)
{
  if (!transactionBody.has_token_fee_schedule_update())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenFeeScheduleUpdate data");
  }

  const proto::TokenFeeScheduleUpdateTransactionBody& body = transactionBody.token_fee_schedule_update();

  if (body.has_token_id())
  {
    mTokenId = TokenId::fromProtobuf(body.token_id());
  }

  for (int i = 0; i < body.custom_fees_size(); ++i)
  {
    mCustomFees.push_back(CustomFee::fromProtobuf(body.custom_fees(i)));
  }
}

//-----
TokenFeeScheduleUpdateTransaction& TokenFeeScheduleUpdateTransaction::setTokenId(const TokenId& tokenId)
{
  requireNotFrozen();
  mTokenId = tokenId;
  return *this;
}

//-----
TokenFeeScheduleUpdateTransaction& TokenFeeScheduleUpdateTransaction::setCustomFees(
  const std::vector<std::shared_ptr<CustomFee>>& fees)
{
  requireNotFrozen();
  mCustomFees = fees;
  return *this;
}

//-----
proto::Transaction TokenFeeScheduleUpdateTransaction::makeRequest(const Client& client,
                                                                  const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody transactionBody = generateTransactionBody(client);
  transactionBody.set_allocated_token_fee_schedule_update(build());

  return signTransaction(transactionBody, client);
}

//-----
grpc::Status TokenFeeScheduleUpdateTransaction::submitRequest(const Client& client,
                                                              const std::chrono::system_clock::time_point& deadline,
                                                              const std::shared_ptr<internal::Node>& node,
                                                              proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kTokenFeeScheduleUpdate, makeRequest(client, node), deadline, response);
}

//-----
proto::TokenFeeScheduleUpdateTransactionBody* TokenFeeScheduleUpdateTransaction::build() const
{
  auto body = std::make_unique<proto::TokenFeeScheduleUpdateTransactionBody>();

  if (!(mTokenId == TokenId()))
  {
    body->set_allocated_token_id(mTokenId.toProtobuf().release());
  }

  for (const auto& fee : mCustomFees)
  {
    *body->add_custom_fees() = *fee->toProtobuf();
  }

  return body.release();
}

} // namespace Hedera