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
#include "ContractInfo.h"
#include "PublicKeyFactory.h"

#include "helper/DurationConverter.h"
#include "helper/InstantConverter.h"

namespace Hedera
{
//-----
ContractInfo::ContractInfo()
  : mContractId()
  , mAccountId()
  , mContractAccountId()
  , mAdminKey()
  , mExpirationTime()
  , mAutoRenewPeriod()
  , mStorage(0LL)
  , mMemo()
  , mBalance(0LL)
  , mDeleted(false)
  , mLedgerId()
  , mAutoRenewAccountId()
  , mMaxAutomaticTokenAssociations(0)
  , mStakingInfo()
{
}

//-----
ContractInfo ContractInfo::fromProtobuf(const proto::ContractGetInfoResponse_ContractInfo& proto)
{
  ContractInfo contractInfo;

  if (proto.has_contractid())
  {
    contractInfo.mContractId.setValue(ContractId::fromProtobuf(proto.contractid()));
  }

  if (proto.has_accountid())
  {
    contractInfo.mAccountId.setValue(AccountId::fromProtobuf(proto.accountid()));
  }

  contractInfo.mContractAccountId = proto.contractaccountid();

  if (proto.has_adminkey())
  {
    contractInfo.mAdminKey = std::move(PublicKeyFactory::fromProtobuf(proto.adminkey()));
  }

  if (proto.has_expirationtime())
  {
    contractInfo.mExpirationTime.setValue(InstantConverter::fromProtobuf(proto.expirationtime()));
  }

  if (proto.has_autorenewperiod())
  {
    contractInfo.mAutoRenewPeriod.setValue(DurationConverter::fromProtobuf(proto.autorenewperiod()));
  }

  contractInfo.mStorage = proto.storage();
  contractInfo.mMemo = proto.memo();
  contractInfo.mBalance = Hbar::fromTinybars(static_cast<uint64_t>(proto.balance()));
  contractInfo.mDeleted = proto.deleted();
  contractInfo.mLedgerId = proto.ledger_id();

  if (proto.has_auto_renew_account_id())
  {
    contractInfo.mAutoRenewAccountId.setValue(AccountId::fromProtobuf(proto.auto_renew_account_id()));
  }

  contractInfo.mMaxAutomaticTokenAssociations = proto.max_automatic_token_associations();

  if (proto.has_staking_info())
  {
    contractInfo.mStakingInfo.setValue(StakingInfo::fromProtobuf(proto.staking_info()));
  }

  return contractInfo;
}

//-----
proto::ContractGetInfoResponse_ContractInfo ContractInfo::toProtobuf() const
{
  proto::ContractGetInfoResponse_ContractInfo proto;

  if (mContractId.isValid())
  {
    proto.set_allocated_contractid(mContractId.getValue().toProtobuf());
  }

  if (mAccountId.isValid())
  {
    proto.set_allocated_accountid(mAccountId.getValue().toProtobuf());
  }

  proto.set_contractaccountid(mContractAccountId);

  if (mAdminKey.get() != nullptr)
  {
    proto.set_allocated_adminkey(mAdminKey->toProtobuf());
  }

  if (mExpirationTime.isValid())
  {
    proto.set_allocated_expirationtime(InstantConverter::toProtobuf(mExpirationTime.getValue()));
  }

  if (mAutoRenewPeriod.isValid())
  {
    proto.set_allocated_autorenewperiod(DurationConverter::toProtobuf(mAutoRenewPeriod.getValue()));
  }

  proto.set_storage(mStorage);
  proto.set_memo(mMemo);
  proto.set_balance(static_cast<uint64_t>(mBalance.toTinybars()));
  proto.set_deleted(mDeleted);
  proto.set_ledger_id(mLedgerId);

  if (mAutoRenewAccountId.isValid())
  {
    proto.set_allocated_auto_renew_account_id(mAutoRenewAccountId.getValue().toProtobuf());
  }

  proto.set_max_automatic_token_associations(mMaxAutomaticTokenAssociations);

  if (mStakingInfo.isValid())
  {
    proto.set_allocated_staking_info(mStakingInfo.getValue().toProtobuf());
  }

  return proto;
}

} // namespace Hedera
