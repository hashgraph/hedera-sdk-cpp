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
#include "AccountCreateTransaction.h"
#include "Client.h"
#include "Defaults.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "PublicKey.h"
#include "exceptions/IllegalStateException.h"
#include "impl/DurationConverter.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class AccountCreateTransactionTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKey() const { return mPublicKey; }
  [[nodiscard]] inline const Hbar& getTestInitialBalance() const { return mInitialBalance; }
  [[nodiscard]] inline bool getTestReceiverSignatureRequired() const { return mReceiverSignatureRequired; }
  [[nodiscard]] inline const std::chrono::system_clock::duration& getTestAutoRenewPeriod() const
  {
    return mAutoRenewPeriod;
  }
  [[nodiscard]] inline const std::string& getTestAccountMemo() const { return mAccountMemo; }
  [[nodiscard]] inline uint32_t getTestMaximumTokenAssociations() const { return mMaxTokenAssociations; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const uint64_t& getTestNodeId() const { return mNodeId; }
  [[nodiscard]] inline bool getTestDeclineStakingReward() const { return mDeclineStakingReward; }
  [[nodiscard]] inline const EvmAddress& getTestEvmAddress() const { return mEvmAddress; }

private:
  const std::shared_ptr<PublicKey> mPublicKey = ED25519PrivateKey::generatePrivateKey()->getPublicKey();
  const Hbar mInitialBalance = Hbar(1LL);
  const bool mReceiverSignatureRequired = true;
  const std::chrono::system_clock::duration mAutoRenewPeriod = std::chrono::hours(2);
  const std::string mAccountMemo = "test account memo";
  const uint32_t mMaxTokenAssociations = 3U;
  const AccountId mAccountId = AccountId(4ULL);
  const uint64_t mNodeId = 5ULL;
  const bool mDeclineStakingReward = true;
  const EvmAddress mEvmAddress = EvmAddress::fromString("303132333435363738396162636465666768696a");
};

//-----
TEST_F(AccountCreateTransactionTest, ConstructAccountCreateTransaction)
{
  // Given / When
  AccountCreateTransaction transaction;

  // Then
  EXPECT_EQ(transaction.getKey(), nullptr);
  EXPECT_EQ(transaction.getInitialBalance(), Hbar(0ULL));
  EXPECT_FALSE(transaction.getReceiverSignatureRequired());
  EXPECT_EQ(transaction.getAutoRenewPeriod(), DEFAULT_AUTO_RENEW_PERIOD);
  EXPECT_EQ(transaction.getTransactionMemo(), std::string());
  EXPECT_EQ(transaction.getMaxAutomaticTokenAssociations(), 0U);
  EXPECT_FALSE(transaction.getStakedAccountId().has_value());
  EXPECT_FALSE(transaction.getStakedNodeId().has_value());
  EXPECT_FALSE(transaction.getDeclineStakingReward());
  EXPECT_FALSE(transaction.getAlias().has_value());
}

