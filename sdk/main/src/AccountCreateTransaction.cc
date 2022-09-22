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

#include "crypto_create.pb.h"

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
  , mAliasKey()
  , mAliasEvmAddress()
{
  mDefaultMaxTransactionFee = Hbar::from(5LL);
}

//-----
AccountCreateTransaction::AccountCreateTransaction(
  const std::unordered_map<
    TransactionId,
    std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  : Transaction(transactions)
{
  initFromTransactionBody();
}

//-----
AccountCreateTransaction::AccountCreateTransaction(
  const proto::TransactionBody& transaction)
  : Transaction(transaction)
{
  initFromTransactionBody();
}

//-----
void
AccountCreateTransaction::validateChecksums(const Client& client) const
{
  if (mStakedAccountId.isValid())
  {
    mStakedAccountId.getValue().validateChecksum(client);
  }
}

//-----
AccountCreateTransaction&
AccountCreateTransaction::setKey(const Key& key)
{
  requireNotFrozen();

  mKey = key;
  return *this;
}

//-----
AccountCreateTransaction&
AccountCreateTransaction::setInitialBalance(const Hbar& initialBalance)
{
  requireNotFrozen();

  mInitialBalance = initialBalance;
  return *this;
}

//-----
AccountCreateTransaction&
AccountCreateTransaction::setReceiverSignatureRequired(
  bool receiveSignatureRequired)
{
  requireNotFrozen();

  mReceiverSignatureRequired = receiveSignatureRequired;
  return *this;
}

//-----
AccountCreateTransaction&
AccountCreateTransaction::setAutoRenewPeriod(
  const std::chrono::days& autoRenewPeriod)
{
  requireNotFrozen();

  mAutoRenewPeriod = autoRenewPeriod;
  return *this;
}

//-----
AccountCreateTransaction&
AccountCreateTransaction::setMaxAutomaticTokenAssociations(int32_t associations)
{
  requireNotFrozen();

  mMaxAutomaticTokenAssociations = associations;
  return *this;
}

//-----
AccountCreateTransaction&
AccountCreateTransaction::setAccountMemo(const std::string& memo)
{
  requireNotFrozen();

  mAccountMemo = memo;
  return *this;
}

//-----
AccountCreateTransaction&
AccountCreateTransaction::setStakedAccountId(const AccountId& stakedAccountId)
{
  requireNotFrozen();

  mStakedAccountId.setValue(stakedAccountId);
  mStakedNodeId.reset();

  return *this;
}

//-----
AccountCreateTransaction&
AccountCreateTransaction::setStakedNodeId(const int64_t& stakedNodeId)
{
  requireNotFrozen();

  mStakedNodeId.setValue(stakedNodeId);
  mStakedAccountId.reset();

  return *this;
}

//-----
AccountCreateTransaction&
AccountCreateTransaction::setDeclineStakingReward(bool declineStakingReward)
{
  requireNotFrozen();

  mDeclineStakingReward = declineStakingReward;
  return *this;
}

//-----
AccountCreateTransaction&
AccountCreateTransaction::setAliasKey(const PublicKey& aliasKey)
{
  requireNotFrozen();

  mAliasKey.setValue(aliasKey);
  return *this;
}

//-----
AccountCreateTransaction&
AccountCreateTransaction::setAliasEvmAddress(const EvmAddress& aliasEvmAddress)
{
  requireNotFrozen();

  mAliasEvmAddress.setValue(aliasEvmAddress);
  return *this;
}

//-----
void
AccountCreateTransaction::initFromTransactionBody()
{
  if (mSourceTransactionBody.has_cryptocreateaccount())
  {
    const proto::CryptoCreateTransactionBody& body =
      mSourceTransactionBody.cryptocreateaccount();

    if (body.has_key())
    {
      mKey.setValue(Key::fromProtobuf(body.key()));
    }

    if (body.has_autorenewperiod())
    {
      mAutoRenewPeriod =
        DurationConverter::fromProtobuf(body.autorenewperiod());
    }

    mInitialBalance = Hbar::fromTinybars(body.initialbalance());
    mAccountMemo = body.memo();
    mReceiverSignatureRequired = body.receiversigrequired();
    mMaxAutomaticTokenAssociations = body.max_automatic_token_associations();
    mDeclineStakingReward = body.decline_reward();

    if (body.has_staked_account_id())
    {
      mStakedAccountId.setValue(
        AccountId::fromProtobuf(body.staked_account_id()));
    }

    if (body.has_staked_node_id())
    {
      mStakedNodeId.setValue(body.staked_node_id());
    }

    // TODO: this may need to change based on what the alias bytes represent
    mAliasKey.setValue(PublicKey::fromAliasBytes(body.alias()));
    mAliasEvmAddress.setValue(EvmAddress::fromAliasBytes(body.alias()));
  }
}

} // namespace Hedera