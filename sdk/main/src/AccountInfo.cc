/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "AccountInfo.h"
#include "impl/DurationConverter.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <proto/crypto_get_info.pb.h>

namespace Hedera
{
//-----
AccountInfo AccountInfo::fromProtobuf(const proto::CryptoGetInfoResponse_AccountInfo& proto)
{
  AccountInfo accountInfo;

  if (proto.has_accountid())
  {
    accountInfo.mAccountId = AccountId::fromProtobuf(proto.accountid());
  }

  accountInfo.mContractAccountId = proto.contractaccountid();
  accountInfo.mIsDeleted = proto.deleted();
  accountInfo.mProxyReceived = Hbar(proto.proxyreceived(), HbarUnit::TINYBAR());

  if (proto.has_key())
  {
    accountInfo.mKey = Key::fromProtobuf(proto.key());
  }

  accountInfo.mBalance = Hbar(static_cast<int64_t>(proto.balance()), HbarUnit::TINYBAR());
  accountInfo.mReceiverSignatureRequired = proto.receiversigrequired();

  if (proto.has_expirationtime())
  {
    accountInfo.mExpirationTime = internal::TimestampConverter::fromProtobuf(proto.expirationtime());
  }

  if (proto.has_autorenewperiod())
  {
    accountInfo.mAutoRenewPeriod = internal::DurationConverter::fromProtobuf(proto.autorenewperiod());
  }

  accountInfo.mMemo = proto.memo();
  accountInfo.mOwnedNfts = static_cast<uint64_t>(proto.ownednfts());
  accountInfo.mMaxAutomaticTokenAssociations = static_cast<uint32_t>(proto.max_automatic_token_associations());

  if (!proto.alias().empty())
  {
    if (proto.alias().size() == EvmAddress::NUM_BYTES)
    {
      accountInfo.mEvmAddressAlias = EvmAddress::fromBytes(internal::Utilities::stringToByteVector(proto.alias()));
    }
    else
    {
      accountInfo.mPublicKeyAlias = PublicKey::fromAliasBytes(internal::Utilities::stringToByteVector(proto.alias()));
    }
  }

  if (!proto.ledger_id().empty())
  {
    accountInfo.mLedgerId = LedgerId(internal::Utilities::stringToByteVector(proto.ledger_id()));
  }

  if (proto.has_staking_info())
  {
    accountInfo.mStakingInfo = StakingInfo::fromProtobuf(proto.staking_info());
  }

  return accountInfo;
}

} // namespace Hedera
