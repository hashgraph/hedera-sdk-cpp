/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "CustomFixedFee.h"
#include "CustomFractionalFee.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "TokenCreateTransaction.h"
#include "TokenDeleteTransaction.h"
#include "TokenFeeScheduleUpdateTransaction.h"
#include "TokenInfo.h"
#include "TokenInfoQuery.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "exceptions/ReceiptStatusException.h"

#include <gtest/gtest.h>

using namespace Hiero;

class TokenFeeScheduleUpdateTransactionIntegrationTests : public BaseIntegrationTest
{
};

//-----
TEST_F(TokenFeeScheduleUpdateTransactionIntegrationTests, ExecuteTokenFeeScheduleUpdateTransaction)
{
  // Given
  const std::vector<std::shared_ptr<CustomFee>> customFees = {
    std::make_shared<CustomFixedFee>(CustomFixedFee().setAmount(10ULL).setFeeCollectorAccountId(AccountId(2ULL))),
    std::make_shared<CustomFractionalFee>(CustomFractionalFee()
                                            .setNumerator(1LL)
                                            .setDenominator(10LL)
                                            .setMinimumAmount(1ULL)
                                            .setMaximumAmount(10LL)
                                            .setFeeCollectorAccountId(AccountId(2ULL)))
  };

  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  TokenId tokenId;
  EXPECT_NO_THROW(tokenId = TokenCreateTransaction()
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
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = TokenFeeScheduleUpdateTransaction()
                                .setTokenId(tokenId)
                                .setCustomFees(customFees)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  TokenInfo tokenInfo;
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));

  EXPECT_EQ(tokenInfo.mCustomFees.size(), customFees.size());

  // Clean up
  ASSERT_NO_THROW(txReceipt =
                    TokenDeleteTransaction().setTokenId(tokenId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(TokenFeeScheduleUpdateTransactionIntegrationTests, CannotUpdateFeeScheduleIfFeeScheduleKeyDoesNotSign)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  std::shared_ptr<PrivateKey> feeScheduleKey;

  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));
  ASSERT_NO_THROW(feeScheduleKey = ED25519PrivateKey::generatePrivateKey());

  TokenId tokenId;
  EXPECT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setTreasuryAccountId(AccountId(2ULL))
                              .setAdminKey(operatorKey)
                              .setFeeScheduleKey(feeScheduleKey)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt =
                 TokenFeeScheduleUpdateTransaction()
                   .setTokenId(tokenId)
                   .setCustomFees({ std::make_shared<CustomFixedFee>(), std::make_shared<CustomFractionalFee>() })
                   .execute(getTestClient())
                   .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_SIGNATURE

  // Clean up
  ASSERT_NO_THROW(const TransactionReceipt txReceipt =
                    TokenDeleteTransaction().setTokenId(tokenId).execute(getTestClient()).getReceipt(getTestClient()));
}