//-----
TEST_F(AccountCreateTransactionTest, ConstructAccountCreateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::CryptoCreateTransactionBody>();
  body->set_allocated_key(getTestPublicKey()->toProtobufKey().release());
  body->set_initialbalance(static_cast<uint64_t>(getTestInitialBalance().toTinybars()));
  body->set_receiversigrequired(getTestReceiverSignatureRequired());
  body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(getTestAutoRenewPeriod()));
  body->set_memo(getTestAccountMemo());
  body->set_max_automatic_token_associations(static_cast<int32_t>(getTestMaximumTokenAssociations()));
  body->set_allocated_staked_account_id(getTestAccountId().toProtobuf().release());
  body->set_decline_reward(getTestDeclineStakingReward());

  const std::vector<std::byte> testAliasBytes = getTestEvmAddress().toBytes();
  body->set_alias(internal::Utilities::byteVectorToString(testAliasBytes));

  proto::TransactionBody txBody;
  txBody.set_allocated_cryptocreateaccount(body.release());

  // When
  const AccountCreateTransaction accountCreateTransaction(txBody);

  // Then
  EXPECT_EQ(accountCreateTransaction.getKey()->toBytes(), getTestPublicKey()->toBytes());
  EXPECT_EQ(accountCreateTransaction.getInitialBalance(), getTestInitialBalance());
  EXPECT_EQ(accountCreateTransaction.getReceiverSignatureRequired(), getTestReceiverSignatureRequired());
  EXPECT_EQ(accountCreateTransaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  EXPECT_EQ(accountCreateTransaction.getAccountMemo(), getTestAccountMemo());
  EXPECT_EQ(accountCreateTransaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
  ASSERT_TRUE(accountCreateTransaction.getStakedAccountId().has_value());
  EXPECT_EQ(accountCreateTransaction.getStakedAccountId(), getTestAccountId());
  EXPECT_FALSE(accountCreateTransaction.getStakedNodeId().has_value());
  EXPECT_EQ(accountCreateTransaction.getDeclineStakingReward(), getTestDeclineStakingReward());
  ASSERT_TRUE(accountCreateTransaction.getAlias().has_value());
  EXPECT_EQ(accountCreateTransaction.getAlias()->toBytes(), testAliasBytes);
}

//-----
TEST_F(AccountCreateTransactionTest, ConstructAccountCreateTransactionFromWrongTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::CryptoDeleteTransactionBody>();
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodelete(body.release());

  // When / Then
  EXPECT_THROW(const AccountCreateTransaction accountCreateTransaction(txBody), std::invalid_argument);
}

//-----
TEST_F(AccountCreateTransactionTest, SetKey)
{
  // Given
  AccountCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setKey(getTestPublicKey()));

  // Then
  EXPECT_EQ(transaction.getKey()->toBytes(), getTestPublicKey()->toBytes());
}

//-----
TEST_F(AccountCreateTransactionTest, SetKeyFrozen)
{
  // Given
  AccountCreateTransaction transaction = AccountCreateTransaction()
                                           .setNodeAccountIds({ AccountId(1ULL) })
                                           .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setKey(getTestPublicKey()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetInitialBalance)
{
  // Given
  AccountCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setInitialBalance(getTestInitialBalance()));

  // Then
  EXPECT_EQ(transaction.getInitialBalance(), getTestInitialBalance());
}

//-----
TEST_F(AccountCreateTransactionTest, SetInitialBalanceFrozen)
{
  // Given
  AccountCreateTransaction transaction = AccountCreateTransaction()
                                           .setNodeAccountIds({ AccountId(1ULL) })
                                           .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setInitialBalance(getTestInitialBalance()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetReceiverSignatureRequired)
{
  // Given
  AccountCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setReceiverSignatureRequired(getTestReceiverSignatureRequired()));

  // Then
  EXPECT_TRUE(transaction.getReceiverSignatureRequired());
}

