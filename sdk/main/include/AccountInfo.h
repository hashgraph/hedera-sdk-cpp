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
#ifndef ACCOUNT_INFO_H_
#define ACCOUNT_INFO_H_

#include "AccountId.h"
#include "Hbar.h"
#include "LedgerId.h"
#include "LiveHash.h"
#include "PublicKey.h"
#include "StakingInfo.h"

#include "helper/InitType.h"

#include <chrono>
#include <string>
#include <vector>
#include <memory.h>

namespace proto
{
class CryptoGetInfoResponse_AccountInfo;
}

namespace Hedera
{
/**
 * Current information about an account, including the balance.
 */
class AccountInfo
{
public:
  /**
   * Constructor
   */
  AccountInfo();

  /**
   * Construct with initialization values.
   *
   * @param accountId         The account ID of the account.
   * @param contractAccountId The account ID and the contract instance.
   * @param isDeleted         \c TRUE if this account is deleted, otherwise
   *                          \c FALSE
   * @param proxyReceived     The proxy staked to this account.
   * @param key               The key for this account.
   * @param balance           The balance for this account.
   * @param receiverSignatureRequired \c TRUE if a signature is required to
   *                                  receive, otherwise \c FALSE
   * @param expirationTime  The nanoseconds from the epoch at which this account
   *                        will expire.
   * @param autoRenewPeriod The duration with which this account tries to renew.
   * @param liveHashes      The livehashes for this account.
   * @param accountMemo     The memorandum for this account.
   * @param ownedNfts       The number of NFTs owned by this account.
   * @param maxAutomaticTokenAssociations The max number of token associations.
   * @param aliasKey      The public key which aliases to this account.
   * @param ledgerId      The ledger ID from which the response is generated.
   * @param ethereumNonce The etheruem nonce associated with this account.
   * @param stakingInfo   The staking metadata for this account.
   */
  AccountInfo(const AccountId& accountId,
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
              const InitType<StakingInfo> stakingInfo);

  /**
   * Retrieve the account info from an account info protobuf object.
   *
   * @param accountInfo The account info protobuf object.
   * @return            An account info object.
   */
  static AccountInfo fromProtobuf(const proto::CryptoGetInfoResponse_AccountInfo& accountInfo);

  /**
   * Convert this account info object into a protobuf.
   *
   * @return The account info protobuf object.
   */
  proto::CryptoGetInfoResponse_AccountInfo toProtobuf() const;

  /**
   * The account ID of the account for which this information applies.
   */
  AccountId mAccountId;

  /**
   * The contract account ID comprising of both the contract instance and the
   * cryptocurrency account owned by the contract instance, in the format used
   * by Solidity.
   */
  std::string mContractAccountId;

  /**
   * If true, then this account has been deleted, it will disappear when it
   * expires, and all transactions for it will fail except the transaction to
   * extend its expiration date.
   */
  bool mIsDeleted;

  /**
   * The total proxy staked to this account.
   */
  Hbar mProxyReceived;

  /**
   * The key for the account, which must sign in order to transfer out, or to
   * modify the account in any way other than extending its expiration date.
   */
  std::shared_ptr<PublicKey> mKey;

  /**
   * The current balance of account.
   */
  Hbar mBalance;

  /**
   * If true, no transaction can transfer to this account unless signed by this
   * account's key.
   */
  bool mIsReceiverSignatureRequired;

  /**
   * The time at which this account is set to expire. This is held as
   * nanoseconds from the epoch.
   */
  std::chrono::nanoseconds mExpirationTime;

  /**
   * The duration for expiration time will extend every this many seconds. If
   * there are insufficient funds, then it extends as long as possible. If it is
   * empty when it expires, then it is deleted.
   */
  std::chrono::seconds mAutoRenewPeriod;

  /**
   * The list of live hashes for this account.
   */
  std::vector<LiveHash> mLiveHashes;

  /**
   * The memo associated with this account.
   */
  std::string mAccountMemo;

  /**
   * The number of NFTs owned by this account.
   */
  unsigned long long mOwnedNfts;

  /**
   * The maximum number of tokens with which an account can be implicitly
   * associated.
   */
  unsigned int mMaxAutomaticTokenAssociations;

  /**
   * The public key which aliases to this account.
   */
  std::shared_ptr<PublicKey> mAliasKey;

  /**
   * The ledger ID from which the response was returned.
   */
  LedgerId mLedgerId;

  /**
   * The ethereum nonce associated with this account.
   */
  long long mEthereumNonce;

  /**
   * Staking metadata for this account.
   */
  InitType<StakingInfo> mStakingInfo;
};

} // namespace Hedera

#endif // ACCOUNT_INFO_H_