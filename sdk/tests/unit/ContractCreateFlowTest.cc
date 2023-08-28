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
#include "ContractCreateFlow.h"
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "FileId.h"
#include "PublicKey.h"
#include "impl/DurationConverter.h"
#include "impl/Utilities.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>
#include <vector>

using namespace Hedera;

class ContractCreateFlowTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ED25519PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestBytecode() const { return mTestBytecode; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestAdminKey() const { return mTestAdminKey; }
  [[nodiscard]] inline const uint64_t& getTestGas() const { return mTestGas; }
  [[nodiscard]] inline const Hbar& getTestInitialBalance() const { return mTestInitialBalance; }
  [[nodiscard]] inline const std::chrono::duration<double>& getTestAutoRenewPeriod() const
  {
    return mTestAutoRenewPeriod;
  }
  [[nodiscard]] inline const std::vector<std::byte>& getTestConstructorParameters() const
  {
    return mTestConstructorParameters;
  }
  [[nodiscard]] inline const std::string& getTestMemo() const { return mTestMemo; }
  [[nodiscard]] inline uint32_t getTestMaximumTokenAssociations() const { return mTestMaxTokenAssociations; }
  [[nodiscard]] inline const AccountId& getTestAutoRenewAccountId() const { return mTestAutoRenewAccountId; }
  [[nodiscard]] inline const AccountId& getTestStakedAccountId() const { return mTestStakedAccountId; }
  [[nodiscard]] inline const uint64_t& getTestStakedNodeId() const { return mTestStakedNodeId; }
  [[nodiscard]] inline bool getTestDeclineStakingReward() const { return mTestDeclineStakingReward; }
  [[nodiscard]] inline const std::vector<AccountId>& getTestNodeAccountIds() const { return mTestNodeAccountIds; }
  [[nodiscard]] inline unsigned int getTestMaxChunks() const { return mTestMaxChunks; }

private:
  Client mClient;
  const std::vector<std::byte> mTestBytecode = { std::byte(0x02), std::byte(0x03), std::byte(0x04) };
  const std::shared_ptr<PublicKey> mTestAdminKey = PublicKey::fromStringDer(
    "302A300506032B6570032100BCAF3153262A767B281CC8C888DB3E097C83D690AEF01B8C1BE64D3DE11AACC3");
  const uint64_t mTestGas = 5ULL;
  const Hbar mTestInitialBalance = Hbar(6LL);
  const std::chrono::duration<double> mTestAutoRenewPeriod = std::chrono::hours(7);
  const std::vector<std::byte> mTestConstructorParameters = { std::byte(0x08), std::byte(0x09), std::byte(0x10) };
  const std::string mTestMemo = "test smart contract memo";
  const uint32_t mTestMaxTokenAssociations = 11U;
  const AccountId mTestAutoRenewAccountId = AccountId(12ULL);
  const AccountId mTestStakedAccountId = AccountId(13ULL);
  const uint64_t mTestStakedNodeId = 14ULL;
  const bool mTestDeclineStakingReward = true;
  const std::vector<AccountId> mTestNodeAccountIds = { AccountId(15ULL), AccountId(16ULL), AccountId(17ULL) };
  const unsigned int mTestMaxChunks = 18ULL;
};

//-----
TEST_F(ContractCreateFlowTest, GetSetBytecode)
{
  // Given
  ContractCreateFlow flowBytes;
  ContractCreateFlow flowStr;

  // When
  EXPECT_NO_THROW(flowBytes.setBytecode(getTestBytecode()));
  EXPECT_NO_THROW(flowStr.setBytecode(internal::Utilities::byteVectorToString(getTestBytecode())));

  // Then
  EXPECT_EQ(flowBytes.getBytecode(), getTestBytecode());
  EXPECT_EQ(flowStr.getBytecode(), getTestBytecode());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetAdminKey)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setAdminKey(getTestAdminKey().get()));

  // Then
  EXPECT_EQ(flow.getAdminKey()->toBytes(), getTestAdminKey()->toBytes());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetGas)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setGas(getTestGas()));

  // Then
  EXPECT_EQ(flow.getGas(), getTestGas());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetInitialBalance)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setInitialBalance(getTestInitialBalance()));

  // Then
  EXPECT_EQ(flow.getInitialBalance(), getTestInitialBalance());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetAutoRenewPeriod)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setAutoRenewPeriod(getTestAutoRenewPeriod()));

  // Then
  EXPECT_EQ(flow.getAutoRenewPeriod(), getTestAutoRenewPeriod());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetConstructorParameters)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setConstructorParameters(getTestConstructorParameters()));

  // Then
  EXPECT_EQ(flow.getConstructorParameters(), getTestConstructorParameters());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetMemo)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setMemo(getTestMemo()));

  // Then
  EXPECT_EQ(flow.getMemo(), getTestMemo());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetMaxAutomaticTokenAssociations)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setMaxAutomaticTokenAssociations(getTestMaximumTokenAssociations()));

  // Then
  EXPECT_EQ(flow.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetAutoRenewAccountId)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setAutoRenewAccountId(getTestAutoRenewAccountId()));

  // Then
  EXPECT_EQ(flow.getAutoRenewAccountId(), getTestAutoRenewAccountId());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetStakedAccountId)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setStakedAccountId(getTestStakedAccountId()));

  // Then
  EXPECT_EQ(flow.getStakedAccountId(), getTestStakedAccountId());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetStakedNodeId)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setStakedNodeId(getTestStakedNodeId()));

  // Then
  EXPECT_EQ(flow.getStakedNodeId(), getTestStakedNodeId());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetDeclineReward)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setDeclineStakingReward(getTestDeclineStakingReward()));

  // Then
  EXPECT_EQ(flow.getDeclineStakingReward(), getTestDeclineStakingReward());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetNodeAccountIds)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setNodeAccountIds(getTestNodeAccountIds()));

  // Then
  EXPECT_EQ(flow.getNodeAccountIds(), getTestNodeAccountIds());
}

//-----
TEST_F(ContractCreateFlowTest, GetSetMaxChunks)
{
  // Given
  ContractCreateFlow flow;

  // When
  EXPECT_NO_THROW(flow.setMaxChunks(getTestMaxChunks()));

  // Then
  EXPECT_EQ(flow.getMaxChunks(), getTestMaxChunks());
}

//-----
TEST_F(ContractCreateFlowTest, ResetStakedAccountIdWhenSettingStakedNodeId)
{
  // Given
  ContractCreateFlow flow;

  // When
  flow.setStakedNodeId(getTestStakedNodeId());

  // Then
  EXPECT_FALSE(flow.getStakedAccountId().has_value());
}

//-----
TEST_F(ContractCreateFlowTest, ResetStakedNodeIdWhenSettingStakedAccountId)
{
  // Given
  ContractCreateFlow flow;

  // When
  flow.setStakedAccountId(getTestStakedAccountId());

  // Then
  EXPECT_FALSE(flow.getStakedNodeId().has_value());
}
