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

#include "helper/DurationConverter.h"
#include "helper/InstantConverter.h"

namespace Hedera
{
//-----
ContractUpdateTransaction::ContractUpdateTransaction()
  : Transaction()
  , mContractId()
  , mExpirationTime()
  , mAdminKey()
  , mAutoRenewPeriod()
  , mMemo()
  , mMaxAutomaticTokenAssociations()
  , mAutoRenewAccountId()
  , mStakedAccountId()
  , mStakedNodeId()
  , mDeclineReward()
{
}

//-----
ContractUpdateTransaction::ContractUpdateTransaction(
  const std::unordered_map<
    TransactionId,
    std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  : Transaction(transactions)
{
  initFromTransactionBody();
}

//-----
ContractUpdateTransaction::ContractUpdateTransaction(
  const proto::TransactionBody& transaction)
{
  initFromTransactionBody();
}

//-----
void
ContractUpdateTransaction::validateChecksums(const Client& client) const
{
  if (mContractId.isValid())
  {
    mContractId.getValue().validateChecksum(client);
  }

  if (mStakedAccountId.isValid())
  {
    mStakedAccountId.getValue().validateChecksum(client);
  }

  if (mAutoRenewAccountId.isValid())
  {
    mAutoRenewAccountId.getValue().validateChecksum(client);
  }
}

//-----
proto::ContractUpdateTransactionBody
ContractUpdateTransaction::build() const
{
  proto::ContractUpdateTransactionBody proto;

  if (mContractId.isValid())
  {
    proto.set_allocated_contractid(mContractId.getValue().toProtobuf());
  }

  if (mExpirationTime.isValid())
  {
    proto.set_allocated_expirationtime(
      InstantConverter::toProtobuf(mExpirationTime.getValue()));
  }

  if (mAdminKey.isValid())
  {
    proto.set_allocated_adminkey(mAdminKey.getValue().toProtobuf());
  }

  if (mAutoRenewPeriod.isValid())
  {
    proto.set_allocated_autorenewperiod(
      DurationConverter::toProtobuf(mAutoRenewPeriod.getValue()));
  }

  if (mMemo.isValid())
  {
    google::protobuf::StringValue* str = proto.mutable_memowrapper();
    str->set_value(mMemo.getValue());
  }

  if (mMaxAutomaticTokenAssociations.isValid())
  {
    google::protobuf::Int32Value* intVal =
      proto.mutable_max_automatic_token_associations();
    intVal->set_value(mMaxAutomaticTokenAssociations.getValue());
  }

  if (mAutoRenewAccountId.isValid())
  {
    proto.set_allocated_auto_renew_account_id(
      mAutoRenewAccountId.getValue().toProtobuf());
  }

  if (mStakedAccountId.isValid())
  {
    proto.set_allocated_staked_account_id(
      mStakedAccountId.getValue().toProtobuf());
  }

  if (mStakedNodeId.isValid())
  {
    proto.set_staked_node_id(mStakedNodeId.getValue());
  }

  if (mDeclineReward.isValid())
  {
    google::protobuf::BoolValue* boolVal = proto.mutable_decline_reward();
    boolVal->set_value(mDeclineReward.getValue());
  }

  return proto;
}

//-----
ContractUpdateTransaction&
ContractUpdateTransaction::setContractId(const ContractId& contractId)
{
  requireNotFrozen();

  mContractId.setValue(contractId);
  return *this;
}

//-----
ContractUpdateTransaction&
ContractUpdateTransaction::setExpirationTime(
  const std::chrono::nanoseconds& expirationTime)
{
  requireNotFrozen();

  mExpirationTime.setValue(expirationTime);
  return *this;
}

//-----
ContractUpdateTransaction&
ContractUpdateTransaction::setAutoRenewPeriod(
  const std::chrono::seconds& autoRenewPeriod)
{
  requireNotFrozen();

  mAutoRenewPeriod.setValue(autoRenewPeriod);
  return *this;
}

//-----
ContractUpdateTransaction&
ContractUpdateTransaction::setMemo(const std::string& memo)
{
  requireNotFrozen();

  mMemo.setValue(memo);
  return *this;
}

//-----
ContractUpdateTransaction&
ContractUpdateTransaction::setMaxAutomaticTokenAssociations(
  int32_t maxAutomaticTokenAssociations)
{
  requireNotFrozen();

  mMaxAutomaticTokenAssociations.setValue(maxAutomaticTokenAssociations);
  return *this;
}

//-----
ContractUpdateTransaction&
ContractUpdateTransaction::setAutoRenewAccountId(
  const AccountId& autoRenewAccountId)
{
  requireNotFrozen();

  mAutoRenewAccountId.setValue(autoRenewAccountId);
  return *this;
}

//-----
ContractUpdateTransaction&
ContractUpdateTransaction::setStakedAccountId(const AccountId& stakedAccountId)
{
  requireNotFrozen();

  mStakedAccountId.setValue(stakedAccountId);
  mStakedNodeId.reset();

  return *this;
}

//-----
ContractUpdateTransaction&
ContractUpdateTransaction::setStakedNodeId(const int64_t& stakedNodeId)
{
  requireNotFrozen();

  mStakedNodeId.setValue(stakedNodeId);
  mStakedAccountId.reset();

  return *this;
}

//-----
ContractUpdateTransaction&
ContractUpdateTransaction::setDeclineRewards(bool declineRewards)
{
  requireNotFrozen();

  mDeclineReward.setValue(declineRewards);
  return *this;
}

//-----
void
ContractUpdateTransaction::initFromTransactionBody()
{
  if (mSourceTransactionBody.has_contractupdateinstance())
  {
    const proto::ContractUpdateTransactionBody& body =
      mSourceTransactionBody.contractupdateinstance();

    if (body.has_contractid())
    {
      mContractId.setValue(ContractId::fromProtobuf(body.contractid()));
    }

    if (body.has_expirationtime())
    {
      mExpirationTime.setValue(
        InstantConverter::fromProtobuf(body.expirationtime()));
    }

    if (body.has_adminkey())
    {
      mAdminKey.setValue(Key::fromProtobuf(body.adminkey()));
    }

    if (body.has_autorenewperiod())
    {
      mAutoRenewPeriod.setValue(
        DurationConverter::fromProtobuf(body.autorenewperiod()));
    }

    if (body.has_memowrapper())
    {
      mMemo.setValue(body.memowrapper().value());
    }

    if (body.has_max_automatic_token_associations())
    {
      mMaxAutomaticTokenAssociations.setValue(
        body.max_automatic_token_associations().value());
    }

    if (body.has_auto_renew_account_id())
    {
      mAutoRenewAccountId.setValue(
        AccountId::fromProtobuf(body.auto_renew_account_id()));
    }

    if (body.has_staked_account_id())
    {
      mStakedAccountId.setValue(
        AccountId::fromProtobuf(body.staked_account_id()));
    }

    if (body.has_staked_node_id())
    {
      mStakedNodeId.setValue(body.staked_node_id());
    }

    if (body.has_decline_reward())
    {
      mDeclineReward.setValue(body.decline_reward().value());
    }
  }
}

} // namespace Hedera
