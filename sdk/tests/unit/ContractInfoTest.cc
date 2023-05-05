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
#include "ContractInfo.h"
#include "impl/DurationConverter.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/contract_get_info.pb.h>

using namespace Hedera;

class ContractInfoTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const ContractId& getTestContractId() const { return mTestContractId; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const std::string& getTestContractAccountId() const { return mTestContractAccountId; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestAdminKey() const { return mTestAdminKey; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestExpirationTime() const
  {
    return mTestExpirationTime;
  }
  [[nodiscard]] inline const std::chrono::duration<double>& getTestAutoRenewPeriod() const
  {
    return mTestAutoRenewPeriod;
  }
  [[nodiscard]] inline const uint64_t& getTestStorage() const { return mTestStorage; }
  [[nodiscard]] inline const std::string& getTestMemo() const { return mTestMemo; }
  [[nodiscard]] inline const Hbar& getTestBalance() const { return mTestBalance; }
  [[nodiscard]] inline bool getTestIsDeleted() const { return mTestIsDeleted; }
  [[nodiscard]] inline const LedgerId& getTestLedgerId() const { return mTestLedgerId; }
  [[nodiscard]] inline const AccountId& getTestAutoRenewAccountId() const { return mTestAutoRenewAccountId; }
  [[nodiscard]] inline int32_t getTestMaxAutomaticTokenAssociations() const { return mMaxAutomaticTokenAssociations; }
  [[nodiscard]] inline bool getTestDeclineReward() const { return mTestDeclineReward; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestStakePeriodStart() const
  {
    return mTestStakePeriodStart;
  }
  [[nodiscard]] inline const Hbar& getTestPendingReward() const { return mTestPendingReward; }
  [[nodiscard]] inline const Hbar& getTestStakedToMe() const { return mTestStakedToMe; }
  [[nodiscard]] inline const AccountId& getTestStakedAccountId() const { return mTestStakedAccountId; }

private:
  const ContractId mTestContractId = ContractId(1ULL);
  const AccountId mTestAccountId = AccountId(2ULL);
  const std::string mTestContractAccountId = "ContractAccountId";
  const std::shared_ptr<PublicKey> mTestAdminKey = PublicKey::fromStringDer(
    "302A300506032B6570032100D75A980182B10AB7D54BFED3C964073A0EE172f3DAA62325AF021A68F707511A");
  const std::chrono::system_clock::time_point mTestExpirationTime = std::chrono::system_clock::now();
  const std::chrono::duration<double> mTestAutoRenewPeriod = std::chrono::hours(3);
  const uint64_t mTestStorage = 40000ULL;
  const std::string mTestMemo = "test memo";
  const Hbar mTestBalance = Hbar(5LL);
  const bool mTestIsDeleted = true;
  const LedgerId mTestLedgerId = LedgerId({ std::byte(0x06), std::byte(0x07), std::byte(0x08) });
  const AccountId mTestAutoRenewAccountId = AccountId(9ULL);
  const int32_t mMaxAutomaticTokenAssociations = 10;
  const bool mTestDeclineReward = true;
  const std::chrono::system_clock::time_point mTestStakePeriodStart = std::chrono::system_clock::now();
  const Hbar mTestPendingReward = Hbar(11LL);
  const Hbar mTestStakedToMe = Hbar(12LL);
  const AccountId mTestStakedAccountId = AccountId(13ULL);
};

//-----
TEST_F(ContractInfoTest, FromProtobuf)
{
  // Given
  proto::ContractGetInfoResponse_ContractInfo protoContractInfo;
  protoContractInfo.set_allocated_contractid(getTestContractId().toProtobuf().release());
  protoContractInfo.set_allocated_accountid(getTestAccountId().toProtobuf().release());
  protoContractInfo.set_contractaccountid(getTestContractAccountId());
  protoContractInfo.set_deleted(getTestIsDeleted());
  protoContractInfo.set_allocated_adminkey(getTestAdminKey()->toProtobuf().release());
  protoContractInfo.set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(getTestExpirationTime()));
  protoContractInfo.set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(getTestAutoRenewPeriod()));
  protoContractInfo.set_storage(static_cast<int64_t>(getTestStorage()));
  protoContractInfo.set_memo(getTestMemo());
  protoContractInfo.set_balance(static_cast<uint64_t>(getTestBalance().toTinybars()));
  protoContractInfo.set_deleted(getTestIsDeleted());
  protoContractInfo.set_ledger_id(internal::Utilities::byteVectorToString(getTestLedgerId().toBytes()));
  protoContractInfo.set_allocated_auto_renew_account_id(getTestAutoRenewAccountId().toProtobuf().release());
  protoContractInfo.set_max_automatic_token_associations(getTestMaxAutomaticTokenAssociations());

  protoContractInfo.mutable_staking_info()->set_decline_reward(getTestDeclineReward());
  protoContractInfo.mutable_staking_info()->set_allocated_stake_period_start(
    internal::TimestampConverter::toProtobuf(getTestStakePeriodStart()));
  protoContractInfo.mutable_staking_info()->set_pending_reward(getTestPendingReward().toTinybars());
  protoContractInfo.mutable_staking_info()->set_staked_to_me(getTestStakedToMe().toTinybars());
  protoContractInfo.mutable_staking_info()->set_allocated_staked_account_id(
    getTestStakedAccountId().toProtobuf().release());

  // When
  const ContractInfo contractInfo = ContractInfo::fromProtobuf(protoContractInfo);

  // Then
  EXPECT_EQ(contractInfo.mContractId, getTestContractId());
  EXPECT_EQ(contractInfo.mAccountId, getTestAccountId());
  EXPECT_EQ(contractInfo.mContractAccountId, getTestContractAccountId());
  ASSERT_NE(contractInfo.mAdminKey, nullptr);
  EXPECT_EQ(contractInfo.mAdminKey->toBytesDer(), getTestAdminKey()->toBytesDer());
  EXPECT_EQ(contractInfo.mExpirationTime, getTestExpirationTime());
  EXPECT_EQ(contractInfo.mAutoRenewPeriod, getTestAutoRenewPeriod());
  EXPECT_EQ(contractInfo.mStorage, getTestStorage());
  EXPECT_EQ(contractInfo.mMemo, getTestMemo());
  EXPECT_EQ(contractInfo.mBalance, getTestBalance());
  EXPECT_EQ(contractInfo.mIsDeleted, getTestIsDeleted());
  EXPECT_EQ(contractInfo.mLedgerId.toBytes(), getTestLedgerId().toBytes());
  ASSERT_TRUE(contractInfo.mAutoRenewAccountId.has_value());
  EXPECT_EQ(contractInfo.mAutoRenewAccountId, getTestAutoRenewAccountId());
  EXPECT_EQ(contractInfo.mMaxAutomaticTokenAssociations, getTestMaxAutomaticTokenAssociations());

  EXPECT_EQ(contractInfo.mStakingInfo.getDeclineReward(), getTestDeclineReward());
  EXPECT_EQ(contractInfo.mStakingInfo.getStakePeriodStart(), getTestStakePeriodStart());
  EXPECT_EQ(contractInfo.mStakingInfo.getPendingReward(), getTestPendingReward());
  EXPECT_EQ(contractInfo.mStakingInfo.getStakedToMe(), getTestStakedToMe());
  ASSERT_TRUE(contractInfo.mStakingInfo.getStakedAccountId().has_value());
  EXPECT_EQ(contractInfo.mStakingInfo.getStakedAccountId(), getTestStakedAccountId());
  EXPECT_FALSE(contractInfo.mStakingInfo.getStakedNodeId().has_value());
}
