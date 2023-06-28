/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License")
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
#include "TokenInfo.h"
#include "impl/DurationConverter.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <proto/basic_types.pb.h>
#include <proto/token_get_info.pb.h>

namespace Hedera
{
//-----
TokenInfo TokenInfo::fromProtobuf(const proto::TokenInfo& proto)
{
  TokenInfo tokenInfo;

  if (proto.has_tokenid())
  {
    tokenInfo.mTokenId = TokenId::fromProtobuf(proto.tokenid());
  }

  tokenInfo.mTokenName = proto.name();
  tokenInfo.mTokenSymbol = proto.symbol();
  tokenInfo.mDecimals = proto.decimals();
  tokenInfo.mTotalSupply = proto.totalsupply();

  if (proto.has_treasury())
  {
    tokenInfo.mTreasuryAccountId = AccountId::fromProtobuf(proto.treasury());
  }

  if (proto.has_adminkey())
  {
    tokenInfo.mAdminKey = Key::fromProtobuf(proto.adminkey());
  }

  if (proto.has_kyckey())
  {
    tokenInfo.mKycKey = Key::fromProtobuf(proto.kyckey());
  }

  if (proto.has_freezekey())
  {
    tokenInfo.mFreezeKey = Key::fromProtobuf(proto.freezekey());
  }

  if (proto.has_wipekey())
  {
    tokenInfo.mWipeKey = Key::fromProtobuf(proto.wipekey());
  }

  if (proto.has_supplykey())
  {
    tokenInfo.mSupplyKey = Key::fromProtobuf(proto.supplykey());
  }

  if (proto.defaultfreezestatus() != proto::TokenFreezeStatus::FreezeNotApplicable)
  {
    tokenInfo.mDefaultFreezeStatus = proto.defaultfreezestatus() == proto::TokenFreezeStatus::Frozen;
  }

  if (proto.defaultkycstatus() != proto::TokenKycStatus::KycNotApplicable)
  {
    tokenInfo.mDefaultKycStatus = proto.defaultkycstatus() == proto::TokenKycStatus::Granted;
  }

  tokenInfo.mIsDeleted = proto.deleted();

  if (proto.has_autorenewaccount())
  {
    tokenInfo.mAutoRenewAccountId = AccountId::fromProtobuf(proto.autorenewaccount());
  }

  if (proto.has_autorenewperiod())
  {
    tokenInfo.mAutoRenewPeriod = internal::DurationConverter::fromProtobuf(proto.autorenewperiod());
  }

  if (proto.has_expiry())
  {
    tokenInfo.mExpirationTime = internal::TimestampConverter::fromProtobuf(proto.expiry());
  }

  tokenInfo.mTokenMemo = proto.memo();
  tokenInfo.mTokenType = gProtobufTokenTypeToTokenType.at(proto.tokentype());
  tokenInfo.mSupplyType = gProtobufTokenSupplyTypeToTokenSupplyType.at(proto.supplytype());
  tokenInfo.mMaxSupply = static_cast<uint64_t>(proto.maxsupply());

  if (proto.has_fee_schedule_key())
  {
    tokenInfo.mFeeScheduleKey = Key::fromProtobuf(proto.fee_schedule_key());
  }

  for (int i = 0; i < proto.custom_fees_size(); ++i)
  {
    tokenInfo.mCustomFees.push_back(CustomFee::fromProtobuf(proto.custom_fees(i)));
  }

  if (proto.has_pause_key())
  {
    tokenInfo.mPauseKey = Key::fromProtobuf(proto.pause_key());
  }

  if (proto.pause_status() != proto::TokenPauseStatus::PauseNotApplicable)
  {
    tokenInfo.mPauseStatus = proto.pause_status() == proto::TokenPauseStatus::Paused;
  }

  tokenInfo.mLedgerId = LedgerId(internal::Utilities::stringToByteVector(proto.ledger_id()));

  return tokenInfo;
}

} // namespace Hedera
