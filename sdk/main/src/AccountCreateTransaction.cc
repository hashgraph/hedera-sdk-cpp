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
#include "AccountCreateTransaction.h"
#include "TransactionResponse.h"
#include "impl/DurationConverter.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/crypto_create.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_response.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
AccountCreateTransaction::AccountCreateTransaction()
  : Transaction<AccountCreateTransaction>()
{
  setMaxTransactionFee(Hbar(5LL));
}

//-----
std::unique_ptr<
  Executable<AccountCreateTransaction, proto::Transaction, proto::TransactionResponse, TransactionResponse>>
AccountCreateTransaction::clone() const
{
  return std::make_unique<AccountCreateTransaction>(*this);
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setKey(const std::shared_ptr<PublicKey>& key)
{
  mKey = key;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setInitialBalance(const Hbar& initialBalance)
{
  mInitialBalance = initialBalance;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setReceiverSignatureRequired(bool receiveSignatureRequired)
{
  mReceiverSignatureRequired = receiveSignatureRequired;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setAutoRenewPeriod(
  const std::chrono::duration<double>& autoRenewPeriod)
{
  mAutoRenewPeriod = autoRenewPeriod;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setAccountMemo(std::string_view memo)
{
  if (memo.size() > 100)
  {
    throw std::invalid_argument("Account memo is too large. Must be smaller than 100 bytes");
  }

  mAccountMemo = memo;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setMaxAutomaticTokenAssociations(uint32_t associations)
{
  if (associations > 1000)
  {
    throw std::invalid_argument("Too many maximum number of token associations. Maximum can't be over 1000");
  }

  mMaxAutomaticTokenAssociations = associations;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setStakedAccountId(const AccountId& stakedAccountId)
{
  mStakedAccountId = stakedAccountId;
  mStakedNodeId.reset();
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setStakedNodeId(const uint64_t& stakedNodeId)
{
  mStakedNodeId = stakedNodeId;
  mStakedAccountId.reset();
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setDeclineStakingReward(bool declineReward)
{
  mDeclineStakingReward = declineReward;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setAlias(const std::shared_ptr<PublicKey>& alias)
{
  mAlias = alias;
  return *this;
}

//-----
proto::Transaction AccountCreateTransaction::makeRequest(const Client& client,
                                                         const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody transactionBody = generateTransactionBody(client);
  transactionBody.set_allocated_cryptocreateaccount(build());

  return signTransaction(transactionBody, client);
}

//-----
grpc::Status AccountCreateTransaction::submitRequest(const Client& client,
                                                     const std::chrono::system_clock::time_point& deadline,
                                                     const std::shared_ptr<internal::Node>& node,
                                                     proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kCryptoCreateAccount, makeRequest(client, node), deadline, response);
}

//-----
proto::CryptoCreateTransactionBody* AccountCreateTransaction::build() const
{
  auto body = std::make_unique<proto::CryptoCreateTransactionBody>();

  if (mKey)
  {
    body->set_allocated_key(mKey->toProtobuf().release());
  }

  body->set_initialbalance(mInitialBalance.toTinybars());
  body->set_receiversigrequired(mReceiverSignatureRequired);
  body->set_allocated_autorenewperiod(
    internal::DurationConverter::toProtobuf(std::chrono::duration_cast<std::chrono::seconds>(mAutoRenewPeriod)));
  body->set_memo(mAccountMemo);
  body->set_max_automatic_token_associations(static_cast<int32_t>(mMaxAutomaticTokenAssociations));

  if (mStakedAccountId)
  {
    body->set_allocated_staked_account_id(mStakedAccountId->toProtobuf().release());
  }

  if (mStakedNodeId)
  {
    body->set_staked_node_id(static_cast<int64_t>(*mStakedNodeId));
  }

  body->set_decline_reward(mDeclineStakingReward);

  if (mAlias)
  {
    body->set_allocated_alias(new std::string(mAlias->toString()));
  }

  return body.release();
}

} // namespace Hedera
