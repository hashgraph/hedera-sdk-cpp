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
#include "AccountAllowanceApproveTransaction.h"
#include "AccountId.h"
#include "Client.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "Hbar.h"
#include "NftId.h"
#include "TokenId.h"
#include "exceptions/IllegalStateException.h"

#include <gtest/gtest.h>
#include <proto/transaction_body.pb.h>
#include <stdexcept>
#include <vector>

using namespace Hedera;

class AccountAllowanceApproveTransactionTest : public ::testing::Test
{
protected:
  void SetUp() override { mClient.setOperator(AccountId(1ULL), ECDSAsecp256k1PrivateKey::generatePrivateKey().get()); }

  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const AccountId& getTestOwnerAccountId() const { return mOwnerAccountId; }
  [[nodiscard]] inline const AccountId& getTestSpenderAccountId() const { return mSpenderAccountId; }
  [[nodiscard]] inline const Hbar& getTestAmountHbar() const { return mAmountHbar; }
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTokenId; }
  [[nodiscard]] inline const std::vector<uint64_t>& getTestSerialNumbers() const { return mSerialNumbers; }
  [[nodiscard]] inline bool getTestApprovalForAll() const { return mApprovalForAll; }
  [[nodiscard]] inline const AccountId& getTestDelegatingSpenderAccountId() const
  {
    return mDelegatingSpenderAccountId;
  }

private:
  Client mClient;
  const AccountId mOwnerAccountId = AccountId(2ULL);
  const AccountId mSpenderAccountId = AccountId(3ULL);
  const Hbar mAmountHbar = Hbar(4LL);
  const TokenId mTokenId = TokenId(5ULL);
  const std::vector<uint64_t> mSerialNumbers = { 6ULL, 7ULL, 8ULL };
  const bool mApprovalForAll = true;
  const AccountId mDelegatingSpenderAccountId = AccountId(9ULL);
};

//-----
TEST_F(AccountAllowanceApproveTransactionTest, ConstructAccountAllowanceApproveTransaction)
{
  // Given / When
  AccountAllowanceApproveTransaction transaction;

  // Then
  EXPECT_TRUE(transaction.getHbarApprovals().empty());
  EXPECT_TRUE(transaction.getTokenApprovals().empty());
  EXPECT_TRUE(transaction.getNftApprovals().empty());
}

