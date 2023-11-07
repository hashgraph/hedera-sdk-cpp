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
#include "ContractCreateTransaction.h"
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "FileId.h"
#include "PublicKey.h"
#include "exceptions/IllegalStateException.h"
#include "impl/DurationConverter.h"
#include "impl/Utilities.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>
#include <vector>

using namespace Hedera;

class ContractCreateTransactionTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const FileId& getTestFileId() const { return mTestFileId; }
  [[nodiscard]] inline const std::vector<std::byte>& getTestBytecode() const { return mTestBytecode; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestAdminKey() const { return mTestAdminKey; }
  [[nodiscard]] inline const uint64_t& getTestGas() const { return mTestGas; }
  [[nodiscard]] inline const Hbar& getTestInitialBalance() const { return mTestInitialBalance; }
  [[nodiscard]] inline const std::chrono::system_clock::duration& getTestAutoRenewPeriod() const
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

private:
  const FileId mTestFileId = FileId(1ULL);
  const std::vector<std::byte> mTestBytecode = { std::byte(0x02), std::byte(0x03), std::byte(0x04) };
  const std::shared_ptr<PublicKey> mTestAdminKey = PublicKey::fromStringDer(
    "302A300506032B6570032100BCAF3153262A767B281CC8C888DB3E097C83D690AEF01B8C1BE64D3DE11AACC3");
  const uint64_t mTestGas = 5ULL;
  const Hbar mTestInitialBalance = Hbar(6LL);
  const std::chrono::system_clock::duration mTestAutoRenewPeriod = std::chrono::hours(7);
  const std::vector<std::byte> mTestConstructorParameters = { std::byte(0x08), std::byte(0x09), std::byte(0x10) };
  const std::string mTestMemo = "test smart contract memo";
  const uint32_t mTestMaxTokenAssociations = 11U;
  const AccountId mTestAutoRenewAccountId = AccountId(12ULL);
  const AccountId mTestStakedAccountId = AccountId(13ULL);
  const uint64_t mTestStakedNodeId = 14ULL;
  const bool mTestDeclineStakingReward = true;
};

//-----
TEST_F(ContractCreateTransactionTest, ConstructContractCreateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::ContractCreateTransactionBody>();
  body->set_allocated_fileid(getTestFileId().toProtobuf().release());
  body->set_allocated_adminkey(getTestAdminKey()->toProtobufKey().release());
  body->set_gas(static_cast<int64_t>(getTestGas()));
  body->set_initialbalance(getTestInitialBalance().toTinybars());
  body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(getTestAutoRenewPeriod()));
  body->set_allocated_constructorparameters(
    new std::string(internal::Utilities::byteVectorToString(getTestConstructorParameters())));
  body->set_allocated_memo(new std::string(getTestMemo()));
  body->set_max_automatic_token_associations(static_cast<int32_t>(getTestMaximumTokenAssociations()));
  body->set_allocated_auto_renew_account_id(getTestAutoRenewAccountId().toProtobuf().release());
  body->set_allocated_staked_account_id(getTestStakedAccountId().toProtobuf().release());
  body->set_decline_reward(getTestDeclineStakingReward());

  proto::TransactionBody txBody;
  txBody.set_allocated_contractcreateinstance(body.release());

  // When
  const ContractCreateTransaction contractCreateTransaction(txBody);

  // Then
  ASSERT_TRUE(contractCreateTransaction.getFileId().has_value());
  EXPECT_EQ(contractCreateTransaction.getFileId(), getTestFileId());
  EXPECT_FALSE(contractCreateTransaction.getInitCode().has_value());
  EXPECT_EQ(contractCreateTransaction.getAdminKey()->toBytes(), getTestAdminKey()->toBytes());
  EXPECT_EQ(contractCreateTransaction.getGas(), getTestGas());
  EXPECT_EQ(contractCreateTransaction.getInitialBalance(), getTestInitialBalance());
  EXPECT_EQ(contractCreateTransaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  EXPECT_EQ(contractCreateTransaction.getConstructorParameters(), getTestConstructorParameters());
  EXPECT_EQ(contractCreateTransaction.getMemo(), getTestMemo());
  EXPECT_EQ(contractCreateTransaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
  EXPECT_EQ(contractCreateTransaction.getAutoRenewAccountId(), getTestAutoRenewAccountId());
  ASSERT_TRUE(contractCreateTransaction.getStakedAccountId().has_value());
  EXPECT_EQ(contractCreateTransaction.getStakedAccountId(), getTestStakedAccountId());
  EXPECT_FALSE(contractCreateTransaction.getStakedNodeId().has_value());
  EXPECT_EQ(contractCreateTransaction.getDeclineStakingReward(), getTestDeclineStakingReward());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetFileId)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setBytecodeFileId(getTestFileId()));

  // Then
  ASSERT_TRUE(transaction.getFileId().has_value());
  EXPECT_EQ(transaction.getFileId(), getTestFileId());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetFileIdFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setBytecodeFileId(getTestFileId()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetInitCode)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setBytecode(getTestBytecode()));

  // Then
  EXPECT_EQ(transaction.getInitCode(), getTestBytecode());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetInitCodeFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setBytecode(getTestBytecode()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetAdminKey)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAdminKey(getTestAdminKey()));

  // Then
  EXPECT_EQ(transaction.getAdminKey()->toBytes(), getTestAdminKey()->toBytes());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetAdminKeyFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAdminKey(getTestAdminKey()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetGas)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setGas(getTestGas()));

  // Then
  EXPECT_EQ(transaction.getGas(), getTestGas());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetGasFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setGas(getTestGas()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetInitialBalance)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setInitialBalance(getTestInitialBalance()));

  // Then
  EXPECT_EQ(transaction.getInitialBalance(), getTestInitialBalance());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetInitialBalanceFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setInitialBalance(getTestInitialBalance()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetAutoRenewPeriod)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAutoRenewPeriod(getTestAutoRenewPeriod()));

  // Then
  EXPECT_EQ(transaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetAutoRenewPeriodFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAutoRenewPeriod(getTestAutoRenewPeriod()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetConstructorParameters)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setConstructorParameters(getTestConstructorParameters()));

  // Then
  EXPECT_EQ(transaction.getConstructorParameters(), getTestConstructorParameters());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetConstructorParametersFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setConstructorParameters(getTestConstructorParameters()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetMemo)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setMemo(getTestMemo()));

  // Then
  EXPECT_EQ(transaction.getMemo(), getTestMemo());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetMemoFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setMemo(getTestMemo()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetMaxAutomaticTokenAssociations)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setMaxAutomaticTokenAssociations(getTestMaximumTokenAssociations()));

  // Then
  EXPECT_EQ(transaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetMaxAutomaticTokenAssociationsFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setMaxAutomaticTokenAssociations(getTestMaximumTokenAssociations()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetAutoRenewAccountId)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAutoRenewAccountId(getTestAutoRenewAccountId()));

  // Then
  EXPECT_EQ(transaction.getAutoRenewAccountId(), getTestAutoRenewAccountId());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetAutoRenewAccountIdFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAutoRenewAccountId(getTestAutoRenewAccountId()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetStakedAccountId)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setStakedAccountId(getTestStakedAccountId()));

  // Then
  EXPECT_EQ(transaction.getStakedAccountId(), getTestStakedAccountId());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetStakedAccountIdFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setStakedAccountId(getTestStakedAccountId()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetStakedNodeId)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setStakedNodeId(getTestStakedNodeId()));

  // Then
  EXPECT_EQ(transaction.getStakedNodeId(), getTestStakedNodeId());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetStakedNodeIdFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setStakedNodeId(getTestStakedNodeId()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetDeclineReward)
{
  // Given
  ContractCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setDeclineStakingReward(getTestDeclineStakingReward()));

  // Then
  EXPECT_EQ(transaction.getDeclineStakingReward(), getTestDeclineStakingReward());
}

