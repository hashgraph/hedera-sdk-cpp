/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_ACCOUNT_INFO_H_
#define HEDERA_SDK_CPP_ACCOUNT_INFO_H_

#include "AccountId.h"
#include "EvmAddress.h"
#include "Hbar.h"
#include "Key.h"
#include "LedgerId.h"
#include "StakingInfo.h"

#include <chrono>
#include <memory>
#include <optional>
#include <string>

namespace proto
{
class CryptoGetInfoResponse_AccountInfo;
}

namespace Hedera
{
/**
 * Response from a Hedera network when the client sends an AccountInfoQuery.
 */
class AccountInfo
{
public:
  /**
   * Construct an AccountInfo object from a CryptoGetInfoResponse_AccountInfo protobuf object.
   *
   * @param proto The CryptoGetInfoResponse_AccountInfo protobuf object from which to construct an AccountInfo object.
   * @return The constructed AccountInfo object.
   */
  [[nodiscard]] static AccountInfo fromProtobuf(const proto::CryptoGetInfoResponse_AccountInfo& proto);

  /**
   * The ID of the queried account.
   */
  AccountId mAccountId;

  /**
   * The contract account ID comprising of both the contract instance and the cryptocurrency account owned by the
   * contract instance, in the format used by Solidity.
   */
  std::string mContractAccountId;

  /**
   * Is the queried account deleted or not? If \c TRUE, then the account has been deleted, it will disappear when it
   * expires, and all transactions for it will fail except the transaction to extend its expiration date.
   */
  bool mIsDeleted = false;

  /**
   * The total amount of Hbar proxy staked to the queried account.
   */
  Hbar mProxyReceived = Hbar(0LL);

  /**
   * The key for the account, which must sign in order to transfer out, or to modify the account in any way other than
   * extending its expiration date.
   */
  std::shared_ptr<Key> mKey = nullptr;

  /**
   * The current balance of the queried account.
   */
  Hbar mBalance = Hbar(0LL);

  /**
   * If \c TRUE, the queried account's key must sign any transaction being deposited into it (in addition to all
   * withdrawals).
   */
  bool mReceiverSignatureRequired = false;

  /**
   * The time at which the queried account will expire.
   */
  std::chrono::system_clock::time_point mExpirationTime = std::chrono::system_clock::now();

  /**
   * The duration of time the queried account uses to automatically extend its expiration period. It it doesn't have
   * enough balance, it extends as long as possible. If it is empty when it expires, then it is deleted.
   */
  std::chrono::system_clock::duration mAutoRenewPeriod;

  /**
   * The queried account's memo.
   */
  std::string mMemo;

  /**
   * The number of NFTs owned by the queried account.
   */
  uint64_t mOwnedNfts = 0ULL;

  /**
   * The maximum number of tokens with which the queried account can be associated.
   */
  uint32_t mMaxAutomaticTokenAssociations = 0U;

  /**
   * The PublicKey alias of the queried account.
   */
  std::shared_ptr<PublicKey> mPublicKeyAlias = nullptr;

  /**
   * The EVM address alias of the queried account.
   */
  std::optional<EvmAddress> mEvmAddressAlias;

  /**
   * The ID of the ledger from which this AccountInfo was received.
   */
  LedgerId mLedgerId;

  /**
   * The staking metadata for the queried account.
   */
  StakingInfo mStakingInfo;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ACCOUNT_INFO_H_
