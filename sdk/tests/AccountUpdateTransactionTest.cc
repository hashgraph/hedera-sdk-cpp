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
#include "AccountUpdateTransaction.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "exceptions/IllegalStateException.h"
#include "impl/DurationConverter.h"
#include "impl/TimestampConverter.h"

#include <gtest/gtest.h>
#include <limits>
#include <proto/crypto_update.pb.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class AccountUpdateTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(getTestAccountId(), ECDSAsecp256k1PrivateKey::generatePrivateKey()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKey() const { return mPublicKey; }
  [[nodiscard]] inline bool getTestReceiverSignatureRequired() const { return mReceiverSignatureRequired; }
  [[nodiscard]] inline const std::chrono::duration<double>& getTestAutoRenewPeriod() const { return mAutoRenewPeriod; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestExpirationTime() const
  {
    return mExpirationTime;
  }
  [[nodiscard]] inline const std::string& getTestAccountMemo() const { return mAccountMemo; }
  [[nodiscard]] inline uint32_t getTestMaximumTokenAssociations() const { return mMaxTokenAssociations; }
  [[nodiscard]] inline const AccountId& getTestStakedAccountId() const { return mStakedAccountId; }
  [[nodiscard]] inline const uint64_t& getTestStakedNodeId() const { return mStakedNodeId; }
  [[nodiscard]] inline bool getTestDeclineStakingReward() const { return mDeclineStakingReward; }

private:
  Client mClient;
  const AccountId mAccountId = AccountId(1ULL);
  const std::shared_ptr<PublicKey> mPublicKey = ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey();
  const bool mReceiverSignatureRequired = true;
  const std::chrono::duration<double> mAutoRenewPeriod = std::chrono::hours(2);
  const std::chrono::system_clock::time_point mExpirationTime = std::chrono::system_clock::now();
  const std::string mAccountMemo = "test account memo";
  const uint32_t mMaxTokenAssociations = 3U;
  const AccountId mStakedAccountId = AccountId(4ULL);
  const uint64_t mStakedNodeId = 5ULL;
  const bool mDeclineStakingReward = true;
};

//-----
TEST_F(AccountUpdateTransactionTest, ConstructAccountUpdateTransaction)
{
  AccountUpdateTransaction transaction;
  EXPECT_EQ(transaction.getAccountId(), AccountId());
  EXPECT_EQ(transaction.getKey(), nullptr);
  EXPECT_FALSE(transaction.getReceiverSignatureRequired().has_value());
  EXPECT_FALSE(transaction.getAutoRenewPeriod().has_value());
  EXPECT_FALSE(transaction.getExpirationTime().has_value());
  EXPECT_FALSE(transaction.getAccountMemo().has_value());
  EXPECT_FALSE(transaction.getMaxAutomaticTokenAssociations().has_value());
  EXPECT_FALSE(transaction.getStakedAccountId().has_value());
  EXPECT_FALSE(transaction.getStakedNodeId().has_value());
  EXPECT_FALSE(transaction.getDeclineStakingReward().has_value());
}

//-----
TEST_F(AccountUpdateTransactionTest, ConstructAccountUpdateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::CryptoUpdateTransactionBody>();
  body->set_allocated_accountidtoupdate(getTestAccountId().toProtobuf().release());
  body->set_allocated_key(getTestPublicKey()->toProtobuf().release());
  body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(getTestAutoRenewPeriod()));
  body->set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(getTestExpirationTime()));
  body->set_allocated_staked_account_id(getTestAccountId().toProtobuf().release());

  auto boolValue = std::make_unique<google::protobuf::BoolValue>();
  boolValue->set_value(getTestReceiverSignatureRequired());
  body->set_allocated_receiversigrequiredwrapper(boolValue.release());

  boolValue = std::make_unique<google::protobuf::BoolValue>();
  boolValue->set_value(getTestDeclineStakingReward());
  body->set_allocated_decline_reward(boolValue.release());

  auto stringValue = std::make_unique<google::protobuf::StringValue>();
  stringValue->set_value(getTestAccountMemo());
  body->set_allocated_memo(stringValue.release());

  auto int32Value = std::make_unique<google::protobuf::Int32Value>();
  int32Value->set_value(static_cast<int32_t>(getTestMaximumTokenAssociations()));
  body->set_allocated_max_automatic_token_associations(int32Value.release());

  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoupdateaccount(body.release());

  // When
  AccountUpdateTransaction accountUpdateTransaction(txBody);

  // Then
  EXPECT_EQ(accountUpdateTransaction.getAccountId(), getTestAccountId());
  EXPECT_EQ(accountUpdateTransaction.getKey()->toString(), getTestPublicKey()->toString());
  ASSERT_TRUE(accountUpdateTransaction.getReceiverSignatureRequired().has_value());
  EXPECT_EQ(accountUpdateTransaction.getReceiverSignatureRequired(), getTestReceiverSignatureRequired());
  ASSERT_TRUE(accountUpdateTransaction.getAutoRenewPeriod().has_value());
  EXPECT_EQ(accountUpdateTransaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  ASSERT_TRUE(accountUpdateTransaction.getExpirationTime().has_value());
  EXPECT_EQ(accountUpdateTransaction.getExpirationTime(), getTestExpirationTime());
  ASSERT_TRUE(accountUpdateTransaction.getAccountMemo().has_value());
  EXPECT_EQ(accountUpdateTransaction.getAccountMemo(), getTestAccountMemo());
  ASSERT_TRUE(accountUpdateTransaction.getMaxAutomaticTokenAssociations().has_value());
  EXPECT_EQ(accountUpdateTransaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
  ASSERT_TRUE(accountUpdateTransaction.getStakedAccountId().has_value());
  EXPECT_EQ(accountUpdateTransaction.getStakedAccountId(), getTestAccountId());
  EXPECT_FALSE(accountUpdateTransaction.getStakedNodeId().has_value());
  ASSERT_TRUE(accountUpdateTransaction.getDeclineStakingReward().has_value());
  EXPECT_EQ(accountUpdateTransaction.getDeclineStakingReward(), getTestDeclineStakingReward());
}

//-----
TEST_F(AccountUpdateTransactionTest, SetAccountId)
{
  AccountUpdateTransaction transaction;
  transaction.setAccountId(getTestAccountId());
  EXPECT_EQ(transaction.getAccountId(), getTestAccountId());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setAccountId(getTestAccountId()), IllegalStateException);
}