//-----
TEST_F(ContractCreateTransactionTest, GetSetDeclineRewardFrozen)
{
  // Given
  ContractCreateTransaction transaction = ContractCreateTransaction()
                                            .setNodeAccountIds({ AccountId(1ULL) })
                                            .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setDeclineStakingReward(getTestDeclineStakingReward()), IllegalStateException);
}

//-----
TEST_F(ContractCreateTransactionTest, ResetFileIdWhenSettingInitCode)
{
  // Given
  ContractCreateTransaction transaction;
  ASSERT_NO_THROW(transaction.setBytecodeFileId(getTestFileId()));

  // When
  EXPECT_NO_THROW(transaction.setBytecode(getTestBytecode()));

  // Then
  EXPECT_FALSE(transaction.getFileId().has_value());
}

//-----
TEST_F(ContractCreateTransactionTest, ResetInitCodeWhenSettingFileId)
{
  // Given
  ContractCreateTransaction transaction;
  ASSERT_NO_THROW(transaction.setBytecode(getTestBytecode()));

  // When
  EXPECT_NO_THROW(transaction.setBytecodeFileId(getTestFileId()));

  // Then
  EXPECT_FALSE(transaction.getInitCode().has_value());
}

//-----
TEST_F(ContractCreateTransactionTest, ResetStakedAccountIdWhenSettingStakedNodeId)
{
  // Given
  ContractCreateTransaction transaction;
  ASSERT_NO_THROW(transaction.setStakedAccountId(getTestStakedAccountId()));

  // When
  EXPECT_NO_THROW(transaction.setStakedNodeId(getTestStakedNodeId()));

  // Then
  EXPECT_FALSE(transaction.getStakedAccountId().has_value());
}

//-----
TEST_F(ContractCreateTransactionTest, ResetStakedNodeIdWhenSettingStakedAccountId)
{
  // Given
  ContractCreateTransaction transaction;
  ASSERT_NO_THROW(transaction.setStakedNodeId(getTestStakedNodeId()));

  // When
  EXPECT_NO_THROW(transaction.setStakedAccountId(getTestStakedAccountId()));

  // Then
  EXPECT_FALSE(transaction.getStakedNodeId().has_value());
}
