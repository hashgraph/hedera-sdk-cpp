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
#include "TokenUnlockTransaction.h"

#include <grpcpp/client_context.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
TokenUnlockTransaction::TokenUnlockTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<TokenUnlockTransaction>(transactionBody)
{
  initFromSourceTransactionBody();
}

//-----
TokenUnlockTransaction::TokenUnlockTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<TokenUnlockTransaction>(transactions)
{
  initFromSourceTransactionBody();
}

//-----
TokenUnlockTransaction& TokenUnlockTransaction::setAccountId(const AccountId& accountId)
{
  requireNotFrozen();
  mAccountId = accountId;
  return *this;
}

//-----
TokenUnlockTransaction& TokenUnlockTransaction::setTokenId(const TokenId& tokenId)
{
  requireNotFrozen();
  mTokenId = tokenId;
  return *this;
}

//-----
TokenUnlockTransaction& TokenUnlockTransaction::setAmount(uint64_t amount)
{
  requireNotFrozen();
  mAmount = amount;
  mSerialNumber.reset();
  return *this;
}

//-----
TokenUnlockTransaction& TokenUnlockTransaction::setSerialNumber(uint64_t serialNumber)
{
  requireNotFrozen();
  mSerialNumber = serialNumber;
  mAmount.reset();
  return *this;
}

//-----
grpc::Status TokenUnlockTransaction::submitRequest(const proto::Transaction& request,
                                                   const std::shared_ptr<internal::Node>& node,
                                                   const std::chrono::system_clock::time_point& deadline,
                                                   proto::TransactionResponse* response) const
{
  return node->submitTransaction(proto::TransactionBody::DataCase::kUnlockToken, request, deadline, response);
}

//-----
void TokenUnlockTransaction::validateChecksums(const Client& client) const
{
  mAccountId.validateChecksum(client);
  mTokenId.validateChecksum(client);
}

//-----
void TokenUnlockTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_tokenunlock(build());
}

//-----
void TokenUnlockTransaction::initFromSourceTransactionBody()
{
  const proto::TransactionBody transactionBody = getSourceTransactionBody();

  if (!transactionBody.has_tokenunlock())
  {
    throw std::invalid_argument("Transaction body doesn't contain TokenUnlock data");
  }

  const proto::TokenUnlockTransactionBody& body = transactionBody.tokenunlock();

  mAccountId = AccountId::fromProtobuf(body.account_id());
  mTokenId = TokenId::fromProtobuf(body.token_id());

  if (body.has_amount())
  {
    mAmount = static_cast<uint64_t>(body.amount());
  }
  else if (body.has_serial_number())
  {
    mSerialNumber = static_cast<uint64_t>(body.serial_number());
  }
}

//-----
proto::TokenUnlockTransactionBody* TokenUnlockTransaction::build() const
{
  auto body = std::make_unique<proto::TokenUnlockTransactionBody>();
  body->set_allocated_account_id(mAccountId.toProtobuf().release());
  body->set_allocated_token_id(mTokenId.toProtobuf().release());

  if (mAmount.has_value())
  {
    body->set_amount(static_cast<int64_t>(mAmount.value()));
  }
  else if (mSerialNumber.has_value())
  {
    body->set_serial_number(static_cast<int64_t>(mSerialNumber.value()));
  }

  return body.release();
}

} // namespace Hedera