//-----
TEST_F(AccountCreateTransactionTest, SetReceiverSignatureRequiredFrozen)
{
  // Given
  AccountCreateTransaction transaction = AccountCreateTransaction()
                                           .setNodeAccountIds({ AccountId(1ULL) })
                                           .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setReceiverSignatureRequired(getTestReceiverSignatureRequired()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetAutoRenewPeriod)
{
  // Given
  AccountCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAutoRenewPeriod(getTestAutoRenewPeriod()));

  // Then
  EXPECT_EQ(transaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
}

//-----
TEST_F(AccountCreateTransactionTest, SetAutoRenewPeriodFrozen)
{
  // Given
  AccountCreateTransaction transaction = AccountCreateTransaction()
                                           .setNodeAccountIds({ AccountId(1ULL) })
                                           .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAutoRenewPeriod(getTestAutoRenewPeriod()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetAccountMemo)
{
  // Given
  AccountCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAccountMemo(getTestAccountMemo()));

  // Then
  EXPECT_EQ(transaction.getAccountMemo(), getTestAccountMemo());
}

//-----
TEST_F(AccountCreateTransactionTest, SetAccountMemoTooLarge)
{
  // Given
  AccountCreateTransaction transaction;

  // When / Then
  EXPECT_THROW(transaction.setAccountMemo(std::string(101, 'a')), std::length_error);
}

//-----
TEST_F(AccountCreateTransactionTest, SetAccountMemoFrozen)
{
  // Given
  AccountCreateTransaction transaction = AccountCreateTransaction()
                                           .setNodeAccountIds({ AccountId(1ULL) })
                                           .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setAccountMemo(getTestAccountMemo()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetMaxAutomaticTokenAssociations)
{
  // Given
  AccountCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setMaxAutomaticTokenAssociations(getTestMaximumTokenAssociations()));

  // Then
  EXPECT_EQ(transaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
}

//-----
TEST_F(AccountCreateTransactionTest, SetMaxAutomaticTokenAssociationsTooMany)
{
  // Given
  AccountCreateTransaction transaction;

  // When / Then
  EXPECT_THROW(transaction.setMaxAutomaticTokenAssociations(5001U), std::invalid_argument);
}

//-----
TEST_F(AccountCreateTransactionTest, SetMaxAutomaticTokenAssociationsFrozen)
{
  // Given
  AccountCreateTransaction transaction = AccountCreateTransaction()
                                           .setNodeAccountIds({ AccountId(1ULL) })
                                           .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setMaxAutomaticTokenAssociations(getTestMaximumTokenAssociations()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetStakedAccountId)
{
  // Given
  AccountCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setStakedAccountId(getTestAccountId()));

  // Then
  EXPECT_EQ(transaction.getStakedAccountId(), getTestAccountId());
}

//-----
TEST_F(AccountCreateTransactionTest, SetStakedAccountIdFrozen)
{
  // Given
  AccountCreateTransaction transaction = AccountCreateTransaction()
                                           .setNodeAccountIds({ AccountId(1ULL) })
                                           .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setStakedAccountId(getTestAccountId()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetStakedNodeId)
{
  // Given
  AccountCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setStakedNodeId(getTestNodeId()));

  // Then
  EXPECT_EQ(transaction.getStakedNodeId(), getTestNodeId());
}

//-----
TEST_F(AccountCreateTransactionTest, SetStakedNodeIdFrozen)
{
  // Given
  AccountCreateTransaction transaction = AccountCreateTransaction()
                                           .setNodeAccountIds({ AccountId(1ULL) })
                                           .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setStakedNodeId(getTestNodeId()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetStakingRewardPolicy)
{
  // Given
  AccountCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setDeclineStakingReward(getTestDeclineStakingReward()));

  // Then
  EXPECT_TRUE(transaction.getDeclineStakingReward());
}

//-----
TEST_F(AccountCreateTransactionTest, SetStakingRewardPolicyFrozen)
{
  // Given
  AccountCreateTransaction transaction = AccountCreateTransaction()
                                           .setNodeAccountIds({ AccountId(1ULL) })
                                           .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When / Then
  EXPECT_THROW(transaction.setDeclineStakingReward(getTestDeclineStakingReward()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetAlias)
{
  // Given
  AccountCreateTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAlias(getTestEvmAddress()));

  // Then
  ASSERT_TRUE(transaction.getAlias().has_value());
  EXPECT_EQ(transaction.getAlias()->toBytes(), getTestEvmAddress().toBytes());
}

//-----
TEST_F(AccountCreateTransactionTest, SetAliasFrozen)
{
  // Given
  AccountCreateTransaction transaction = AccountCreateTransaction()
                                           .setNodeAccountIds({ AccountId(1ULL) })
                                           .setTransactionId(TransactionId::generate(AccountId(1ULL)));
  ASSERT_NO_THROW(transaction.freeze());

  // When
  EXPECT_THROW(transaction.setAlias(getTestEvmAddress()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, ResetStakedAccountId)
{
  // Given
  AccountCreateTransaction transaction;
  ASSERT_NO_THROW(transaction.setStakedAccountId(getTestAccountId()));

  // When
  EXPECT_NO_THROW(transaction.setStakedNodeId(getTestNodeId()));

  // Then
  EXPECT_FALSE(transaction.getStakedAccountId().has_value());
  EXPECT_TRUE(transaction.getStakedNodeId().has_value());
}

//-----
TEST_F(AccountCreateTransactionTest, ResetStakedNodeId)
{
  // Given
  AccountCreateTransaction transaction;
  ASSERT_NO_THROW(transaction.setStakedNodeId(getTestNodeId()));

  // When
  EXPECT_NO_THROW(transaction.setStakedAccountId(getTestAccountId()));

  // Then
  EXPECT_TRUE(transaction.getStakedAccountId().has_value());
  EXPECT_FALSE(transaction.getStakedNodeId().has_value());
}