//-----
TEST_F(AccountUpdateTransactionTest, SetKey)
{
  AccountUpdateTransaction transaction;
  transaction.setKey(getTestPublicKey());
  EXPECT_EQ(transaction.getKey()->toString(), getTestPublicKey()->toString());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setKey(getTestPublicKey()), IllegalStateException);
}

//-----
TEST_F(AccountUpdateTransactionTest, SetReceiverSignatureRequired)
{
  AccountUpdateTransaction transaction;
  transaction.setReceiverSignatureRequired(getTestReceiverSignatureRequired());
  EXPECT_TRUE(transaction.getReceiverSignatureRequired());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setReceiverSignatureRequired(getTestReceiverSignatureRequired()), IllegalStateException);
}

//-----
TEST_F(AccountUpdateTransactionTest, SetAutoRenewPeriod)
{
  AccountUpdateTransaction transaction;
  transaction.setAutoRenewPeriod(getTestAutoRenewPeriod());
  EXPECT_EQ(transaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setAutoRenewPeriod(getTestAutoRenewPeriod()), IllegalStateException);
}

//-----
TEST_F(AccountUpdateTransactionTest, SetExpirationTime)
{
  AccountUpdateTransaction transaction;
  transaction.setExpirationTime(getTestExpirationTime());
  EXPECT_EQ(transaction.getExpirationTime(), getTestExpirationTime());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setExpirationTime(getTestExpirationTime()), IllegalStateException);
}

//-----
TEST_F(AccountUpdateTransactionTest, SetAccountMemo)
{
  AccountUpdateTransaction transaction;
  transaction.setAccountMemo(getTestAccountMemo());
  EXPECT_EQ(transaction.getAccountMemo(), getTestAccountMemo());

  // Throw if account memo is larger than 100 characters
  EXPECT_THROW(transaction.setAccountMemo(std::string(101, 'a')), std::length_error);

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setAccountMemo(getTestAccountMemo()), IllegalStateException);
}

//-----
TEST_F(AccountUpdateTransactionTest, SetMaxAutomaticTokenAssociations)
{
  AccountUpdateTransaction transaction;
  transaction.setMaxAutomaticTokenAssociations(getTestMaximumTokenAssociations());
  EXPECT_EQ(transaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());

  // Throw if over 5000
  EXPECT_NO_THROW(transaction.setMaxAutomaticTokenAssociations(5000U));
  EXPECT_THROW(transaction.setMaxAutomaticTokenAssociations(5001U), std::invalid_argument);
  EXPECT_THROW(transaction.setMaxAutomaticTokenAssociations(std::numeric_limits<uint32_t>::max()),
               std::invalid_argument);

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setMaxAutomaticTokenAssociations(getTestMaximumTokenAssociations()), IllegalStateException);
}

//-----
TEST_F(AccountUpdateTransactionTest, SetStakedAccountId)
{
  AccountUpdateTransaction transaction;
  transaction.setStakedAccountId(getTestStakedAccountId());
  EXPECT_EQ(transaction.getStakedAccountId(), getTestStakedAccountId());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setStakedAccountId(getTestStakedAccountId()), IllegalStateException);
}

//-----
TEST_F(AccountUpdateTransactionTest, SetStakedNodeId)
{
  AccountUpdateTransaction transaction;
  transaction.setStakedNodeId(getTestStakedNodeId());
  EXPECT_EQ(transaction.getStakedNodeId(), getTestStakedNodeId());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setStakedNodeId(getTestStakedNodeId()), IllegalStateException);
}

//-----
TEST_F(AccountUpdateTransactionTest, SetStakingRewardPolicy)
{
  AccountUpdateTransaction transaction;
  transaction.setDeclineStakingReward(getTestDeclineStakingReward());
  EXPECT_EQ(transaction.getDeclineStakingReward(), getTestDeclineStakingReward());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setDeclineStakingReward(getTestDeclineStakingReward()), IllegalStateException);
}

//-----
TEST_F(AccountUpdateTransactionTest, ResetMutuallyExclusiveIds)
{
  AccountUpdateTransaction transaction;
  transaction.setStakedAccountId(getTestStakedAccountId());
  transaction.setStakedNodeId(getTestStakedNodeId());

  EXPECT_FALSE(transaction.getStakedAccountId().has_value());
  EXPECT_TRUE(transaction.getStakedNodeId().has_value());

  transaction.setStakedAccountId(getTestStakedAccountId());
  EXPECT_TRUE(transaction.getStakedAccountId().has_value());
  EXPECT_FALSE(transaction.getStakedNodeId().has_value());
}
