/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "AccountId.h"
#include "PendingAirdropId.h"
#include "TokenClaimAirdropTransaction.h"
#include "TokenId.h"

#include <gtest/gtest.h>
#include <token_claim_airdrop.pb.h>
#include <transaction_body.pb.h>

using namespace Hedera;

class TokenClaimAirdropTransactionUnitTests : public ::testing::Test
{
protected:
  TokenClaimAirdropTransaction tokenClaimAirdropTransaction;
};

//-----
TEST_F(TokenClaimAirdropTransactionUnitTests, ConstructFromTransactionBody)
{
  // Given
  proto::TransactionBody transactionBody;
  proto::TokenClaimAirdropTransactionBody* claimAirdropBody = transactionBody.mutable_tokenclaimairdrop();

  proto::PendingAirdropId* pendingAirdrop = claimAirdropBody->add_pending_airdrops();
  pendingAirdrop->mutable_sender_id()->set_shardnum(1);
  pendingAirdrop->mutable_sender_id()->set_realmnum(2);
  pendingAirdrop->mutable_sender_id()->set_accountnum(3);
  pendingAirdrop->mutable_receiver_id()->set_shardnum(4);
  pendingAirdrop->mutable_receiver_id()->set_realmnum(5);
  pendingAirdrop->mutable_receiver_id()->set_accountnum(6);

  // When
  TokenClaimAirdropTransaction transaction(transactionBody);

  // Then
  ASSERT_EQ(transaction.getPendingAirdrops().size(), 1);
  EXPECT_EQ(transaction.getPendingAirdrops()[0].mSender, AccountId(1, 2, 3));
  EXPECT_EQ(transaction.getPendingAirdrops()[0].mReceiver, AccountId(4, 5, 6));
}

//-----
TEST_F(TokenClaimAirdropTransactionUnitTests, SetPendingAirdrops)
{
  // Given
  std::vector<PendingAirdropId> pendingAirdrops;
  pendingAirdrops.emplace_back(AccountId(1, 2, 3), AccountId(4, 5, 6), TokenId(7, 8, 9));

  // When
  tokenClaimAirdropTransaction.setPendingAirdrops(pendingAirdrops);

  // Then
  ASSERT_EQ(tokenClaimAirdropTransaction.getPendingAirdrops().size(), 1);
  EXPECT_EQ(tokenClaimAirdropTransaction.getPendingAirdrops()[0].mSender, AccountId(1, 2, 3));
  EXPECT_EQ(tokenClaimAirdropTransaction.getPendingAirdrops()[0].mReceiver, AccountId(4, 5, 6));
  EXPECT_EQ(tokenClaimAirdropTransaction.getPendingAirdrops()[0].mFt.value(), TokenId(7, 8, 9));
}

//-----
TEST_F(TokenClaimAirdropTransactionUnitTests, InitFromSourceTransactionBody)
{
  // Given
  proto::TransactionBody transactionBody;
  proto::TokenClaimAirdropTransactionBody* claimAirdropBody = transactionBody.mutable_tokenclaimairdrop();
  proto::PendingAirdropId* pendingAirdrop = claimAirdropBody->add_pending_airdrops();

  pendingAirdrop->mutable_sender_id()->set_shardnum(1);
  pendingAirdrop->mutable_sender_id()->set_realmnum(2);
  pendingAirdrop->mutable_sender_id()->set_accountnum(3);
  pendingAirdrop->mutable_receiver_id()->set_shardnum(4);
  pendingAirdrop->mutable_receiver_id()->set_realmnum(5);
  pendingAirdrop->mutable_receiver_id()->set_accountnum(6);

  // When
  TokenClaimAirdropTransaction transaction(transactionBody);

  // Then
  ASSERT_EQ(transaction.getPendingAirdrops().size(), 1);
  EXPECT_EQ(transaction.getPendingAirdrops()[0].mSender, AccountId(1, 2, 3));
  EXPECT_EQ(transaction.getPendingAirdrops()[0].mReceiver, AccountId(4, 5, 6));
}
