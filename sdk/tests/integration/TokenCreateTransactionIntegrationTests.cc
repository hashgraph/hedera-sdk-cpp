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
#include "AccountDeleteTransaction.h"
#include "AccountId.h"
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "CustomFixedFee.h"
#include "CustomFractionalFee.h"
#include "CustomRoyaltyFee.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "TokenCreateTransaction.h"
#include "TokenDeleteTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "exceptions/ReceiptStatusException.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>

using namespace Hedera;

class TokenCreateTransactionIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(TokenCreateTransactionIntegrationTest, ExecuteTokenCreateTransaction)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = std::shared_ptr<PrivateKey>(
      ED25519PrivateKey::fromString(
        "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137")
        .release()));

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = TokenCreateTransaction()
                                .setTokenName("ffff")
                                .setTokenSymbol("F")
                                .setDecimals(3)
                                .setInitialSupply(100000)
                                .setTreasuryAccountId(AccountId(2ULL))
                                .setAdminKey(operatorKey)
                                .setFreezeKey(operatorKey)
                                .setWipeKey(operatorKey)
                                .setKycKey(operatorKey)
                                .setSupplyKey(operatorKey)
                                .setFeeScheduleKey(operatorKey)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  TokenId tokenId;
  EXPECT_NO_THROW(tokenId = txReceipt.mTokenId.value());

  // Clean up
  ASSERT_NO_THROW(txReceipt =
                    TokenDeleteTransaction().setTokenId(tokenId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CanCreateTokenWithMinimalPropertiesSet)
{
  // Given / When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = TokenCreateTransaction()
                                .setTokenName("ffff")
                                .setTokenSymbol("F")
                                .setTreasuryAccountId(AccountId(2ULL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  TokenId tokenId;
  EXPECT_NO_THROW(tokenId = txReceipt.mTokenId.value());
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CannotCreateTokenWithNoName)
{
  // Given / When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = TokenCreateTransaction()
                                                      .setTokenSymbol("F")
                                                      .setTreasuryAccountId(AccountId(2ULL))
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               PrecheckStatusException); // MISSING_TOKEN_NAME
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CannotCreateTokenWithNoSymbol)
{
  // Given / When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = TokenCreateTransaction()
                                                      .setTokenName("ffff")
                                                      .setTreasuryAccountId(AccountId(2ULL))
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               PrecheckStatusException); // MISSING_TOKEN_SYMBOL
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CannotCreateTokenWithNoTreasuryAccount)
{
  // Given / When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = TokenCreateTransaction()
                                                      .setTokenName("ffff")
                                                      .setTokenSymbol("F")
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               PrecheckStatusException); // INVALID_TREASURY_ACCOUNT_FOR_TOKEN
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CannotCreateTokenWithoutTreasuryAccountSignature)
{
  // Given
  std::shared_ptr<PrivateKey> accountKey;
  ASSERT_NO_THROW(accountKey = ED25519PrivateKey::generatePrivateKey());

  AccountId accountId;
  ASSERT_NO_THROW(accountId = AccountCreateTransaction()
                                .setKey(accountKey.get())
                                .execute(getTestClient())
                                .getReceipt(getTestClient())
                                .mAccountId.value());

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = TokenCreateTransaction()
                                                      .setTokenName("ffff")
                                                      .setTokenSymbol("F")
                                                      .setTreasuryAccountId(accountId)
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_SIGNATURE

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt = AccountDeleteTransaction()
                                                         .setDeleteAccountId(accountId)
                                                         .setTransferAccountId(AccountId(2ULL))
                                                         .freezeWith(&getTestClient())
                                                         .sign(accountKey.get())
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CannotCreateTokenWithoutAdminKeySignature)
{
  // Given
  std::shared_ptr<PrivateKey> adminKey;
  ASSERT_NO_THROW(adminKey = ED25519PrivateKey::generatePrivateKey());

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = TokenCreateTransaction()
                                                      .setTokenName("ffff")
                                                      .setTokenSymbol("F")
                                                      .setTreasuryAccountId(AccountId(2ULL))
                                                      .setAdminKey(adminKey)
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_SIGNATURE
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CanCreateTokenWithCustomFees)
{
  // Given
  const std::vector<std::shared_ptr<CustomFee>> fees = {
    std::make_shared<CustomFixedFee>(CustomFixedFee().setAmount(10ULL).setFeeCollectorAccountId(AccountId(2ULL))),
    std::make_shared<CustomFractionalFee>(CustomFractionalFee()
                                            .setNumerator(1LL)
                                            .setDenominator(20LL)
                                            .setMinimumAmount(1ULL)
                                            .setMaximumAmount(10ULL)
                                            .setFeeCollectorAccountId(AccountId(2ULL)))
  };

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = TokenCreateTransaction()
                                .setTokenName("ffff")
                                .setTokenSymbol("F")
                                .setTreasuryAccountId(AccountId(2ULL))
                                .setCustomFees(fees)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  TokenId tokenId;
  EXPECT_NO_THROW(tokenId = txReceipt.mTokenId.value());
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CannotCreateTokenWithMoreThanTenCustomFees)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = std::shared_ptr<PrivateKey>(
      ED25519PrivateKey::fromString(
        "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137")
        .release()));

  const std::vector<std::shared_ptr<CustomFee>> fees(
    11, std::make_shared<CustomFixedFee>(CustomFixedFee().setAmount(10LL).setFeeCollectorAccountId(AccountId(2ULL))));

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = TokenCreateTransaction()
                                                      .setTokenName("ffff")
                                                      .setTokenSymbol("F")
                                                      .setTreasuryAccountId(AccountId(2ULL))
                                                      .setAdminKey(operatorKey)
                                                      .setCustomFees(fees)
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // CUSTOM_FEES_LIST_TOO_LONG
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CanCreateTokenWithListOfTenCustomFixedFees)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = std::shared_ptr<PrivateKey>(
      ED25519PrivateKey::fromString(
        "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137")
        .release()));

  const std::vector<std::shared_ptr<CustomFee>> fees(
    10, std::make_shared<CustomFixedFee>(CustomFixedFee().setAmount(10LL).setFeeCollectorAccountId(AccountId(2ULL))));

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = TokenCreateTransaction()
                                .setTokenName("ffff")
                                .setTokenSymbol("F")
                                .setTreasuryAccountId(AccountId(2ULL))
                                .setAdminKey(operatorKey)
                                .setCustomFees(fees)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  TokenId tokenId;
  EXPECT_NO_THROW(tokenId = txReceipt.mTokenId.value());

  // Clean up
  ASSERT_NO_THROW(txReceipt =
                    TokenDeleteTransaction().setTokenId(tokenId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CanCreateTokenWithListOfTenCustomFractionalFees)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = std::shared_ptr<PrivateKey>(
      ED25519PrivateKey::fromString(
        "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137")
        .release()));

  const std::vector<std::shared_ptr<CustomFee>> fees(
    10,
    std::make_shared<CustomFractionalFee>(CustomFractionalFee()
                                            .setNumerator(1LL)
                                            .setDenominator(20LL)
                                            .setMinimumAmount(1ULL)
                                            .setMaximumAmount(10ULL)
                                            .setFeeCollectorAccountId(AccountId(2ULL))));

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = TokenCreateTransaction()
                                .setTokenName("ffff")
                                .setTokenSymbol("F")
                                .setTreasuryAccountId(AccountId(2ULL))
                                .setAdminKey(operatorKey)
                                .setCustomFees(fees)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  TokenId tokenId;
  EXPECT_NO_THROW(tokenId = txReceipt.mTokenId.value());

  // Clean up
  ASSERT_NO_THROW(txReceipt =
                    TokenDeleteTransaction().setTokenId(tokenId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CannotCreateTokenWithCustomFractionalFeeWithHigherMaxThanMin)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = std::shared_ptr<PrivateKey>(
      ED25519PrivateKey::fromString(
        "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137")
        .release()));

  // When / Then
  EXPECT_THROW(
    const TransactionReceipt txReceipt =
      TokenCreateTransaction()
        .setTokenName("ffff")
        .setTokenSymbol("F")
        .setTreasuryAccountId(AccountId(2ULL))
        .setAdminKey(operatorKey)
        .setCustomFees({ std::make_shared<CustomFractionalFee>(CustomFractionalFee()
                                                                 .setNumerator(1LL)
                                                                 .setDenominator(3LL)
                                                                 .setMinimumAmount(3ULL)
                                                                 .setMaximumAmount(2ULL)
                                                                 .setFeeCollectorAccountId(AccountId(2ULL))) })
        .execute(getTestClient())
        .getReceipt(getTestClient()),
    ReceiptStatusException); // FRACTIONAL_FEE_MAX_AMOUNT_LESS_THAN_MIN_AMOUNT
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CannotCreateTokenWithInvalidFeeCollectorAccountId)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = std::shared_ptr<PrivateKey>(
      ED25519PrivateKey::fromString(
        "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137")
        .release()));

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt =
                 TokenCreateTransaction()
                   .setTokenName("ffff")
                   .setTokenSymbol("F")
                   .setTreasuryAccountId(AccountId(2ULL))
                   .setAdminKey(operatorKey)
                   .setCustomFees({ std::make_shared<CustomFixedFee>(CustomFixedFee().setAmount(1LL)) })
                   .execute(getTestClient())
                   .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_CUSTOM_FEE_COLLECTOR
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CannotCreateTokenWithNegativeCustomFee)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = std::shared_ptr<PrivateKey>(
      ED25519PrivateKey::fromString(
        "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137")
        .release()));

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt =
                 TokenCreateTransaction()
                   .setTokenName("ffff")
                   .setTokenSymbol("F")
                   .setTreasuryAccountId(AccountId(2ULL))
                   .setAdminKey(operatorKey)
                   .setCustomFees({ std::make_shared<CustomFixedFee>(
                     CustomFixedFee().setAmount(-1LL).setFeeCollectorAccountId(AccountId(2ULL))) })
                   .execute(getTestClient())
                   .getReceipt(getTestClient()),
               ReceiptStatusException); // CUSTOM_FEE_MUST_BE_POSITIVE
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CanCreateNft)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = std::shared_ptr<PrivateKey>(
      ED25519PrivateKey::fromString(
        "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137")
        .release()));

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = TokenCreateTransaction()
                                .setTokenName("ffff")
                                .setTokenSymbol("F")
                                .setTokenType(TokenType::NON_FUNGIBLE_UNIQUE)
                                .setTreasuryAccountId(AccountId(2ULL))
                                .setAdminKey(operatorKey)
                                .setFreezeKey(operatorKey)
                                .setWipeKey(operatorKey)
                                .setKycKey(operatorKey)
                                .setSupplyKey(operatorKey)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  TokenId tokenId;
  EXPECT_NO_THROW(tokenId = txReceipt.mTokenId.value());

  // Clean up
  ASSERT_NO_THROW(txReceipt =
                    TokenDeleteTransaction().setTokenId(tokenId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(TokenCreateTransactionIntegrationTest, CanCreateNftWithRoyaltyFee)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = std::shared_ptr<PrivateKey>(
      ED25519PrivateKey::fromString(
        "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137")
        .release()));

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(
    txReceipt =
      TokenCreateTransaction()
        .setTokenName("ffff")
        .setTokenSymbol("F")
        .setTokenType(TokenType::NON_FUNGIBLE_UNIQUE)
        .setTreasuryAccountId(AccountId(2ULL))
        .setAdminKey(operatorKey)
        .setSupplyKey(operatorKey)
        .setCustomFees({ std::make_shared<CustomRoyaltyFee>(CustomRoyaltyFee()
                                                              .setNumerator(1LL)
                                                              .setDenominator(10LL)
                                                              .setFallbackFee(CustomFixedFee().setHbarAmount(Hbar(1LL)))
                                                              .setFeeCollectorAccountId(AccountId(2ULL))) })
        .execute(getTestClient())
        .getReceipt(getTestClient()));

  // Then
  TokenId tokenId;
  EXPECT_NO_THROW(tokenId = txReceipt.mTokenId.value());

  // Clean up
  ASSERT_NO_THROW(txReceipt =
                    TokenDeleteTransaction().setTokenId(tokenId).execute(getTestClient()).getReceipt(getTestClient()));
}