//-----
TEST_F(AccountAllowanceApproveTransactionTest, ConstructAccountAllowanceApproveTransactionFromTransactionBodyProtobuf)
{
  // Given
  auto body = std::make_unique<proto::CryptoApproveAllowanceTransactionBody>();
  proto::CryptoAllowance* cryptoAllowance = body->add_cryptoallowances();
  cryptoAllowance->set_allocated_owner(getTestOwnerAccountId().toProtobuf().release());
  cryptoAllowance->set_allocated_spender(getTestSpenderAccountId().toProtobuf().release());
  cryptoAllowance->set_amount(getTestAmountHbar().toTinybars());

  proto::TokenAllowance* tokenAllowance = body->add_tokenallowances();
  tokenAllowance->set_allocated_tokenid(getTestTokenId().toProtobuf().release());
  tokenAllowance->set_allocated_owner(getTestOwnerAccountId().toProtobuf().release());
  tokenAllowance->set_allocated_spender(getTestSpenderAccountId().toProtobuf().release());
  tokenAllowance->set_amount(getTestAmountHbar().toTinybars());

  proto::NftAllowance* nftAllowance = body->add_nftallowances();
  nftAllowance->set_allocated_tokenid(getTestTokenId().toProtobuf().release());
  nftAllowance->set_allocated_owner(getTestOwnerAccountId().toProtobuf().release());
  nftAllowance->set_allocated_spender(getTestSpenderAccountId().toProtobuf().release());

  for (const uint64_t& num : getTestSerialNumbers())
  {
    nftAllowance->add_serial_numbers(static_cast<int64_t>(num));
  }

  nftAllowance->mutable_approved_for_all()->set_value(getTestApprovalForAll());
  nftAllowance->set_allocated_delegating_spender(getTestDelegatingSpenderAccountId().toProtobuf().release());

  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoapproveallowance(body.release());

  // When
  const AccountAllowanceApproveTransaction accountAllowanceApproveTransaction(txBody);

  // Then
  ASSERT_EQ(accountAllowanceApproveTransaction.getHbarApprovals().size(), 1);
  EXPECT_EQ(accountAllowanceApproveTransaction.getHbarApprovals().at(0).getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(accountAllowanceApproveTransaction.getHbarApprovals().at(0).getSpenderAccountId(),
            getTestSpenderAccountId());
  EXPECT_EQ(accountAllowanceApproveTransaction.getHbarApprovals().at(0).getAmount(), getTestAmountHbar());

  ASSERT_EQ(accountAllowanceApproveTransaction.getTokenApprovals().size(), 1);
  EXPECT_EQ(accountAllowanceApproveTransaction.getTokenApprovals().at(0).getTokenId(), getTestTokenId());
  EXPECT_EQ(accountAllowanceApproveTransaction.getTokenApprovals().at(0).getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(accountAllowanceApproveTransaction.getTokenApprovals().at(0).getSpenderAccountId(),
            getTestSpenderAccountId());
  EXPECT_EQ(accountAllowanceApproveTransaction.getTokenApprovals().at(0).getAmount(), getTestAmountHbar().toTinybars());

  ASSERT_EQ(accountAllowanceApproveTransaction.getNftApprovals().size(), 1);
  EXPECT_EQ(accountAllowanceApproveTransaction.getNftApprovals().at(0).getTokenId(), getTestTokenId());
  EXPECT_EQ(accountAllowanceApproveTransaction.getNftApprovals().at(0).getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(accountAllowanceApproveTransaction.getNftApprovals().at(0).getSpenderAccountId(),
            getTestSpenderAccountId());
  EXPECT_EQ(accountAllowanceApproveTransaction.getNftApprovals().at(0).getSerialNumbers(), getTestSerialNumbers());
  ASSERT_TRUE(accountAllowanceApproveTransaction.getNftApprovals().at(0).getApprovedForAll().has_value());
  EXPECT_EQ(*accountAllowanceApproveTransaction.getNftApprovals().at(0).getApprovedForAll(), getTestApprovalForAll());
  EXPECT_EQ(*accountAllowanceApproveTransaction.getNftApprovals().at(0).getDelegateSpender(),
            getTestDelegatingSpenderAccountId());
}

//-----
TEST_F(AccountAllowanceApproveTransactionTest, ApproveHbarAllowance)
{
  // Given
  AccountAllowanceApproveTransaction transaction;

  // When
  EXPECT_NO_THROW(
    transaction.approveHbarAllowance(getTestOwnerAccountId(), getTestSpenderAccountId(), getTestAmountHbar()));

  // Then
  ASSERT_EQ(transaction.getHbarApprovals().size(), 1);
  EXPECT_EQ(transaction.getHbarApprovals().at(0).getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(transaction.getHbarApprovals().at(0).getSpenderAccountId(), getTestSpenderAccountId());
  EXPECT_EQ(transaction.getHbarApprovals().at(0).getAmount(), getTestAmountHbar());
}

//-----
TEST_F(AccountAllowanceApproveTransactionTest, ApproveHbarAllowanceFrozen)
{
  // Given
  AccountAllowanceApproveTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(
    transaction.approveHbarAllowance(getTestOwnerAccountId(), getTestSpenderAccountId(), getTestAmountHbar()),
    IllegalStateException);
}

//-----
TEST_F(AccountAllowanceApproveTransactionTest, ApproveHbarAllowanceNegativeAllowance)
{
  // Given
  AccountAllowanceApproveTransaction transaction;

  // When / Then
  EXPECT_THROW(
    transaction.approveHbarAllowance(getTestOwnerAccountId(), getTestSpenderAccountId(), getTestAmountHbar().negated()),
    std::invalid_argument);
}

//-----
TEST_F(AccountAllowanceApproveTransactionTest, ApproveTokenAllowance)
{
  // Given
  AccountAllowanceApproveTransaction transaction;

  // When
  EXPECT_NO_THROW(transaction.approveTokenAllowance(
    getTestTokenId(), getTestOwnerAccountId(), getTestSpenderAccountId(), getTestAmountHbar().toTinybars()));

  // Then
  ASSERT_EQ(transaction.getTokenApprovals().size(), 1);
  EXPECT_EQ(transaction.getTokenApprovals().at(0).getTokenId(), getTestTokenId());
  EXPECT_EQ(transaction.getTokenApprovals().at(0).getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(transaction.getTokenApprovals().at(0).getSpenderAccountId(), getTestSpenderAccountId());
  EXPECT_EQ(transaction.getTokenApprovals().at(0).getAmount(), getTestAmountHbar().toTinybars());
}

//-----
TEST_F(AccountAllowanceApproveTransactionTest, ApproveTokenAllowanceFrozen)
{
  // Given
  AccountAllowanceApproveTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(
    transaction.approveTokenAllowance(
      getTestTokenId(), getTestOwnerAccountId(), getTestSpenderAccountId(), getTestAmountHbar().toTinybars()),
    IllegalStateException);
}

//-----
TEST_F(AccountAllowanceApproveTransactionTest, ApproveNftAllowance)
{
  // Given
  AccountAllowanceApproveTransaction transaction;
  const TokenId tokenId(20ULL);
  const uint64_t serialNum = 30ULL;

  // When
  for (const uint64_t& num : getTestSerialNumbers())
  {
    EXPECT_NO_THROW(transaction.approveTokenNftAllowance(
      NftId(getTestTokenId(), num), getTestOwnerAccountId(), getTestSpenderAccountId()));
  }

  EXPECT_NO_THROW(transaction.approveTokenNftAllowance(
    NftId(tokenId, serialNum), getTestOwnerAccountId(), getTestSpenderAccountId()));

  // Then
  ASSERT_EQ(transaction.getNftApprovals().size(), 2);

  EXPECT_EQ(transaction.getNftApprovals().at(0).getTokenId(), getTestTokenId());
  EXPECT_EQ(transaction.getNftApprovals().at(0).getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(transaction.getNftApprovals().at(0).getSpenderAccountId(), getTestSpenderAccountId());
  ASSERT_EQ(transaction.getNftApprovals().at(0).getSerialNumbers().size(), getTestSerialNumbers().size());
  for (int i = 0; i < getTestSerialNumbers().size(); ++i)
  {
    EXPECT_EQ(transaction.getNftApprovals().at(0).getSerialNumbers().at(i), getTestSerialNumbers().at(i));
  }

  EXPECT_EQ(transaction.getNftApprovals().at(1).getTokenId(), tokenId);
  EXPECT_EQ(transaction.getNftApprovals().at(1).getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(transaction.getNftApprovals().at(1).getSpenderAccountId(), getTestSpenderAccountId());
  ASSERT_EQ(transaction.getNftApprovals().at(1).getSerialNumbers().size(), 1);
  EXPECT_EQ(transaction.getNftApprovals().at(1).getSerialNumbers().at(0), serialNum);
}

//-----
TEST_F(AccountAllowanceApproveTransactionTest, ApproveNftAllowanceFrozen)
{
  // Given
  AccountAllowanceApproveTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(transaction.approveTokenNftAllowance(
                 NftId(getTestTokenId(), 1ULL), getTestOwnerAccountId(), getTestSpenderAccountId()),
               IllegalStateException);
}

//-----
TEST_F(AccountAllowanceApproveTransactionTest, ApproveNftAllowanceAllSerials)
{
  // Given
  AccountAllowanceApproveTransaction transaction;

  // When
  EXPECT_NO_THROW(
    transaction.approveNftAllowanceAllSerials(getTestTokenId(), getTestOwnerAccountId(), getTestSpenderAccountId()));

  // Then
  ASSERT_EQ(transaction.getNftApprovals().size(), 1);
  EXPECT_EQ(transaction.getNftApprovals().at(0).getTokenId(), getTestTokenId());
  EXPECT_EQ(transaction.getNftApprovals().at(0).getOwnerAccountId(), getTestOwnerAccountId());
  EXPECT_EQ(transaction.getNftApprovals().at(0).getSpenderAccountId(), getTestSpenderAccountId());
  ASSERT_TRUE(transaction.getNftApprovals().at(0).getSerialNumbers().empty());
  ASSERT_TRUE(transaction.getNftApprovals().at(0).getApprovedForAll().has_value());
  EXPECT_TRUE(*transaction.getNftApprovals().at(0).getApprovedForAll());
  EXPECT_FALSE(transaction.getNftApprovals().at(0).getDelegateSpender());
}

//-----
TEST_F(AccountAllowanceApproveTransactionTest, ApproveNftAllowanceAllSerialsFrozen)
{
  // Given
  AccountAllowanceApproveTransaction transaction;
  ASSERT_NO_THROW(transaction.freezeWith(getTestClient()));

  // When / Then
  EXPECT_THROW(
    transaction.approveNftAllowanceAllSerials(getTestTokenId(), getTestOwnerAccountId(), getTestSpenderAccountId()),
    IllegalStateException);
}
