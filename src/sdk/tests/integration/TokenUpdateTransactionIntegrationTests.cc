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
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "AccountId.h"
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "TokenCreateTransaction.h"
#include "TokenDeleteTransaction.h"
#include "TokenInfo.h"
#include "TokenInfoQuery.h"
#include "TokenUpdateTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "exceptions/ReceiptStatusException.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>

using namespace Hedera;

class TokenUpdateTransactionIntegrationTests : public BaseIntegrationTest
{
protected:
  [[nodiscard]] inline const std::vector<std::byte>& getTestMetadata() const { return mMetadata; }

private:
  const std::vector<std::byte> mMetadata = { std::byte(0xAA), std::byte(0xAB), std::byte(0xAC), std::byte(0xAD) };
};

//-----
TEST_F(TokenUpdateTransactionIntegrationTests, ExecuteTokenUpdateTransaction)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));
  const std::string updatedTokenName = "Token";
  const std::string updatedTokenSymbol = "T";

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
  EXPECT_NO_THROW(txReceipt = TokenUpdateTransaction()
                                .setTokenId(tokenId)
                                .setTokenName(updatedTokenName)
                                .setTokenSymbol(updatedTokenSymbol)
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  TokenInfo tokenInfo;
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));

  EXPECT_EQ(tokenInfo.mTokenName, updatedTokenName);
  EXPECT_EQ(tokenInfo.mTokenSymbol, updatedTokenSymbol);

  // Clean up
  ASSERT_NO_THROW(txReceipt =
                    TokenDeleteTransaction().setTokenId(tokenId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(TokenUpdateTransactionIntegrationTests, CannotUpdateImmutableKey)
{
  // Given
  std::shared_ptr<PrivateKey> operatorKey;
  ASSERT_NO_THROW(
    operatorKey = ED25519PrivateKey::fromString(
      "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137"));
  const std::string updatedTokenName = "Token";
  const std::string updatedTokenSymbol = "T";

  TokenId tokenId;
  EXPECT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setTreasuryAccountId(AccountId(2ULL))
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = TokenUpdateTransaction()
                                                      .setTokenId(tokenId)
                                                      .setTokenName(updatedTokenName)
                                                      .setTokenSymbol(updatedTokenSymbol)
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // TOKEN_IS_IMMUTABLE
}

//-----
TEST_F(TokenUpdateTransactionIntegrationTests, CanUpdateFungibleTokenMetadata)
{
  // Given
  std::shared_ptr<PrivateKey> metadataKey;
  ASSERT_NO_THROW(metadataKey = ED25519PrivateKey::generatePrivateKey());
  const std::vector<std::byte> updatedMetadataRecord = {
    std::byte(0xBA), std::byte(0xBB), std::byte(0xBC), std::byte(0xBD)
  };

  // create a fungible token with metadata and metadata key
  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setMetadata(getTestMetadata())
                              .setTokenType(TokenType::FUNGIBLE_COMMON)
                              .setDecimals(3)
                              .setInitialSupply(1000000)
                              .setTreasuryAccountId(getTestClient().getOperatorAccountId().value())
                              .setAdminKey(getTestClient().getOperatorPublicKey())
                              .setMetadataKey(metadataKey)
                              .setFreezeDefault(false)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // check if token created successfully with metadata and metadata key
  TokenInfo tokenInfo;
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));
  EXPECT_EQ(tokenInfo.mMetadata, getTestMetadata());
  EXPECT_EQ(tokenInfo.mMetadataKey->toBytes(), metadataKey->getPublicKey()->toBytes());

  // When
  // update token's metadata
  ASSERT_NO_THROW(TokenUpdateTransaction()
                    .setTokenId(tokenId)
                    .setMetadata(updatedMetadataRecord)
                    .freezeWith(&getTestClient())
                    .sign(metadataKey)
                    .execute(getTestClient())
                    .getReceipt(getTestClient()));

  // Then
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));
  EXPECT_EQ(tokenInfo.mMetadata, updatedMetadataRecord);
}

