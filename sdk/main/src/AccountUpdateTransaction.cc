/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include "AccountUpdateTransaction.h"
#include "impl/DurationConverter.h"
#include "impl/Node.h"
#include "impl/TimestampConverter.h"

#include <proto/crypto_update.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
AccountUpdateTransaction::AccountUpdateTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<AccountUpdateTransaction>(transactionBody)
{
  if (!transactionBody.has_cryptoupdateaccount())
  {
    throw std::invalid_argument("Transaction body doesn't contain CryptoUpdate data");
  }

  const proto::CryptoUpdateTransactionBody& body = transactionBody.cryptoupdateaccount();

  if (body.has_accountidtoupdate())
  {
    mAccountId = AccountId::fromProtobuf(body.accountidtoupdate());
  }

  if (body.has_key())
  {
    mKey = ValuePtr<Key, KeyCloner>(Key::fromProtobuf(body.key()).release());
  }

  if (body.has_receiversigrequiredwrapper())
  {
    mReceiverSignatureRequired = body.receiversigrequiredwrapper().value();
  }

  if (body.has_autorenewperiod())
  {
    mAutoRenewPeriod = internal::DurationConverter::fromProtobuf(body.autorenewperiod());
  }

  if (body.has_expirationtime())
  {
    mExpirationTime = internal::TimestampConverter::fromProtobuf(body.expirationtime());
  }

  if (body.has_memo())
  {
    mAccountMemo = body.memo().value();
  }

  if (body.has_max_automatic_token_associations())
  {
    mMaxAutomaticTokenAssociations = body.max_automatic_token_associations().value();
  }

  if (body.has_staked_account_id())
  {
    mStakedAccountId = AccountId::fromProtobuf(body.staked_account_id());
  }

  if (body.has_staked_node_id())
  {
    mStakedNodeId = static_cast<uint64_t>(body.staked_node_id());
  }

  if (body.has_decline_reward())
  {
    mDeclineStakingReward = body.decline_reward().value();
  }
}

//-----
AccountUpdateTransaction& AccountUpdateTransaction::setAccountId(const AccountId& accountId)
{
  requireNotFrozen();

  mAccountId = accountId;
  return *this;
}

//-----
AccountUpdateTransaction& AccountUpdateTransaction::setKey(const Key* key)
{
  requireNotFrozen();

  mKey = ValuePtr<Key, KeyCloner>(key->clone().release());
  return *this;
}

//-----
AccountUpdateTransaction& AccountUpdateTransaction::setReceiverSignatureRequired(bool receiveSignatureRequired)
{
  requireNotFrozen();

  mReceiverSignatureRequired = receiveSignatureRequired;
  return *this;
}

//-----
AccountUpdateTransaction& AccountUpdateTransaction::setAutoRenewPeriod(
  const std::chrono::duration<double>& autoRenewPeriod)
{
  requireNotFrozen();

  mAutoRenewPeriod = autoRenewPeriod;
  return *this;
}

//-----
AccountUpdateTransaction& AccountUpdateTransaction::setExpirationTime(
  const std::chrono::system_clock::time_point& expiration)
{
  requireNotFrozen();

  mExpirationTime = expiration;
  return *this;
}

//-----
AccountUpdateTransaction& AccountUpdateTransaction::setAccountMemo(std::string_view memo)
{
  requireNotFrozen();

  if (memo.size() > 100)
  {
    throw std::length_error("Account memo is too large. Must be smaller than 100 bytes");
  }

  mAccountMemo = memo;
  return *this;
}

//-----
AccountUpdateTransaction& AccountUpdateTransaction::setMaxAutomaticTokenAssociations(uint32_t associations)
{
  requireNotFrozen();

  if (associations > 5000U)
  {
    throw std::invalid_argument("Too many maximum number of token associations. Maximum can't be over 5000");
  }

  mMaxAutomaticTokenAssociations = associations;
  return *this;
}

//-----
AccountUpdateTransaction& AccountUpdateTransaction::setStakedAccountId(const AccountId& stakedAccountId)
{
  requireNotFrozen();

  mStakedAccountId = stakedAccountId;
  mStakedNodeId.reset();
  return *this;
}

//-----
AccountUpdateTransaction& AccountUpdateTransaction::setStakedNodeId(const uint64_t& stakedNodeId)
{
  requireNotFrozen();

  mStakedNodeId = stakedNodeId;
  mStakedAccountId.reset();
  return *this;
}

//-----
AccountUpdateTransaction& AccountUpdateTransaction::setDeclineStakingReward(bool declineReward)
{
  requireNotFrozen();

  mDeclineStakingReward = declineReward;
  return *this;
}

//-----
proto::Transaction AccountUpdateTransaction::makeRequest(const Client& client,
                                                         const std::shared_ptr<internal::Node>&) const
{
  return signTransaction(generateTransactionBody(&client), client);
}

//-----
grpc::Status AccountUpdateTransaction::submitRequest(const Client& client,
                                                     const std::chrono::system_clock::time_point& deadline,
                                                     const std::shared_ptr<internal::Node>& node,
                                                     proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kCryptoUpdateAccount, makeRequest(client, node), deadline, response);
}

//-----
void AccountUpdateTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_cryptoupdateaccount(build());
}

//-----
proto::CryptoUpdateTransactionBody* AccountUpdateTransaction::build() const
{
  auto body = std::make_unique<proto::CryptoUpdateTransactionBody>();

  body->set_allocated_accountidtoupdate(mAccountId.toProtobuf().release());

  if (mKey)
  {
    body->set_allocated_key(mKey->toProtobufKey().release());
  }

  if (mReceiverSignatureRequired.has_value())
  {
    auto value = std::make_unique<google::protobuf::BoolValue>();
    value->set_value(mReceiverSignatureRequired.value());
    body->set_allocated_receiversigrequiredwrapper(value.release());
  }

  if (mAutoRenewPeriod.has_value())
  {
    body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(mAutoRenewPeriod.value()));
  }

  if (mExpirationTime.has_value())
  {
    body->set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(mExpirationTime.value()));
  }

  if (mAccountMemo.has_value())
  {
    auto value = std::make_unique<google::protobuf::StringValue>();
    value->set_value(mAccountMemo.value());
    body->set_allocated_memo(value.release());
  }

  if (mMaxAutomaticTokenAssociations.has_value())
  {
    auto value = std::make_unique<google::protobuf::Int32Value>();
    value->set_value(static_cast<int32_t>(mMaxAutomaticTokenAssociations.value()));
    body->set_allocated_max_automatic_token_associations(value.release());
  }

  if (mStakedAccountId.has_value())
  {
    body->set_allocated_staked_account_id(mStakedAccountId->toProtobuf().release());
  }

  if (mStakedNodeId.has_value())
  {
    body->set_staked_node_id(static_cast<int64_t>(mStakedNodeId.value()));
  }

  if (mDeclineStakingReward.has_value())
  {
    auto value = std::make_unique<google::protobuf::BoolValue>();
    value->set_value(mDeclineStakingReward.value());
    body->set_allocated_decline_reward(value.release());
  }

  return body.release();
}

} // namespace Hedera
