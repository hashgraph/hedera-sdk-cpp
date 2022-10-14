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
#include "AccountInfo.h"
#include "PublicKey.h"

#include "LiveHash.h"

#include "helper/DurationConverter.h"
#include "helper/InstantConverter.h"

#include <proto/crypto_get_info.pb.h>

namespace Hedera
{
//-----
AccountInfo::AccountInfo()
  : mAccountId()
  , mContractAccountId()
  , mIsDeleted(false)
  , mProxyReceived(0LL)
  , mKey()
  , mBalance(0LL)
  , mIsReceiverSignatureRequired(false)
  , mExpirationTime()
  , mAutoRenewPeriod()
  , mAccountMemo()
  , mOwnedNfts(0ULL)
  , mMaxAutomaticTokenAssociations(0UL)
  , mAliasKey()
  , mLedgerId()
  , mEthereumNonce(0LL)
  , mStakingInfo()
{
}

//-----
AccountInfo::AccountInfo(const AccountId& accountId,
                         const std::string& contractAccountId,
                         bool isDeleted,
                         const long long& proxyReceived,
                         const std::shared_ptr<PublicKey> key,
                         const long long& balance,
                         bool receiverSignatureRequired,
                         const std::chrono::nanoseconds& expirationTime,
                         const std::chrono::seconds& autoRenewPeriod,
                         const std::vector<LiveHash>& liveHashes,
                         const std::string& accountMemo,
                         const unsigned long long& ownedNfts,
                         unsigned int maxAutomaticTokenAssociations,
                         const std::shared_ptr<PublicKey> aliasKey,
                         const LedgerId& ledgerId,
                         const long long& ethereumNonce,
                         const InitType<StakingInfo> stakingInfo)
  : mAccountId(accountId)
  , mContractAccountId(contractAccountId)
  , mIsDeleted(isDeleted)
  , mProxyReceived(Hbar::fromTinybars(proxyReceived))
  , mKey(key)
  , mBalance(Hbar::fromTinybars(balance))
  , mIsReceiverSignatureRequired(false)
  , mExpirationTime(expirationTime)
  , mAutoRenewPeriod(autoRenewPeriod)
  , mLiveHashes(liveHashes)
  , mAccountMemo(accountMemo)
  , mOwnedNfts(ownedNfts)
  , mMaxAutomaticTokenAssociations(maxAutomaticTokenAssociations)
  , mAliasKey(aliasKey)
  , mLedgerId(ledgerId)
  , mEthereumNonce(ethereumNonce)
  , mStakingInfo(stakingInfo)
{
}

//-----
AccountInfo AccountInfo::fromProtobuf(const proto::CryptoGetInfoResponse_AccountInfo& accountInfo)
{
  std::vector<LiveHash> liveHashes;
  for (size_t i = 0; i < accountInfo.livehashes_size(); ++i)
  {
    liveHashes.push_back(LiveHash::fromProtobuf(accountInfo.livehashes(i)));
  }

  return AccountInfo(AccountId::fromProtobuf(accountInfo.accountid()),
                     accountInfo.contractaccountid(),
                     accountInfo.deleted(),
                     accountInfo.proxyreceived(),
                     PublicKey::fromProtobuf(accountInfo.key()),
                     accountInfo.balance(),
                     accountInfo.receiversigrequired(),
                     InstantConverter::fromProtobuf(accountInfo.expirationtime()),
                     DurationConverter::fromProtobuf(accountInfo.autorenewperiod()),
                     liveHashes,
                     accountInfo.memo(),
                     accountInfo.ownednfts(),
                     accountInfo.max_automatic_token_associations(),
                     PublicKey::fromAliasBytes(accountInfo.alias()),
                     LedgerId::fromByteString(accountInfo.ledger_id()),
                     accountInfo.ethereum_nonce(),
                     (accountInfo.has_staking_info())
                       ? InitType<StakingInfo>(StakingInfo::fromProtobuf(accountInfo.staking_info()))
                       : InitType<StakingInfo>());
}

//-----
proto::CryptoGetInfoResponse_AccountInfo AccountInfo::toProtobuf() const
{
  proto::CryptoGetInfoResponse_AccountInfo proto;
  proto.set_allocated_accountid(mAccountId.toProtobuf());
  proto.set_allocated_contractaccountid(new std::string(mContractAccountId));
  proto.set_deleted(mIsDeleted);
  proto.set_proxyreceived(mProxyReceived.toTinybars());
  proto.set_allocated_key(mKey->toProtobuf());
  proto.set_balance(static_cast<unsigned long long>(mBalance.toTinybars()));
  proto.set_receiversigrequired(mIsReceiverSignatureRequired);
  proto.set_allocated_expirationtime(InstantConverter::toProtobuf(mExpirationTime));
  proto.set_allocated_autorenewperiod(DurationConverter::toProtobuf(mAutoRenewPeriod));

  for (size_t i = 0; i < mLiveHashes.size(); ++i)
  {
    proto::LiveHash* protoLiveHash = proto.add_livehashes();
    // TODO: fill in with live hash fields
  }

  proto.set_allocated_memo(new std::string(mAccountMemo));
  proto.set_ownednfts(mOwnedNfts);
  proto.set_max_automatic_token_associations(mMaxAutomaticTokenAssociations);

  if (mAliasKey.get() != nullptr)
  {
    proto.set_allocated_alias(new std::string(mAliasKey->toStringDER()));
  }

  proto.set_ledger_id(mLedgerId.toByteString());
  proto.set_ethereum_nonce(mEthereumNonce);

  if (mStakingInfo.isValid())
  {
    proto.set_allocated_staking_info(mStakingInfo.getValue().toProtobuf());
  }

  return proto;
}

} // namespace Hedera