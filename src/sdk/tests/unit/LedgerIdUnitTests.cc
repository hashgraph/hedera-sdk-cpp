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
#include "LedgerId.h"
#include "impl/HexConverter.h"

#include <cstddef>
#include <gtest/gtest.h>
#include <vector>

using namespace Hiero;

class LedgerIdUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const std::vector<std::byte>& getTestBytes() const { return mTestBytes; }

private:
  const std::vector<std::byte> mTestBytes = { std::byte(0x10), std::byte(0x20), std::byte(0x30) };
};

//-----
TEST_F(LedgerIdUnitTests, MainnetLedgerIdInitialization)
{
  // Given / When / Then
  EXPECT_EQ(LedgerId::MAINNET.toBytes(), std::vector<std::byte>{ std::byte(0x0) });
  EXPECT_TRUE(LedgerId::MAINNET.isMainnet());
}

//-----
TEST_F(LedgerIdUnitTests, TestnetLedgerIdInitialization)
{
  // Given / When / Then
  EXPECT_EQ(LedgerId::TESTNET.toBytes(), std::vector<std::byte>{ std::byte(0x1) });
  EXPECT_TRUE(LedgerId::TESTNET.isTestnet());
}

//-----
TEST_F(LedgerIdUnitTests, PreviewnetLedgerIdInitialization)
{
  // Given / When / Then
  EXPECT_EQ(LedgerId::PREVIEWNET.toBytes(), std::vector<std::byte>{ std::byte(0x2) });
  EXPECT_TRUE(LedgerId::PREVIEWNET.isPreviewnet());
}

//-----
TEST_F(LedgerIdUnitTests, ConstructLedgerIdWithMainnetBytes)
{
  // Given / When
  const LedgerId ledgerId(LedgerId::MAINNET.toBytes());

  // Then
  EXPECT_TRUE(ledgerId.isMainnet());
  EXPECT_FALSE(ledgerId.isTestnet());
  EXPECT_FALSE(ledgerId.isPreviewnet());
  EXPECT_EQ(ledgerId.toBytes(), LedgerId::MAINNET.toBytes());
}

//-----
TEST_F(LedgerIdUnitTests, ConstructLedgerIdWithTestnetBytes)
{
  // Given / When
  const LedgerId ledgerId(LedgerId::TESTNET.toBytes());

  // Then
  EXPECT_FALSE(ledgerId.isMainnet());
  EXPECT_TRUE(ledgerId.isTestnet());
  EXPECT_FALSE(ledgerId.isPreviewnet());
  EXPECT_EQ(ledgerId.toBytes(), LedgerId::TESTNET.toBytes());
}

//-----
TEST_F(LedgerIdUnitTests, ConstructLedgerIdWithPreviewnetBytes)
{
  // Given / When
  const LedgerId ledgerId(LedgerId::PREVIEWNET.toBytes());

  // Then
  EXPECT_FALSE(ledgerId.isMainnet());
  EXPECT_FALSE(ledgerId.isTestnet());
  EXPECT_TRUE(ledgerId.isPreviewnet());
  EXPECT_EQ(ledgerId.toBytes(), LedgerId::PREVIEWNET.toBytes());
}

//-----
TEST_F(LedgerIdUnitTests, ConstructLedgerIdWithBytes)
{
  // Given / When
  const LedgerId ledgerId(getTestBytes());

  // Then
  EXPECT_FALSE(ledgerId.isMainnet());
  EXPECT_FALSE(ledgerId.isTestnet());
  EXPECT_FALSE(ledgerId.isPreviewnet());
  EXPECT_EQ(ledgerId.toBytes(), getTestBytes());
}

//-----
TEST_F(LedgerIdUnitTests, FromString)
{
  // Given / When
  const LedgerId mainnet = LedgerId::fromString("mainnet");
  const LedgerId testnet = LedgerId::fromString("testnet");
  const LedgerId previewnet = LedgerId::fromString("previewnet");
  const LedgerId ledgerFromBytes = LedgerId::fromString(internal::HexConverter::bytesToHex(getTestBytes()));

  // Then
  EXPECT_TRUE(mainnet.isMainnet());
  EXPECT_TRUE(testnet.isTestnet());
  EXPECT_TRUE(previewnet.isPreviewnet());
  EXPECT_EQ(ledgerFromBytes.toBytes(), getTestBytes());
}

//-----
TEST_F(LedgerIdUnitTests, FromBadStrings)
{
  // Given / When / Then
  EXPECT_THROW(const LedgerId ledgerId = LedgerId::fromString("mainet"), std::invalid_argument);
  EXPECT_THROW(const LedgerId ledgerId = LedgerId::fromString("this is a bad string"), std::invalid_argument);
  EXPECT_THROW(const LedgerId ledgerId = LedgerId::fromString("1234567890abcdefg"), std::invalid_argument);
  EXPECT_THROW(const LedgerId ledgerId = LedgerId::fromString("TESTNET"), std::invalid_argument);
  EXPECT_THROW(const LedgerId ledgerId = LedgerId::fromString("Previewnet"), std::invalid_argument);
}

//-----
TEST_F(LedgerIdUnitTests, ToString)
{
  // Given
  const std::string hexString = internal::HexConverter::bytesToHex(getTestBytes());
  const LedgerId ledgerId = LedgerId::fromString(hexString);

  // When
  const std::string mainnetStr = LedgerId::MAINNET.toString();
  const std::string testnetStr = LedgerId::TESTNET.toString();
  const std::string previewnetStr = LedgerId::PREVIEWNET.toString();
  const std::string toStringStr = ledgerId.toString();

  // Then
  EXPECT_EQ(mainnetStr, "mainnet");
  EXPECT_EQ(testnetStr, "testnet");
  EXPECT_EQ(previewnetStr, "previewnet");
  EXPECT_EQ(toStringStr, hexString);
}
