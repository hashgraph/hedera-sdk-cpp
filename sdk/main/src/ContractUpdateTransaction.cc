/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#include "ContractUpdateTransaction.h"
#include "impl/DurationConverter.h"
#include "impl/Node.h"
#include "impl/TimestampConverter.h"

#include <grpcpp/client_context.h>
#include <proto/contract_update.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
ContractUpdateTransaction::ContractUpdateTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<ContractUpdateTransaction>(transactionBody)
{
  if (!transactionBody.has_contractupdateinstance())
  {
    throw std::invalid_argument("Transaction body doesn't contain ContractUpdateInstance data");
  }

  const proto::ContractUpdateTransactionBody& body = transactionBody.contractupdateinstance();

  if (body.has_contractid())
  {
    mContractId = ContractId::fromProtobuf(body.contractid());
  }

  if (body.has_expirationtime())
  {
    mExpirationTime = internal::TimestampConverter::fromProtobuf(body.expirationtime());
  }

  if (body.has_adminkey())
  {
    mAdminKey = ValuePtr<Key, KeyCloner>(Key::fromProtobuf(body.adminkey()).release());
  }

  if (body.has_autorenewperiod())
  {
    mAutoRenewPeriod = internal::DurationConverter::fromProtobuf(body.autorenewperiod());
  }

  if (body.has_memowrapper())
  {
    mContractMemo = body.memowrapper().value();
  }

  if (body.has_max_automatic_token_associations())
  {
    mMaxAutomaticTokenAssociations = static_cast<uint32_t>(body.max_automatic_token_associations().value());
  }

  if (body.has_auto_renew_account_id())
  {
    mAutoRenewAccountId = AccountId::fromProtobuf(body.auto_renew_account_id());
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
ContractUpdateTransaction& ContractUpdateTransaction::setContractId(const ContractId& contractId)
{
  requireNotFrozen();
  mContractId = contractId;
  return *this;
}

//-----
ContractUpdateTransaction& ContractUpdateTransaction::setExpirationTime(
  const std::chrono::system_clock::time_point& expiration)
{
  requireNotFrozen();
  mExpirationTime = expiration;
  return *this;
}

//-----
ContractUpdateTransaction& ContractUpdateTransaction::setAdminKey(const Key* adminKey)
{
  requireNotFrozen();
  mAdminKey = ValuePtr<Key, KeyCloner>(adminKey->clone().release());
  return *this;
}

//-----
ContractUpdateTransaction& ContractUpdateTransaction::setAutoRenewPeriod(
  const std::chrono::duration<double>& autoRenewPeriod)
{
  requireNotFrozen();
  mAutoRenewPeriod = autoRenewPeriod;
  return *this;
}

//-----
ContractUpdateTransaction& ContractUpdateTransaction::setContractMemo(std::string_view memo)
{
  requireNotFrozen();

  if (memo.size() > 100)
  {
    throw std::length_error("Contract memo is too large. Must be smaller than 100 bytes");
  }

  mContractMemo = memo;
  return *this;
}

//-----
ContractUpdateTransaction& ContractUpdateTransaction::setMaxAutomaticTokenAssociations(uint32_t associations)
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
ContractUpdateTransaction& ContractUpdateTransaction::setAutoRenewAccountId(const AccountId& autoRenewAccountId)
{
  requireNotFrozen();
  mAutoRenewAccountId = autoRenewAccountId;
  return *this;
}

//-----
ContractUpdateTransaction& ContractUpdateTransaction::setStakedAccountId(const AccountId& stakedAccountId)
{
  requireNotFrozen();
  mStakedAccountId = stakedAccountId;
  mStakedNodeId.reset();
  return *this;
}

//-----
ContractUpdateTransaction& ContractUpdateTransaction::setStakedNodeId(const uint64_t& stakedNodeId)
{
  requireNotFrozen();
  mStakedNodeId = stakedNodeId;
  mStakedAccountId.reset();
  return *this;
}

//-----
ContractUpdateTransaction& ContractUpdateTransaction::setDeclineStakingReward(bool declineReward)
{
  requireNotFrozen();
  mDeclineStakingReward = declineReward;
  return *this;
}

//-----
proto::Transaction ContractUpdateTransaction::makeRequest(const Client& client,
                                                          const std::shared_ptr<internal::Node>&) const
{
  return signTransaction(generateTransactionBody(&client), client);
}

//-----
grpc::Status ContractUpdateTransaction::submitRequest(const Client& client,
                                                      const std::chrono::system_clock::time_point& deadline,
                                                      const std::shared_ptr<internal::Node>& node,
                                                      proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kContractUpdateInstance, makeRequest(client, node), deadline, response);
}

//-----
void ContractUpdateTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_contractupdateinstance(build());
}

//-----
proto::ContractUpdateTransactionBody* ContractUpdateTransaction::build() const
{
  auto body = std::make_unique<proto::ContractUpdateTransactionBody>();

  body->set_allocated_contractid(mContractId.toProtobuf().release());

  if (mExpirationTime.has_value())
  {
    body->set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(mExpirationTime.value()));
  }

  if (mAdminKey)
  {
    body->set_allocated_adminkey(mAdminKey->toProtobufKey().release());
  }

  if (mAutoRenewPeriod.has_value())
  {
    body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(mAutoRenewPeriod.value()));
  }

  if (mContractMemo.has_value())
  {
    auto value = std::make_unique<google::protobuf::StringValue>();
    value->set_value(mContractMemo.value());
    body->set_allocated_memowrapper(value.release());
  }

  if (mMaxAutomaticTokenAssociations.has_value())
  {
    auto value = std::make_unique<google::protobuf::Int32Value>();
    value->set_value(static_cast<int32_t>(mMaxAutomaticTokenAssociations.value()));
    body->set_allocated_max_automatic_token_associations(value.release());
  }

  if (mAutoRenewAccountId.has_value())
  {
    body->set_allocated_auto_renew_account_id(mAutoRenewAccountId->toProtobuf().release());
  }

  if (mStakedAccountId.has_value())
  {
    body->set_allocated_staked_account_id(mStakedAccountId->toProtobuf().release());
  }

  else if (mStakedNodeId.has_value())
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
