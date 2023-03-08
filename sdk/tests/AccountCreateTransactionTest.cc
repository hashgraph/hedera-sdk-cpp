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
#include "AccountCreateTransaction.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ED25519PrivateKey.h"
#include "Hbar.h"
#include "PublicKey.h"
#include "exceptions/IllegalStateException.h"
#include "impl/DurationConverter.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>

using namespace Hedera;

class AccountCreateTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(getTestAccountId(), ECDSAsecp256k1PrivateKey::generatePrivateKey()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKey() const { return mPublicKey; }
  [[nodiscard]] inline const Hbar& getTestInitialBalance() const { return mInitialBalance; }
  [[nodiscard]] inline bool getTestReceiverSignatureRequired() const { return mReceiverSignatureRequired; }
  [[nodiscard]] inline const std::chrono::duration<double>& getTestAutoRenewPeriod() const { return mAutoRenewPeriod; }
  [[nodiscard]] inline const std::string& getTestAccountMemo() const { return mAccountMemo; }
  [[nodiscard]] inline uint32_t getTestMaximumTokenAssociations() const { return mMaxTokenAssociations; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const uint64_t& getTestNodeId() const { return mNodeId; }
  [[nodiscard]] inline bool getTestDeclineStakingReward() const { return mDeclineStakingReward; }
  [[nodiscard]] inline const EvmAddress& getTestEvmAddress() const { return mEvmAddress; }

private:
  Client mClient;
  const std::shared_ptr<PublicKey> mPublicKey = ED25519PrivateKey::generatePrivateKey()->getPublicKey();
  const Hbar mInitialBalance = Hbar(1LL);
  const bool mReceiverSignatureRequired = true;
  const std::chrono::duration<double> mAutoRenewPeriod = std::chrono::hours(2);
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
  AccountCreateTransaction transaction;
  EXPECT_EQ(transaction.getKey(), nullptr);
  EXPECT_EQ(transaction.getInitialBalance(), Hbar(0ULL));
  EXPECT_FALSE(transaction.getReceiverSignatureRequired());
  EXPECT_EQ(transaction.getAutoRenewPeriod(), std::chrono::hours(2160)); // 90 days
  EXPECT_EQ(transaction.getTransactionMemo(), std::string());
  EXPECT_EQ(transaction.getMaxAutomaticTokenAssociations(), 0U);
  EXPECT_FALSE(transaction.getStakedAccountId().has_value());
  EXPECT_FALSE(transaction.getStakedNodeId().has_value());
  EXPECT_FALSE(transaction.getDeclineStakingReward());
  EXPECT_EQ(transaction.getAlias(), nullptr);
  EXPECT_FALSE(transaction.getEvmAddress().has_value());
}

//-----
TEST_F(AccountCreateTransactionTest, ConstructAccountCreateTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::CryptoCreateTransactionBody>();
  body->set_allocated_key(getTestPublicKey()->toProtobuf().release());
  body->set_initialbalance(static_cast<uint64_t>(getTestInitialBalance().toTinybars()));
  body->set_receiversigrequired(getTestReceiverSignatureRequired());
  body->set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(getTestAutoRenewPeriod()));
  body->set_allocated_memo(new std::string{ getTestAccountMemo().cbegin(), getTestAccountMemo().cend() });
  body->set_max_automatic_token_associations(static_cast<int32_t>(getTestMaximumTokenAssociations()));
  body->set_allocated_staked_account_id(getTestAccountId().toProtobuf().release());
  body->set_decline_reward(getTestDeclineStakingReward());

  const std::vector<unsigned char> testPublicKeyBytes = getTestPublicKey()->toBytes();
  const std::vector<unsigned char> testEvmAddressBytes = getTestEvmAddress().toBytes();
  body->set_allocated_alias(new std::string{ testPublicKeyBytes.cbegin(), testPublicKeyBytes.cend() });
  body->set_allocated_evm_address(new std::string{ testEvmAddressBytes.cbegin(), testEvmAddressBytes.cend() });

  proto::TransactionBody txBody;
  txBody.set_allocated_cryptocreateaccount(body.release());

  // When
  AccountCreateTransaction accountCreateTransaction(txBody);

  // Then
  EXPECT_EQ(accountCreateTransaction.getKey()->toString(), getTestPublicKey()->toString());
  EXPECT_EQ(accountCreateTransaction.getInitialBalance(), getTestInitialBalance());
  EXPECT_EQ(accountCreateTransaction.getReceiverSignatureRequired(), getTestReceiverSignatureRequired());
  EXPECT_EQ(accountCreateTransaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  EXPECT_EQ(accountCreateTransaction.getAccountMemo(), getTestAccountMemo());
  EXPECT_EQ(accountCreateTransaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
  ASSERT_TRUE(accountCreateTransaction.getStakedAccountId().has_value());
  EXPECT_EQ(accountCreateTransaction.getStakedAccountId(), getTestAccountId());
  EXPECT_FALSE(accountCreateTransaction.getStakedNodeId().has_value());
  EXPECT_EQ(accountCreateTransaction.getDeclineStakingReward(), getTestDeclineStakingReward());
  EXPECT_EQ(accountCreateTransaction.getAlias()->toBytes(), testPublicKeyBytes);
  ASSERT_TRUE(accountCreateTransaction.getEvmAddress().has_value());
  EXPECT_EQ(accountCreateTransaction.getEvmAddress()->toBytes(), testEvmAddressBytes);
}

//-----
TEST_F(AccountCreateTransactionTest, SetKey)
{
  AccountCreateTransaction transaction;
  transaction.setKey(getTestPublicKey());
  EXPECT_EQ(transaction.getKey()->toString(), getTestPublicKey()->toString());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setKey(getTestPublicKey()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetInitialBalance)
{
  AccountCreateTransaction transaction;
  transaction.setInitialBalance(getTestInitialBalance());
  EXPECT_EQ(transaction.getInitialBalance(), getTestInitialBalance());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setKey(getTestPublicKey()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetReceiverSignatureRequired)
{
  AccountCreateTransaction transaction;
  transaction.setReceiverSignatureRequired(getTestReceiverSignatureRequired());
  EXPECT_TRUE(transaction.getReceiverSignatureRequired());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setReceiverSignatureRequired(getTestReceiverSignatureRequired()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetAutoRenewPeriod)
{
  AccountCreateTransaction transaction;
  transaction.setAutoRenewPeriod(getTestAutoRenewPeriod());
  EXPECT_EQ(transaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setAutoRenewPeriod(getTestAutoRenewPeriod()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetAccountMemo)
{
  AccountCreateTransaction transaction;
  transaction.setAccountMemo(getTestAccountMemo());
  EXPECT_EQ(transaction.getAccountMemo(), getTestAccountMemo());

  // Throw if account memo is larger than 100 characters
  EXPECT_THROW(transaction.setAccountMemo(std::string(101, 'a')), std::length_error);

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setAccountMemo(getTestAccountMemo()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetMaxAutomaticTokenAssociations)
{
  AccountCreateTransaction transaction;
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
TEST_F(AccountCreateTransactionTest, SetStakedAccountId)
{
  AccountCreateTransaction transaction;
  transaction.setStakedAccountId(getTestAccountId());
  EXPECT_EQ(*transaction.getStakedAccountId(), getTestAccountId());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setStakedAccountId(getTestAccountId()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetStakedNodeId)
{
  AccountCreateTransaction transaction;
  transaction.setStakedNodeId(getTestNodeId());
  EXPECT_EQ(*transaction.getStakedNodeId(), getTestNodeId());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setStakedNodeId(getTestNodeId()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetStakingRewardPolicy)
{
  AccountCreateTransaction transaction;
  transaction.setDeclineStakingReward(true);
  EXPECT_TRUE(transaction.getDeclineStakingReward());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setDeclineStakingReward(true), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetAlias)
{
  AccountCreateTransaction transaction;
  transaction.setAlias(getTestPublicKey());
  EXPECT_EQ(transaction.getAlias()->toString(), getTestPublicKey()->toString());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setAlias(getTestPublicKey()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, SetEvmAddress)
{
  AccountCreateTransaction transaction;
  transaction.setEvmAddress(getTestEvmAddress());
  ASSERT_TRUE(transaction.getEvmAddress().has_value());
  EXPECT_EQ(transaction.getEvmAddress()->toString(), getTestEvmAddress().toString());

  transaction.freezeWith(getTestClient());
  EXPECT_THROW(transaction.setEvmAddress(getTestEvmAddress()), IllegalStateException);
}

//-----
TEST_F(AccountCreateTransactionTest, ResetMutuallyExclusiveIds)
{
  AccountCreateTransaction transaction;
  transaction.setStakedAccountId(getTestAccountId());
  transaction.setStakedNodeId(getTestNodeId());

  EXPECT_FALSE(transaction.getStakedAccountId().has_value());
  EXPECT_TRUE(transaction.getStakedNodeId().has_value());

  transaction.setStakedAccountId(getTestAccountId());
  EXPECT_TRUE(transaction.getStakedAccountId().has_value());
  EXPECT_FALSE(transaction.getStakedNodeId().has_value());
}
