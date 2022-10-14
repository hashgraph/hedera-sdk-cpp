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
#include "ContractCreateTransaction.h"

#include "ContractFunctionParameters.h"

#include "PublicKey.h"

#include "helper/DurationConverter.h"

#include <proto/contract_create.pb.h>

namespace Hedera
{
//-----
ContractCreateTransaction::ContractCreateTransaction()
  : Transaction()
  , mInitCodeFileId()
  , mInitCodeByteCode()
  , mAdminKey()
  , mGas(0LL)
  , mInitialBalance(0LL)
  , mAutoRenewPeriod()
  , mConstructorParameters()
  , mMemo(std::string())
  , mMaxAutomaticTokenAssociations(0)
  , mAutoRenewAccountId()
  , mStakedAccountId()
  , mStakedNodeId()
  , mDeclineReward(false)
{
  setAutoRenewPeriod(mDefaultAutoRenewPeriod);
  mDefaultMaxTransactionFee = Hbar::from(20LL);
}

//-----
ContractCreateTransaction::ContractCreateTransaction(
  const std::unordered_map<TransactionId, std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  : Transaction(transactions)
{
  initFromTransactionBody();
}

//-----
ContractCreateTransaction::ContractCreateTransaction(const proto::TransactionBody& transaction)
  : Transaction(transaction)
{
  initFromTransactionBody();
}

//-----
void ContractCreateTransaction::validateChecksums(const Client& client) const
{
  if (mInitCodeFileId.isValid())
  {
    mInitCodeFileId.getValue().validateChecksum(client);
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
proto::ContractCreateTransactionBody ContractCreateTransaction::build() const
{
  proto::ContractCreateTransactionBody body;

  if (mInitCodeFileId.isValid())
  {
    body.set_allocated_fileid(mInitCodeFileId.getValue().toProtobuf());
  }

  if (mInitCodeByteCode.isValid())
  {
    body.set_initcode(mInitCodeByteCode.getValue());
  }

  if (mAdminKey.get() != nullptr)
  {
    body.set_allocated_adminkey(mAdminKey->toProtobuf());
  }

  body.set_gas(mGas);
  body.set_initialbalance(mInitialBalance.toTinybars());

  if (body.has_autorenewperiod())
  {
    body.set_allocated_autorenewperiod(DurationConverter::toProtobuf(mAutoRenewPeriod.getValue()));
  }

  body.set_constructorparameters(mConstructorParameters);
  body.set_memo(mMemo);
  body.set_max_automatic_token_associations(mMaxAutomaticTokenAssociations);

  if (body.has_auto_renew_account_id())
  {
    body.set_allocated_auto_renew_account_id(mAutoRenewAccountId.getValue().toProtobuf());
  }

  if (body.has_staked_account_id())
  {
    body.set_allocated_staked_account_id(mStakedAccountId.getValue().toProtobuf());
  }

  if (body.has_staked_node_id())
  {
    body.set_staked_node_id(mStakedNodeId.getValue());
  }

  body.set_decline_reward(mDeclineReward);

  return body;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setInitCodeFileId(const FileId& bytecodeFileId)
{
  requireNotFrozen();

  mInitCodeByteCode.reset();
  mInitCodeFileId.setValue(bytecodeFileId);

  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setInitCodeByteCode(const std::string& bytecode)
{
  requireNotFrozen();

  mInitCodeFileId.reset();
  mInitCodeByteCode.setValue(bytecode);

  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setAdminKey(const std::shared_ptr<PublicKey> adminKey)
{
  requireNotFrozen();

  mAdminKey = adminKey;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setGas(const int64_t& gas)
{
  requireNotFrozen();

  mGas = gas;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setInitialBalance(const Hbar& initialBalance)
{
  requireNotFrozen();

  mInitialBalance = initialBalance;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setAutoRenewPeriod(const std::chrono::seconds& autoRenewPeriod)
{
  requireNotFrozen();

  mAutoRenewPeriod.setValue(autoRenewPeriod);
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setConstructorParameters(const std::string& constructorParameters)
{
  requireNotFrozen();

  mConstructorParameters = constructorParameters;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setConstructorParameters(
  const ContractFunctionParameters& constructorParameters)
{
  return setConstructorParameters(constructorParameters.toByteArray());
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setMaxAutomaticTokenAssociations(
  int32_t maxAutomaticTokenAssociations)
{
  requireNotFrozen();

  mMaxAutomaticTokenAssociations = maxAutomaticTokenAssociations;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setContractMemo(const std::string& memo)
{
  requireNotFrozen();

  mMemo = memo;
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setAutoRenewAccountId(const AccountId& autoRenewAccountId)
{
  requireNotFrozen();

  mAutoRenewAccountId.setValue(autoRenewAccountId);
  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setStakedAccountId(const AccountId& stakedAccountId)
{
  requireNotFrozen();

  mStakedAccountId.setValue(stakedAccountId);
  mStakedNodeId.reset();

  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setStakedNodeId(const int64_t& stakedNodeId)
{
  requireNotFrozen();

  mStakedNodeId.setValue(stakedNodeId);
  mStakedAccountId.reset();

  return *this;
}

//-----
ContractCreateTransaction& ContractCreateTransaction::setDeclineStakingReward(bool declineStakingReward)
{
  requireNotFrozen();

  mDeclineReward = declineStakingReward;
  return *this;
}

//-----
void ContractCreateTransaction::initFromTransactionBody()
{
  if (mSourceTransactionBody.has_contractcreateinstance())
  {
    const proto::ContractCreateTransactionBody& body = mSourceTransactionBody.contractcreateinstance();

    if (body.has_fileid())
    {
      mInitCodeFileId.setValue(FileId::fromProtobuf(body.fileid()));
    }

    if (body.has_initcode())
    {
      mInitCodeByteCode.setValue(body.initcode());
    }

    if (body.has_adminkey())
    {
      mAdminKey = std::move(PublicKey::fromProtobuf(body.adminkey()));
    }

    mGas = body.gas();
    mInitialBalance = Hbar::fromTinybars(body.initialbalance());

    if (body.has_autorenewperiod())
    {
      mAutoRenewPeriod.setValue(DurationConverter::fromProtobuf(body.autorenewperiod()));
    }

    mConstructorParameters = body.constructorparameters();
    mMemo = body.memo();
    mMaxAutomaticTokenAssociations = body.max_automatic_token_associations();

    if (body.has_auto_renew_account_id())
    {
      mAutoRenewAccountId.setValue(AccountId::fromProtobuf(body.auto_renew_account_id()));
    }

    if (body.has_staked_account_id())
    {
      mStakedAccountId.setValue(AccountId::fromProtobuf(body.staked_account_id()));
    }

    if (body.has_staked_node_id())
    {
      mStakedNodeId.setValue(body.staked_node_id());
    }

    mDeclineReward = body.decline_reward();
  }
}

} // namespace Hedera