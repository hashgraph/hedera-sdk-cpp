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
#include "StakingInfo.h"
#include "impl/TimestampConverter.h"

#include <chrono>
#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class StakingInfoUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline bool getTestDeclineReward() const { return mTestDeclineReward; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestStakePeriodStart() const
  {
    return mTestStakePeriodStart;
  }
  [[nodiscard]] inline const int64_t& getTestPendingReward() const { return mTestPendingReward; }
  [[nodiscard]] inline const int64_t& getTestStakedToMe() const { return mTestStakedToMe; }
  [[nodiscard]] inline const int64_t& getTestStakedNodeId() const { return mTestStakedNodeId; }

private:
  const bool mTestDeclineReward = true;
  const std::chrono::system_clock::time_point mTestStakePeriodStart = std::chrono::system_clock::now();
  const int64_t mTestPendingReward = 1LL;
  const int64_t mTestStakedToMe = 2LL;
  const int64_t mTestStakedNodeId = 3LL;
};

//-----
TEST_F(StakingInfoUnitTests, FromProtobuf)
{
  // Given
  proto::StakingInfo protoStakingInfo;
  protoStakingInfo.set_decline_reward(getTestDeclineReward());
  protoStakingInfo.set_allocated_stake_period_start(
    internal::TimestampConverter::toProtobuf(getTestStakePeriodStart()));
  protoStakingInfo.set_pending_reward(getTestPendingReward());
  protoStakingInfo.set_staked_to_me(getTestStakedToMe());
  protoStakingInfo.set_staked_node_id(getTestStakedNodeId());

  // When
  const StakingInfo stakingInfo = StakingInfo::fromProtobuf(protoStakingInfo);

  // Then
  EXPECT_EQ(stakingInfo.getDeclineReward(), getTestDeclineReward());
  ASSERT_TRUE(stakingInfo.getStakePeriodStart().has_value());
  EXPECT_EQ(*stakingInfo.getStakePeriodStart(), getTestStakePeriodStart());
  EXPECT_EQ(stakingInfo.getPendingReward().toTinybars(), getTestPendingReward());
  EXPECT_EQ(stakingInfo.getStakedToMe().toTinybars(), getTestStakedToMe());
  EXPECT_FALSE(stakingInfo.getStakedAccountId().has_value());
  ASSERT_TRUE(stakingInfo.getStakedNodeId().has_value());
  EXPECT_EQ(*stakingInfo.getStakedNodeId(), getTestStakedNodeId());
}
