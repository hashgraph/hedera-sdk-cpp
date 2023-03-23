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
#ifndef HEDERA_SDK_CPP_STAKING_INFO_H_
#define HEDERA_SDK_CPP_STAKING_INFO_H_

#include "AccountId.h"
#include "Hbar.h"

#include <chrono>
#include <optional>

namespace proto
{
class StakingInfo;
}

namespace Hedera
{
/**
 * The staking metadata for an account or contract.
 */
class StakingInfo
{
public:
  /**
   * Construct an StakingInfo object from a StakingInfo protobuf object.
   *
   * @param proto The StakingInfo protobuf object from which to construct an StakingInfo object.
   * @return The constructed StakingInfo object.
   */
  [[nodiscard]] static StakingInfo fromProtobuf(const proto::StakingInfo& proto);

  /**
   * Get the decline reward policy of the account/contract.
   *
   * @return \c TRUE if the account/contract is declining to receive staking rewards, otherwise \c FALSE.
   */
  [[nodiscard]] inline bool getDeclineReward() const { return mDeclineRewards; }

  /**
   * Get the current staking period start time.
   *
   * @return The current staking period start time. Uninitialized if not staked to a node.
   */
  [[nodiscard]] inline std::optional<std::chrono::system_clock::time_point> getStakePeriodStart() const
  {
    return mStakePeriodStart;
  }

  /**
   * Get the amount of Hbar that the account/contract will receive in the next reward situation.
   *
   * @return The amount of Hbar that the account/contract will receive in the next reward situation.
   */
  [[nodiscard]] inline Hbar getPendingReward() const { return mPendingReward; }

  /**
   * Get the total balance of all accounts staked to the account/contract.
   *
   * @return The total balance of all accounts staked to the account/contract.
   */
  [[nodiscard]] inline Hbar getStakedToMe() const { return mStakedToMe; }

  /**
   * Get the ID of the account to which the account/contract is staking.
   *
   * @return The ID of the account to which the account/contract is staking. Uninitialized if not staking to an account.
   */
  [[nodiscard]] inline std::optional<AccountId> getStakedAccountId() const { return mStakedAccountId; }

  /**
   * Get the ID of the node to which the account/contract is staking.
   *
   * @return The ID of the node to which the account/contract is staking. Uninitialized if not staking to a node.
   */
  [[nodiscard]] inline std::optional<uint64_t> getStakedNodeId() const { return mStakedNodeId; }

private:
  /**
   * Is this account/contract declining to receive staking rewards?
   */
  bool mDeclineRewards = false;

  /**
   * The staking period during which either the staking settings for this account/contract changed (such as starting
   * staking or changing mStakedNodeId) or the most recent reward was earned, whichever is later. If this
   * account/contract is not currently staked to a node, then this field is not set.
   */
  std::optional<std::chrono::system_clock::time_point> mStakePeriodStart;

  /**
   * The amount of Hbar that this account/contract will receive in the next reward situation.
   */
  Hbar mPendingReward = Hbar(0LL);

  /**
   * The total balance of all accounts staked to this account/contract.
   */
  Hbar mStakedToMe = Hbar(0LL);

  /**
   * The ID of the account to which the account/contract is staking. Mutually exclusive with mStakedNodeId.
   */
  std::optional<AccountId> mStakedAccountId;

  /**
   * The ID of the node to which the account/contract is staking. Mutually exclusive with mStakedAccountId.
   */
  std::optional<uint64_t> mStakedNodeId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_STAKING_INFO_H_
