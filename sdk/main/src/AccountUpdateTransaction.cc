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
#include "AccountUpdateTransaction.h"

#include "helper/DurationConverter.h"
#include "helper/InstantConverter.h"

namespace Hedera
{
//-----
AccountUpdateTransaction::AccountUpdateTransaction()
  : mAccountId()
  , mKey()
  , mExpirationTime()
  , mAutoRenewPeriod()
  , mAccountMemo()
  , mMaxAutomaticTokenAssociations()
  , mStakedAccountId()
  , mStakedNodeId()
  , mDeclineStakingReward()
{
}

//-----
AccountUpdateTransaction::AccountUpdateTransaction(
  const std::unordered_map<
    TransactionId,
    std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  : Transaction(transactions)
{
  initFromTransactionBody();
}

//-----
AccountUpdateTransaction::AccountUpdateTransaction(
  const proto::TransactionBody& transaction)
  : Transaction(transaction)
{
  initFromTransactionBody();
}

//-----
AccountUpdateTransaction&
AccountUpdateTransaction::setAccountId(const AccountId& accountId)
{
  requireNotFrozen();

  mAccountId.setValue(accountId);
  return *this;
}

//-----
AccountUpdateTransaction&
AccountUpdateTransaction::setKey(const Key& key)
{
  requireNotFrozen();

  mKey.setValue(key);
  return *this;
}

//-----
AccountUpdateTransaction&
AccountUpdateTransaction::setExpirationTime(
  const std::chrono::nanoseconds& expirationTime)
{
  requireNotFrozen();

  mExpirationTime.setValue(expirationTime);
  return *this;
}

//-----
AccountUpdateTransaction&
AccountUpdateTransaction::setAutoRenewPeriod(
  const std::chrono::days& autoRenewPeriod)
{
  requireNotFrozen();

  mAutoRenewPeriod.setValue(autoRenewPeriod);
  return *this;
}

//-----
AccountUpdateTransaction&
AccountUpdateTransaction::setAccountMemo(const std::string& accountMemo)
{
  requireNotFrozen();

  mAccountMemo.setValue(accountMemo);
  return *this;
}

//-----
AccountUpdateTransaction&
AccountUpdateTransaction::setMaxAutomaticTokenAssociations(
  int32_t maxAssociations)
{
  requireNotFrozen();

  mMaxAutomaticTokenAssociations.setValue(maxAssociations);
  return *this;
}

//-----
AccountUpdateTransaction&
AccountUpdateTransaction::setStakedAccountId(const AccountId& stakedAccountId)
{
  requireNotFrozen();

  mStakedAccountId.setValue(stakedAccountId);
  return *this;
}

//-----
AccountUpdateTransaction&
AccountUpdateTransaction::setStakedNodeId(const int64_t& stakedNodeId)
{
  requireNotFrozen();

  mStakedNodeId.setValue(stakedNodeId);
  return *this;
}

//-----
AccountUpdateTransaction&
AccountUpdateTransaction::setDeclineStakingReward(bool declineStakingReward)
{
  requireNotFrozen();

  mDeclineStakingReward.setValue(declineStakingReward);
  return *this;
}

//-----
void
AccountUpdateTransaction::initFromTransactionBody()
{
  if (mSourceTransactionBody.has_cryptoupdateaccount())
  {
    const proto::CryptoUpdateTransactionBody& body =
      mSourceTransactionBody.cryptoupdateaccount();

    if (body.has_accountidtoupdate())
    {
      mAccountId.setValue(AccountId::fromProtobuf(body.accountidtoupdate()));
    }

    if (body.has_key())
    {
      mKey.setValue(Key::fromProtobuf(body.key()));
    }

    if (body.has_expirationtime())
    {
      mExpirationTime.setValue(
        InstantConverter::fromProtobuf(body.expirationtime()));
    }

    if (body.has_autorenewperiod())
    {
      mAutoRenewPeriod.setValue(
        DurationConverter::fromProtobuf(body.autorenewperiod()));
    }

    if (body.has_memo())
    {
      mAccountMemo.setValue(body.memo().value());
    }

    if (body.has_max_automatic_token_associations())
    {
      mMaxAutomaticTokenAssociations.setValue(
        body.max_automatic_token_associations().value());
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
      mDeclineStakingReward.setValue(body.decline_reward().value());
    }
  }
}

} // namespace Hedera