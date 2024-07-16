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
#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "AccountCreateTransaction.h"
#include "BaseIntegrationTest.h"
#include "ED25519PrivateKey.h"
#include "TokenCreateTransaction.h"
#include "TokenMintTransaction.h"
#include "TokenRejectTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"

#include <gtest/gtest.h>

using namespace Hedera;

class TokenRejectTransactionIntegrationTests : public BaseIntegrationTest
{
protected:
  TokenId createFt(const std::shared_ptr<PrivateKey>& operatorKey)
  {
    return TokenCreateTransaction()
      .setTokenName("ffff")
      .setTokenSymbol("F")
      .setTokenType(TokenType::FUNGIBLE_COMMON)
      .setInitialSupply(100000)
      .setTreasuryAccountId(AccountId(2ULL))
      .setAdminKey(operatorKey)
      .setFreezeKey(operatorKey)
      .setWipeKey(operatorKey)
      .setSupplyKey(operatorKey)
      .setFeeScheduleKey(operatorKey)
      .execute(getTestClient())
      .getReceipt(getTestClient())
      .mTokenId.value();
  }

  TokenId createNft(const std::shared_ptr<PrivateKey>& operatorKey)
  {
    return TokenCreateTransaction()
      .setTokenName("ffff")
      .setTokenSymbol("F")
      .setTokenType(TokenType::NON_FUNGIBLE_UNIQUE)
      .setTreasuryAccountId(AccountId(2ULL))
      .setAdminKey(operatorKey)
      .setFreezeKey(operatorKey)
      .setWipeKey(operatorKey)
      .setSupplyKey(operatorKey)
      .setFeeScheduleKey(operatorKey)
      .execute(getTestClient())
      .getReceipt(getTestClient())
      .mTokenId.value();
  }

  AccountId createAccount(const std::shared_ptr<PrivateKey>& accountKey)
  {
    return AccountCreateTransaction()
      .setKey(accountKey)
      .setInitialBalance(Hbar(5))
      .setMaxAutomaticTokenAssociations(1000)
      .execute(getTestClient())
      .getReceipt(getTestClient())
      .mAccountId.value();
  }

private:
};

//-----
TEST_F(TokenRejectTransactionIntegrationTests, CanExecuteForFT)
{
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  // Create two fts
  TokenId tokenId1;
  ASSERT_NO_THROW(tokenId1 = createFt(operatorKey));
  TokenId tokenId2;
  ASSERT_NO_THROW(tokenId2 = createFt(operatorKey));

  // Create receiver account with auto associations
  AccountId receiver;
  std::shared_ptr<PrivateKey> receiverKey = ED25519PrivateKey::generatePrivateKey();
  ASSERT_NO_THROW(receiver = createAccount(receiverKey));

  // Transfer fts to the receiver
  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = TransferTransaction()
                                .addTokenTransfer(tokenId1, getTestClient().getOperatorAccountId().value(), -10)
                                .addTokenTransfer(tokenId1, receiver, 10)
                                .addTokenTransfer(tokenId2, getTestClient().getOperatorAccountId().value(), -10)
                                .addTokenTransfer(tokenId2, receiver, 10)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Reject the tokens
  EXPECT_NO_THROW(txReceipt = TokenRejectTransaction()
                                .setOwner(receiver)
                                .setFts({ tokenId1, tokenId2 })
                                .freezeWith(&getTestClient())
                                .sign(receiverKey)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Verify the balance of the receiver is 0
  AccountBalance balance;
  EXPECT_NO_THROW(balance = AccountBalanceQuery().setAccountId(receiver).execute(getTestClient()));

  EXPECT_EQ(0, balance.mTokens[tokenId1]);
  EXPECT_EQ(0, balance.mTokens[tokenId2]);

  // Verify the tokens are transferred back to the treasury
  EXPECT_NO_THROW(
    balance =
      AccountBalanceQuery().setAccountId(getTestClient().getOperatorAccountId().value()).execute(getTestClient()));

  EXPECT_EQ(100000, balance.mTokens[tokenId1]);
  EXPECT_EQ(100000, balance.mTokens[tokenId2]);
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, CanExecuteForNFT)
{
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  // Create two nfts
  TokenId tokenId1;
  ASSERT_NO_THROW(tokenId1 = createNft(operatorKey));
  TokenId tokenId2;
  ASSERT_NO_THROW(tokenId2 = createNft(operatorKey));

  // Mint the nfts
  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = TokenMintTransaction()
                                .setTokenId(tokenId1)
                                .setMetadata({ { std::byte(0xAB) } })
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  ASSERT_NO_THROW(txReceipt = TokenMintTransaction()
                                .setTokenId(tokenId2)
                                .setMetadata({ { std::byte(0xAB) } })
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Create receiver account with auto associations
  AccountId receiver;
  std::shared_ptr<PrivateKey> receiverKey = ED25519PrivateKey::generatePrivateKey();
  ASSERT_NO_THROW(receiver = createAccount(receiverKey));

  ASSERT_NO_THROW(txReceipt =
                    TransferTransaction()
                      .addNftTransfer(tokenId1.nft(1), getTestClient().getOperatorAccountId().value(), receiver)
                      .addNftTransfer(tokenId2.nft(1), getTestClient().getOperatorAccountId().value(), receiver)
                      .execute(getTestClient())
                      .getReceipt(getTestClient()));

  // Reject the nfts
  EXPECT_NO_THROW(txReceipt = TokenRejectTransaction()
                                .setOwner(receiver)
                                .setNfts({ tokenId1.nft(1), tokenId2.nft(1) })
                                .freezeWith(&getTestClient())
                                .sign(receiverKey)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Verify the balance of the receiver is 0
  AccountBalance balance;
  EXPECT_NO_THROW(balance = AccountBalanceQuery().setAccountId(receiver).execute(getTestClient()));

  EXPECT_EQ(0, balance.mTokens[tokenId1]);
  EXPECT_EQ(0, balance.mTokens[tokenId2]);

  // Verify the tokens are transferred back to the treasury
  EXPECT_NO_THROW(
    balance =
      AccountBalanceQuery().setAccountId(getTestClient().getOperatorAccountId().value()).execute(getTestClient()));

  EXPECT_EQ(1, balance.mTokens[tokenId1]);
  EXPECT_EQ(1, balance.mTokens[tokenId2]);
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, CanExecuteForFTAndNFT)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, ReceiverSigRequired)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, TokenFrozen)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, TokenPaused)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, RemovesAllowance)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsWhenRejectingNFTWithTokenID)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsWithTokenReferenceRepeated)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsWhenOwnerHasNoBalance)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsTreasuryRejects)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsWithInvalidToken)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsWithReferenceSizeExceeded)
{
  // Given
  // When
  // Then
}

//-----
TEST_F(TokenRejectTransactionIntegrationTests, FailsWithInvalidSignature)
{
  // Given
  // When
  // Then
}
