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
#include "AccountCreateTransaction.h"

#include "helper/DurationConverter.h"

#include <proto/crypto_create.pb.h>
#include <proto/schedulable_transaction_body.pb.h>

#include "PublicKey.h"

namespace Hedera
{
//-----
AccountCreateTransaction::AccountCreateTransaction()
  : Transaction()
  , mKey()
  , mAccountMemo()
  , mInitialBalance()
  , mReceiverSignatureRequired(false)
  , mAutoRenewPeriod(90)
  , mMaxAutomaticTokenAssociations(0)
  , mStakedAccountId()
  , mStakedNodeId()
  , mDeclineStakingReward(false)
  , mAlias()
{
  mDefaultMaxTransactionFee = Hbar::from(5LL);
}

//-----
AccountCreateTransaction::AccountCreateTransaction(
  const std::unordered_map<TransactionId, std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  : Transaction(transactions)
{
  initFromTransactionBody();
}

//-----
AccountCreateTransaction::AccountCreateTransaction(const proto::TransactionBody& transaction)
  : Transaction(transaction)
{
  initFromTransactionBody();
}

//-----
void AccountCreateTransaction::validateChecksums(const Client& client) const
{
  if (mStakedAccountId.isValid())
  {
    mStakedAccountId.getValue().validateChecksum(client);
  }
}

//-----
void AccountCreateTransaction::onFreeze(proto::TransactionBody* body) const
{
  body->set_allocated_cryptocreateaccount(build());
}

//-----
void AccountCreateTransaction::onScheduled(proto::SchedulableTransactionBody* body) const
{
  body->set_allocated_cryptocreateaccount(build());
}

//-----
proto::CryptoCreateTransactionBody* AccountCreateTransaction::build() const
{
  proto::CryptoCreateTransactionBody* body = new proto::CryptoCreateTransactionBody;

  if (mKey != nullptr)
  {
    body->set_allocated_key(mKey->toProtobuf());
  }

  body->set_memo(mAccountMemo);
  body->set_initialbalance(mInitialBalance.toTinybars());
  body->set_receiversigrequired(mReceiverSignatureRequired);
  body->set_allocated_autorenewperiod(DurationConverter::toProtobuf(mAutoRenewPeriod));
  body->set_max_automatic_token_associations(mMaxAutomaticTokenAssociations);

  if (mStakedAccountId.isValid())
  {
    body->set_allocated_staked_account_id(mStakedAccountId.getValue().toProtobuf());
  }

  if (mStakedNodeId.isValid())
  {
    body->set_staked_node_id(mStakedNodeId.getValue());
  }

  body->set_decline_reward(mDeclineStakingReward);

  if (mAlias != nullptr)
  {
    body->set_allocated_alias(new std::string(mAlias->toString()));
  }

  return body;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setKey(const std::shared_ptr<PublicKey> key)
{
  requireNotFrozen();

  this->mKey = key;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setInitialBalance(const Hbar& initialBalance)
{
  requireNotFrozen();

  mInitialBalance = initialBalance;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setReceiverSignatureRequired(bool receiveSignatureRequired)
{
  requireNotFrozen();

  mReceiverSignatureRequired = receiveSignatureRequired;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setAutoRenewPeriod(const std::chrono::seconds& autoRenewPeriod)
{
  requireNotFrozen();

  mAutoRenewPeriod = autoRenewPeriod;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setMaxAutomaticTokenAssociations(int32_t associations)
{
  requireNotFrozen();

  mMaxAutomaticTokenAssociations = associations;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setAccountMemo(const std::string& memo)
{
  requireNotFrozen();

  mAccountMemo = memo;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setStakedAccountId(const AccountId& stakedAccountId)
{
  requireNotFrozen();

  mStakedAccountId.setValue(stakedAccountId);
  mStakedNodeId.reset();

  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setStakedNodeId(const int64_t& stakedNodeId)
{
  requireNotFrozen();

  mStakedNodeId.setValue(stakedNodeId);
  mStakedAccountId.reset();

  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setDeclineStakingReward(bool declineStakingReward)
{
  requireNotFrozen();

  mDeclineStakingReward = declineStakingReward;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setAlias(const std::shared_ptr<PublicKey> alias)
{
  requireNotFrozen();

  this->mAlias = alias;
  return *this;
}

//-----
void AccountCreateTransaction::initFromTransactionBody()
{
  if (mSourceTransactionBody.has_cryptocreateaccount())
  {
    const proto::CryptoCreateTransactionBody& body = mSourceTransactionBody.cryptocreateaccount();

    if (body.has_key())
    {
      mKey = PublicKey::fromProtobuf(body.key());
    }

    if (body.has_autorenewperiod())
    {
      mAutoRenewPeriod = DurationConverter::fromProtobuf(body.autorenewperiod());
    }

    mInitialBalance = Hbar::fromTinybars(body.initialbalance());
    mAccountMemo = body.memo();
    mReceiverSignatureRequired = body.receiversigrequired();
    mMaxAutomaticTokenAssociations = body.max_automatic_token_associations();
    mDeclineStakingReward = body.decline_reward();

    if (body.has_staked_account_id())
    {
      mStakedAccountId.setValue(AccountId::fromProtobuf(body.staked_account_id()));
    }

    if (body.has_staked_node_id())
    {
      mStakedNodeId.setValue(body.staked_node_id());
    }

    mAlias = PublicKey::fromAliasBytes(body.alias());
  }
}

} // namespace Hedera