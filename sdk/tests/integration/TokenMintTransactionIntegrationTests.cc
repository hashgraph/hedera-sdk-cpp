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
#include "BaseIntegrationTest.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "TokenCreateTransaction.h"
#include "TokenMintTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "exceptions/ReceiptStatusException.h"

#include <gtest/gtest.h>
#include <memory>

using namespace Hedera;

class TokenMintTransactionIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(TokenMintTransactionIntegrationTest, ExecuteTokenMintTransaction)
{
  // Given
  const uint64_t initialAmount = 100000ULL;
  const uint64_t mintAmount = 10ULL;

  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setDecimals(3U)
                              .setInitialSupply(initialAmount)
                              .setTreasuryAccountId(AccountId(2ULL))
                              .setAdminKey(operatorKey)
                              .setFreezeKey(operatorKey)
                              .setWipeKey(operatorKey)
                              .setKycKey(operatorKey)
                              .setSupplyKey(operatorKey)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = TokenMintTransaction()
                                .setTokenId(tokenId)
                                .setAmount(mintAmount)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  EXPECT_EQ(txReceipt.mNewTotalSupply, initialAmount + mintAmount);

  // Clean up
  // TODO: TokenDeleteTransaction
}

//-----
TEST_F(TokenMintTransactionIntegrationTest, CannotMintMoreTokensThanMaxSupply)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setSupplyType(TokenSupplyType::FINITE)
                              .setMaxSupply(5ULL)
                              .setTreasuryAccountId(AccountId(2ULL))
                              .setAdminKey(operatorKey)
                              .setSupplyKey(operatorKey)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // When / Then
  EXPECT_THROW(
    const TransactionReceipt txReceipt =
      TokenMintTransaction().setTokenId(tokenId).setAmount(6ULL).execute(getTestClient()).getReceipt(getTestClient()),
    ReceiptStatusException); // TOKEN_MAX_SUPPLY_REACHED

  // Clean up
  // TODO: TokenDeleteTransaction
}

//-----
TEST_F(TokenMintTransactionIntegrationTest, CannotMintTokensWhenTokenIdIsNotSet)
{
  // Given / When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt =
                 TokenMintTransaction().setAmount(6ULL).execute(getTestClient()).getReceipt(getTestClient()),
               PrecheckStatusException); // INVALID_TOKEN_ID

  // Clean up
  // TODO: TokenDeleteTransaction
}

//-----
TEST_F(TokenMintTransactionIntegrationTest, CanMintTokensWhenAmountIsNotSet)
{
  // Given
  const uint64_t initialAmount = 100000ULL;

  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setDecimals(3U)
                              .setInitialSupply(initialAmount)
                              .setTreasuryAccountId(AccountId(2ULL))
                              .setAdminKey(operatorKey)
                              .setFreezeKey(operatorKey)
                              .setWipeKey(operatorKey)
                              .setKycKey(operatorKey)
                              .setSupplyKey(operatorKey)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt =
                    TokenMintTransaction().setTokenId(tokenId).execute(getTestClient()).getReceipt(getTestClient()));

  // Then
  EXPECT_EQ(txReceipt.mNewTotalSupply, initialAmount);

  // Clean up
  // TODO: TokenDeleteTransaction
}

//-----
TEST_F(TokenMintTransactionIntegrationTest, CannotMintTokensWhenSupplyKeyDoesNotSignTransaction)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  std::shared_ptr<PrivateKey> supplyKey;
  ASSERT_NO_THROW(supplyKey = ED25519PrivateKey::generatePrivateKey());

  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setDecimals(3U)
                              .setInitialSupply(100000ULL)
                              .setTreasuryAccountId(AccountId(2ULL))
                              .setAdminKey(operatorKey)
                              .setFreezeKey(operatorKey)
                              .setWipeKey(operatorKey)
                              .setKycKey(operatorKey)
                              .setSupplyKey(supplyKey)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // When / Then
  EXPECT_THROW(
    const TransactionReceipt txReceipt =
      TokenMintTransaction().setTokenId(tokenId).setAmount(10ULL).execute(getTestClient()).getReceipt(getTestClient()),
    ReceiptStatusException); // INVALID_SIGNATURE

  // Clean up
  // TODO: TokenDeleteTransaction
}

//-----
TEST_F(TokenMintTransactionIntegrationTest, CanMintNfts)
{
  // Given
  const std::vector<std::vector<std::byte>> nftsMetadata = {
    { std::byte(0x01) }, { std::byte(0x02) }, { std::byte(0x03) }, { std::byte(0x04) }, { std::byte(0x05) },
    { std::byte(0x06) }, { std::byte(0x07) }, { std::byte(0x08) }, { std::byte(0x09) }, { std::byte(0x0A) },
  };

  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
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
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = TokenMintTransaction()
                                .setTokenId(tokenId)
                                .setMetadata(nftsMetadata)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  EXPECT_EQ(txReceipt.mSerialNumbers.size(), nftsMetadata.size());

  // Clean up
  // TODO: TokenDeleteTransaction
}

//-----
TEST_F(TokenMintTransactionIntegrationTest, CannotMintNftsIfMetadataIsTooBig)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));

  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
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
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = TokenMintTransaction()
                                                      .addMetadata(std::vector<std::byte>(101, std::byte(0xFF)))
                                                      .setTokenId(tokenId)
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               PrecheckStatusException); // METADATA_TOO_LONG

  // Clean up
  // TODO: TokenDeleteTransaction
}
