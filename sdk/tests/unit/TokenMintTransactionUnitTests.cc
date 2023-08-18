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
#include "AccountId.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "TokenMintTransaction.h"
#include "exceptions/IllegalStateException.h"
#include "impl/Utilities.h"

#include <cstdint>
#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>
#include <vector>

using namespace Hedera;

class TokenMintTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(), ECDSAsecp256k1PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTestTokenId; }
  [[nodiscard]] inline const uint64_t& getTestAmount() const { return mTestAmount; }
  [[nodiscard]] inline const std::vector<std::vector<std::byte>>& getTestMetadataList() const
  {
    return mTestMetadataList;
  }

private:
  Client mClient;
  const TokenId mTestTokenId = TokenId(1ULL, 2ULL, 3ULL);
  const uint64_t mTestAmount = 4ULL;
  const std::vector<std::vector<std::byte>> mTestMetadataList = {
    {std::byte(0x05),  std::byte(0x06), std::byte(0x07)},
    { std::byte(0x08), std::byte(0x09), std::byte(0x0A)},
    { std::byte(0x0B), std::byte(0x0C), std::byte(0x0D)}
  };
};

//-----
TEST_F(TokenMintTransactionTest, ConstructTokenMintTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::TokenMintTransactionBody>();
  body->set_allocated_token(getTestTokenId().toProtobuf().release());
  body->set_amount(getTestAmount());

  for (const auto& metadata : getTestMetadataList())
  {
    body->add_metadata(internal::Utilities::byteVectorToString(metadata));
  }

  proto::TransactionBody txBody;
  txBody.set_allocated_tokenmint(body.release());

  // When
  const TokenMintTransaction tokenMintTransaction(txBody);

  // Then
  EXPECT_EQ(tokenMintTransaction.getTokenId(), getTestTokenId());
  EXPECT_EQ(tokenMintTransaction.getAmount(), getTestAmount());
  EXPECT_EQ(tokenMintTransaction.getMetadata(), getTestMetadataList());
}

//-----
TEST_F(TokenMintTransactionTest, GetSetTokenId)
{
  // Given
  TokenMintTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setTokenId(getTestTokenId()));

  // Then
  EXPECT_EQ(transaction.getTokenId(), getTestTokenId());
}

//-----
TEST_F(TokenMintTransactionTest, GetSetTokenIdFrozen)
{
  // Given
  TokenMintTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setTokenId(getTestTokenId()), IllegalStateException);
}

//-----
TEST_F(TokenMintTransactionTest, GetSetAmount)
{
  // Given
  TokenMintTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setAmount(getTestAmount()));

  // Then
  EXPECT_EQ(transaction.getAmount(), getTestAmount());
}

//-----
TEST_F(TokenMintTransactionTest, GetSetAmountFrozen)
{
  // Given
  TokenMintTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setAmount(getTestAmount()), IllegalStateException);
}

//-----
TEST_F(TokenMintTransactionTest, GetSetMetadata)
{
  // Given
  TokenMintTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.setMetadata(getTestMetadataList()));

  // Then
  EXPECT_EQ(transaction.getMetadata(), getTestMetadataList());
}

//-----
TEST_F(TokenMintTransactionTest, GetSetMetadataFrozen)
{
  // Given
  TokenMintTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(&getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.setMetadata(getTestMetadataList()), IllegalStateException);
}

//-----
TEST_F(TokenMintTransactionTest, AddMetadata)
{
  // Given
  TokenMintTransaction tokenMintTransaction;
  const std::vector<std::byte> metadata = { std::byte(0x0E), std::byte(0x0F) };

  // When
  EXPECT_NO_THROW(tokenMintTransaction.addMetadata(metadata));

  // Then
  EXPECT_EQ(tokenMintTransaction.getMetadata(), std::vector<std::vector<std::byte>>{ metadata });
}
