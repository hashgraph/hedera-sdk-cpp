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

#include "PublicKey.h"
#include "TransactionResponse.h"

#include "helper/DurationConverter.h"

#include <proto/crypto_create.pb.h>
#include <proto/response.pb.h>
#include <proto/transaction.pb.h>

namespace Hedera
{
//-----
AccountCreateTransaction::AccountCreateTransaction()
{
  mMaxTransactionFee = Hbar::from(5LL);
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
AccountCreateTransaction& AccountCreateTransaction::setAccountMemo(const std::string& memo)
{
  mAccountMemo = memo;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setMaxAutomaticTokenAssociations(int32_t associations)
{
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
AccountCreateTransaction& AccountCreateTransaction::setStakedNodeId(const int64_t& stakedNodeId)
{
  mStakedNodeId = stakedNodeId;
  mStakedAccountId.reset();
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setDeclineStakingReward(bool declineStakingReward)
{
  mDeclineStakingReward = declineStakingReward;
  return *this;
}

//-----
AccountCreateTransaction& AccountCreateTransaction::setAlias(const std::shared_ptr<PublicKey>& alias)
{
  mAlias = alias;
  return *this;
}

//-----
proto::Transaction AccountCreateTransaction::makeRequest() const
{
  proto::Transaction transaction;
  proto::TransactionBody transactionBody;
  transactionBody.set_allocated_cryptocreateaccount(build().get());

  // TODO: sign here?

  return transaction;
}

//-----
std::shared_ptr<proto::CryptoCreateTransactionBody> AccountCreateTransaction::build() const
{
  std::shared_ptr<proto::CryptoCreateTransactionBody> body = std::make_shared<proto::CryptoCreateTransactionBody>();

  if (mKey != nullptr)
  {
    body->set_allocated_key(mKey->toProtobuf());
  }

  body->set_initialbalance(mInitialBalance.toTinybars());
  body->set_receiversigrequired(mReceiverSignatureRequired);

  if (body->has_autorenewperiod())
  {
    body->set_allocated_autorenewperiod(
      DurationConverter::toProtobuf(std::chrono::duration_cast<std::chrono::seconds>(mAutoRenewPeriod.value())).get());
  }

  body->set_memo(mAccountMemo);
  body->set_max_automatic_token_associations(mMaxAutomaticTokenAssociations);

  if (mStakedAccountId.has_value())
  {
    body->set_allocated_staked_account_id(mStakedAccountId.value().toProtobuf().get());
  }

  if (mStakedNodeId.has_value())
  {
    body->set_staked_node_id(mStakedNodeId.value());
  }

  body->set_decline_reward(mDeclineStakingReward);

  if (mAlias)
  {
    body->set_allocated_alias(new std::string(mAlias->toString()));
  }

  return body;
}

} // namespace Hedera