//-----
TEST_F(TokenUpdateTransactionIntegrationTests, CanUpdateNonFungibleTokenMetadata)
{
  // Given
  std::shared_ptr<PrivateKey> metadataKey;
  ASSERT_NO_THROW(metadataKey = ED25519PrivateKey::generatePrivateKey());
  const std::vector<std::byte> updatedMetadataRecord = {
    std::byte(0xBA), std::byte(0xBB), std::byte(0xBC), std::byte(0xBD)
  };

  // create a NFT with metadata and metadata key
  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setMetadata(getTestMetadata())
                              .setTokenType(TokenType::NON_FUNGIBLE_UNIQUE)
                              .setTreasuryAccountId(getTestClient().getOperatorAccountId().value())
                              .setAdminKey(getTestClient().getOperatorPublicKey())
                              .setSupplyKey(getTestClient().getOperatorPublicKey())
                              .setMetadataKey(metadataKey)
                              .setFreezeDefault(false)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // check if NFT created successfully with metadata and metadata key
  TokenInfo tokenInfo;
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));
  EXPECT_EQ(tokenInfo.mMetadata, getTestMetadata());
  EXPECT_EQ(tokenInfo.mMetadataKey->toBytes(), metadataKey->getPublicKey()->toBytes());

  // When
  // update token's metadata
  ASSERT_NO_THROW(TokenUpdateTransaction()
                    .setTokenId(tokenId)
                    .setMetadata(updatedMetadataRecord)
                    .freezeWith(&getTestClient())
                    .sign(metadataKey)
                    .execute(getTestClient())
                    .getReceipt(getTestClient()));

  // Then
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));
  EXPECT_EQ(tokenInfo.mMetadata, updatedMetadataRecord);
}

//-----
// This test verifies that metadata is not updated silently. This test is kept for regression
// because previously the metadata got updated silently
TEST_F(TokenUpdateTransactionIntegrationTests, CannotUpdateFungibleTokenMetadataKeyNotSet)
{
  // Given
  // create a NFT without metadata and metadata key
  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setMetadata(getTestMetadata())
                              .setTokenType(TokenType::FUNGIBLE_COMMON)
                              .setTreasuryAccountId(getTestClient().getOperatorAccountId().value())
                              .setAdminKey(getTestClient().getOperatorPublicKey())
                              .setSupplyKey(getTestClient().getOperatorPublicKey())
                              .setFreezeDefault(false)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // check if FT created successfully with metadata and metadata key
  TokenInfo tokenInfo;
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));
  EXPECT_EQ(tokenInfo.mMetadata, getTestMetadata());
  EXPECT_EQ(tokenInfo.mMetadataKey, nullptr);

  // When
  // update token's metadata
  ASSERT_NO_THROW(TokenUpdateTransaction()
                    .setTokenId(tokenId)
                    .freezeWith(&getTestClient())
                    .execute(getTestClient())
                    .getReceipt(getTestClient()));

  // Then
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));
  EXPECT_EQ(tokenInfo.mMetadata, getTestMetadata());
}

//-----
// This test verifies that metadata is not updated silently. This test is kept for regression
// because previously the metadata got updated silently
TEST_F(TokenUpdateTransactionIntegrationTests, CannotUpdateNonFungibleTokenMetadataKeyNotSet)
{
  // Given
  // create a NFT without metadata and metadata key
  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setMetadata(getTestMetadata())
                              .setTokenType(TokenType::NON_FUNGIBLE_UNIQUE)
                              .setTreasuryAccountId(getTestClient().getOperatorAccountId().value())
                              .setAdminKey(getTestClient().getOperatorPublicKey())
                              .setSupplyKey(getTestClient().getOperatorPublicKey())
                              .setFreezeDefault(false)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // check if NFT created successfully with metadata and metadata key
  TokenInfo tokenInfo;
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));
  EXPECT_EQ(tokenInfo.mMetadata, getTestMetadata());
  EXPECT_EQ(tokenInfo.mMetadataKey, nullptr);

  // When
  // update token's metadata
  ASSERT_NO_THROW(TokenUpdateTransaction()
                    .setTokenId(tokenId)
                    .setTokenMemo("abc")
                    .execute(getTestClient())
                    .getReceipt(getTestClient()));

  // Then
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));
  EXPECT_EQ(tokenInfo.mMetadata, getTestMetadata());
}

//-----
TEST_F(TokenUpdateTransactionIntegrationTests,
       CannotUpdateFungibleTokenMetadataWhenTransactionNotSignedWithMetadataOrAdminKey)
{
  // Given
  std::shared_ptr<PrivateKey> metadataKey;
  ASSERT_NO_THROW(metadataKey = ED25519PrivateKey::generatePrivateKey());
  std::shared_ptr<PrivateKey> adminKey;
  ASSERT_NO_THROW(adminKey = ED25519PrivateKey::generatePrivateKey());
  const std::vector<std::byte> updatedMetadataRecord = {
    std::byte(0xBA), std::byte(0xBB), std::byte(0xBC), std::byte(0xBD)
  };

  // create a FT with metadata and metadata key
  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setMetadata(getTestMetadata())
                              .setTokenType(TokenType::FUNGIBLE_COMMON)
                              .setTreasuryAccountId(getTestClient().getOperatorAccountId().value())
                              .setAdminKey(adminKey)
                              .setMetadataKey(metadataKey)
                              .setSupplyKey(getTestClient().getOperatorPublicKey())
                              .freezeWith(&getTestClient())
                              .sign(adminKey)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // check if FT created successfully with metadata and metadata key
  TokenInfo tokenInfo;
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));

  // When / Then
  // update token's metadata
  EXPECT_THROW(TokenUpdateTransaction()
                 .setTokenId(tokenId)
                 .setMetadata(updatedMetadataRecord)
                 .freezeWith(&getTestClient())
                 .execute(getTestClient())
                 .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_SIGNATURE
}

