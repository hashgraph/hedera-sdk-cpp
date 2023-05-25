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
#include "impl/DurationConverter.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <proto/contract_get_info.pb.h>

namespace Hedera
{
//-----
ContractInfo ContractInfo::fromProtobuf(const proto::ContractGetInfoResponse_ContractInfo& proto)
{
  ContractInfo contractInfo;

  if (proto.has_contractid())
  {
    contractInfo.mContractId = ContractId::fromProtobuf(proto.contractid());
  }

  if (proto.has_accountid())
  {
    contractInfo.mAccountId = AccountId::fromProtobuf(proto.accountid());
  }

  contractInfo.mContractAccountId = proto.contractaccountid();

  if (proto.has_adminkey())
  {
    contractInfo.mAdminKey = ValuePtr<Key, KeyCloner>(Key::fromProtobuf(proto.adminkey()).release());
  }

  if (proto.has_expirationtime())
  {
    contractInfo.mExpirationTime = internal::TimestampConverter::fromProtobuf(proto.expirationtime());
  }

  if (proto.has_autorenewperiod())
  {
    contractInfo.mAutoRenewPeriod = internal::DurationConverter::fromProtobuf(proto.autorenewperiod());
  }

  contractInfo.mStorage = proto.storage();
  contractInfo.mMemo = proto.memo();
  contractInfo.mBalance = Hbar(static_cast<int64_t>(proto.balance()), HbarUnit::TINYBAR());
  contractInfo.mIsDeleted = proto.deleted();
  contractInfo.mLedgerId = LedgerId(internal::Utilities::stringToByteVector(proto.ledger_id()));

  if (proto.has_auto_renew_account_id())
  {
    contractInfo.mAutoRenewAccountId = AccountId::fromProtobuf(proto.auto_renew_account_id());
  }

  contractInfo.mMaxAutomaticTokenAssociations = proto.max_automatic_token_associations();

  if (proto.has_staking_info())
  {
    contractInfo.mStakingInfo = StakingInfo::fromProtobuf(proto.staking_info());
  }

  return contractInfo;
}

} // namespace Hedera
