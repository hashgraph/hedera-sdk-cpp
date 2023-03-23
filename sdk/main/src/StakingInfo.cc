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
#include "StakingInfo.h"
#include "impl/TimestampConverter.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
StakingInfo StakingInfo::fromProtobuf(const proto::StakingInfo& proto)
{
  StakingInfo stakingInfo;
  stakingInfo.mDeclineRewards = proto.decline_reward();

  if (proto.has_stake_period_start())
  {
    stakingInfo.mStakePeriodStart = internal::TimestampConverter::fromProtobuf(proto.stake_period_start());
  }

  stakingInfo.mPendingReward = Hbar(proto.pending_reward(), HbarUnit::TINYBAR());
  stakingInfo.mStakedToMe = Hbar(proto.staked_to_me(), HbarUnit::TINYBAR());

  if (proto.has_staked_account_id())
  {
    stakingInfo.mStakedAccountId = AccountId::fromProtobuf(proto.staked_account_id());
  }

  if (proto.has_staked_node_id())
  {
    stakingInfo.mStakedNodeId = static_cast<uint64_t>(proto.staked_node_id());
  }

  return stakingInfo;
}

} // namespace Hedera