//-----
TEST_F(TokenUpdateTransactionIntegrationTests,
       CannotUpdateNonFungibleTokenMetadataWhenTransactionNotSignedWithMetadataOrAdminKey)
{
  // Given
  std::shared_ptr<PrivateKey> metadataKey;
  ASSERT_NO_THROW(metadataKey = ED25519PrivateKey::generatePrivateKey());
  std::shared_ptr<PrivateKey> adminKey;
  ASSERT_NO_THROW(adminKey = ED25519PrivateKey::generatePrivateKey());
  const std::vector<std::byte> updatedMetadataRecord = {
    std::byte(0xBA), std::byte(0xBB), std::byte(0xBC), std::byte(0xBD)
  };

  // create a NFT with metadata and metadata key
  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setMetadata(getTestMetadata())
                              .setTokenType(TokenType::NON_FUNGIBLE_UNIQUE)
                              .setTreasuryAccountId(getTestClient().getOperatorAccountId().value())
                              .setAdminKey(adminKey)
                              .setMetadataKey(metadataKey)
                              .setSupplyKey(getTestClient().getOperatorPublicKey())
                              .freezeWith(&getTestClient())
                              .sign(adminKey)
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // check if NFT created successfully with metadata and metadata key
  TokenInfo tokenInfo;
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));

  // When / Then
  // update token's metadata
  EXPECT_THROW(TokenUpdateTransaction()
                 .setTokenId(tokenId)
                 .setMetadata(updatedMetadataRecord)
                 .freezeWith(&getTestClient())
                 .execute(getTestClient())
                 .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_SIGNATURE
}

//-----
TEST_F(TokenUpdateTransactionIntegrationTests, CannotUpdateNonFungibleTokenMetadataWhenMetadataKeyNotSet)
{
  // Given
  const std::vector<std::byte> updatedMetadataRecord = {
    std::byte(0xBA), std::byte(0xBB), std::byte(0xBC), std::byte(0xBD)
  };

  // create a NFT with metadata
  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setMetadata(getTestMetadata())
                              .setTokenType(TokenType::NON_FUNGIBLE_UNIQUE)
                              .setTreasuryAccountId(getTestClient().getOperatorAccountId().value())
                              .setSupplyKey(getTestClient().getOperatorPublicKey())
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // check if NFT created successfully with metadata
  TokenInfo tokenInfo;
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));

  // When / Then
  // update token's metadata
  EXPECT_THROW(TokenUpdateTransaction()
                 .setTokenId(tokenId)
                 .setMetadata(updatedMetadataRecord)
                 .execute(getTestClient())
                 .getReceipt(getTestClient()),
               ReceiptStatusException); // TOKEN_IS_IMMUTABLE
}

//-----
TEST_F(TokenUpdateTransactionIntegrationTests, CannotUpdateFungibleTokenMetadataWhenMetadataKeyNotSet)
{
  // Given
  const std::vector<std::byte> updatedMetadataRecord = {
    std::byte(0xBA), std::byte(0xBB), std::byte(0xBC), std::byte(0xBD)
  };

  // create a NFT with metadata
  TokenId tokenId;
  ASSERT_NO_THROW(tokenId = TokenCreateTransaction()
                              .setTokenName("ffff")
                              .setTokenSymbol("F")
                              .setMetadata(getTestMetadata())
                              .setTokenType(TokenType::FUNGIBLE_COMMON)
                              .setTreasuryAccountId(getTestClient().getOperatorAccountId().value())
                              .setSupplyKey(getTestClient().getOperatorPublicKey())
                              .execute(getTestClient())
                              .getReceipt(getTestClient())
                              .mTokenId.value());

  // check if NFT created successfully with metadata and metadata key
  TokenInfo tokenInfo;
  ASSERT_NO_THROW(tokenInfo = TokenInfoQuery().setTokenId(tokenId).execute(getTestClient()));

  // When / Then
  // update token's metadata
  EXPECT_THROW(TokenUpdateTransaction()
                 .setTokenId(tokenId)
                 .setMetadata(updatedMetadataRecord)
                 .execute(getTestClient())
                 .getReceipt(getTestClient()),
               ReceiptStatusException); // TOKEN_IS_IMMUTABLE
}