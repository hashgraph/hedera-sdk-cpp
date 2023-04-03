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
#include "AccountInfo.h"
#include "AccountId.h"
#include "Hbar.h"
#include "LedgerId.h"
#include "PublicKey.h"
#include "impl/DurationConverter.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <chrono>
#include <gtest/gtest.h>
#include <memory>
#include <proto/crypto_get_info.pb.h>
#include <string>

using namespace Hedera;

class AccountInfoTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const std::string& getTestContractAccountId() const { return mTestContractAccountId; }
  [[nodiscard]] inline bool getTestIsDeleted() const { return mTestIsDeleted; }
  [[nodiscard]] inline const Hbar& getTestProxyReceived() const { return mTestProxyReceived; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKey() const { return mTestPublicKey; }
  [[nodiscard]] inline const Hbar& getTestBalance() const { return mTestBalance; }
  [[nodiscard]] inline bool getTestReceiverSignatureRequired() const { return mTestReceiverSignatureRequired; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestExpirationTime() const
  {
    return mTestExpirationTime;
  }
  [[nodiscard]] inline const std::chrono::duration<double>& getTestAutoRenewPeriod() const
  {
    return mTestAutoRenewPeriod;
  }
  [[nodiscard]] inline const std::string& getTestMemo() const { return mTestMemo; }
  [[nodiscard]] inline const uint64_t& getTestOwnedNfts() const { return mTestOwnedNfts; }
  [[nodiscard]] inline uint32_t getTestMaxAutomaticTokenAssociations() const { return mMaxAutomaticTokenAssociations; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestAlias() const { return mTestAlias; }
  [[nodiscard]] inline const LedgerId& getTestLedgerId() const { return mTestLedgerId; }
  [[nodiscard]] inline bool getTestDeclineReward() const { return mTestDeclineReward; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestStakePeriodStart() const
  {
    return mTestStakePeriodStart;
  }
  [[nodiscard]] inline const Hbar& getTestPendingReward() const { return mTestPendingReward; }
  [[nodiscard]] inline const Hbar& getTestStakedToMe() const { return mTestStakedToMe; }
  [[nodiscard]] inline const AccountId& getTestStakedAccountId() const { return mTestStakedAccountId; }

private:
  const AccountId mTestAccountId = AccountId(1ULL);
  const std::string mTestContractAccountId = "ContractAccountId";
  const bool mTestIsDeleted = true;
  const Hbar mTestProxyReceived = Hbar(2LL);
  const std::shared_ptr<PublicKey> mTestPublicKey = PublicKey::fromStringDer(
    "302A300506032B6570032100d75a980182b10ab7d54bfed3c964073a0ee172f3daa62325af021a68f707511a");
  const Hbar mTestBalance = Hbar(3LL);
  const bool mTestReceiverSignatureRequired = true;
  const std::chrono::system_clock::time_point mTestExpirationTime = std::chrono::system_clock::now();
  const std::chrono::duration<double> mTestAutoRenewPeriod = std::chrono::hours(4);
  const std::string mTestMemo = "test memo";
  const uint64_t mTestOwnedNfts = 5ULL;
  const uint32_t mMaxAutomaticTokenAssociations = 6U;
  const std::shared_ptr<PublicKey> mTestAlias = PublicKey::fromStringDer(
    "302A300506032B6570032100fc51cd8e6218a1a38da47ed00230f0580816ed13ba3303ac5deb911548908025");
  const LedgerId mTestLedgerId = LedgerId({ std::byte(0x07), std::byte(0x08), std::byte(0x09) });
  const bool mTestDeclineReward = true;
  const std::chrono::system_clock::time_point mTestStakePeriodStart = std::chrono::system_clock::now();
  const Hbar mTestPendingReward = Hbar(10LL);
  const Hbar mTestStakedToMe = Hbar(11LL);
  const AccountId mTestStakedAccountId = AccountId(12ULL);
};

//-----
TEST_F(AccountInfoTest, FromProtobuf)
{
  // Given
  proto::CryptoGetInfoResponse_AccountInfo protoAccountInfo;
  protoAccountInfo.set_allocated_accountid(getTestAccountId().toProtobuf().release());
  protoAccountInfo.set_allocated_contractaccountid(new std::string(getTestContractAccountId()));
  protoAccountInfo.set_deleted(getTestIsDeleted());
  protoAccountInfo.set_proxyreceived(getTestProxyReceived().toTinybars());
  protoAccountInfo.set_allocated_key(getTestPublicKey()->toProtobuf().release());
  protoAccountInfo.set_balance(static_cast<uint64_t>(getTestBalance().toTinybars()));
  protoAccountInfo.set_receiversigrequired(getTestReceiverSignatureRequired());
  protoAccountInfo.set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(getTestExpirationTime()));
  protoAccountInfo.set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(getTestAutoRenewPeriod()));
  protoAccountInfo.set_allocated_memo(new std::string(getTestMemo()));
  protoAccountInfo.set_ownednfts(static_cast<int64_t>(getTestOwnedNfts()));
  protoAccountInfo.set_max_automatic_token_associations(static_cast<int32_t>(getTestMaxAutomaticTokenAssociations()));
  protoAccountInfo.set_allocated_alias(
    new std::string(internal::Utilities::byteVectorToString(getTestAlias()->toBytesDer())));
  protoAccountInfo.set_allocated_ledger_id(
    new std::string(internal::Utilities::byteVectorToString(getTestLedgerId().toBytes())));

  protoAccountInfo.mutable_staking_info()->set_decline_reward(getTestDeclineReward());
  protoAccountInfo.mutable_staking_info()->set_allocated_stake_period_start(
    internal::TimestampConverter::toProtobuf(getTestStakePeriodStart()));
  protoAccountInfo.mutable_staking_info()->set_pending_reward(getTestPendingReward().toTinybars());
  protoAccountInfo.mutable_staking_info()->set_staked_to_me(getTestStakedToMe().toTinybars());
  protoAccountInfo.mutable_staking_info()->set_allocated_staked_account_id(
    getTestStakedAccountId().toProtobuf().release());

  // When
  const AccountInfo accountInfo = AccountInfo::fromProtobuf(protoAccountInfo);

  // Then
  EXPECT_EQ(accountInfo.getAccountId(), getTestAccountId());
  EXPECT_EQ(accountInfo.getContractAccountId(), getTestContractAccountId());
  EXPECT_EQ(accountInfo.getIsDeleted(), getTestIsDeleted());
  EXPECT_EQ(accountInfo.getProxyReceived(), getTestProxyReceived());
  EXPECT_EQ(accountInfo.getKey()->toBytesDer(), getTestPublicKey()->toBytesDer());
  EXPECT_EQ(accountInfo.getBalance(), getTestBalance());
  EXPECT_EQ(accountInfo.getReceiverSignatureRequired(), getTestReceiverSignatureRequired());
  EXPECT_EQ(accountInfo.getExpirationTime(), getTestExpirationTime());
  EXPECT_EQ(accountInfo.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  EXPECT_EQ(accountInfo.getMemo(), getTestMemo());
  EXPECT_EQ(accountInfo.getOwnedNfts(), getTestOwnedNfts());
  EXPECT_EQ(accountInfo.getMaxAutomaticTokenAssociations(), getTestMaxAutomaticTokenAssociations());
  EXPECT_EQ(accountInfo.getPublicKeyAlias()->toBytesDer(), getTestAlias()->toBytesDer());
  EXPECT_EQ(accountInfo.getLedgerId().toBytes(), getTestLedgerId().toBytes());
  EXPECT_EQ(accountInfo.getStakingInfo().getDeclineReward(), getTestDeclineReward());
  EXPECT_EQ(accountInfo.getStakingInfo().getStakePeriodStart(), getTestStakePeriodStart());
  EXPECT_EQ(accountInfo.getStakingInfo().getPendingReward(), getTestPendingReward());
  EXPECT_EQ(accountInfo.getStakingInfo().getStakedToMe(), getTestStakedToMe());
  ASSERT_TRUE(accountInfo.getStakingInfo().getStakedAccountId().has_value());
  EXPECT_EQ(accountInfo.getStakingInfo().getStakedAccountId(), getTestStakedAccountId());
  EXPECT_FALSE(accountInfo.getStakingInfo().getStakedNodeId().has_value());
}
