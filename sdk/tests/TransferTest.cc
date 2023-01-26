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
#include "Transfer.h"
#include "AccountId.h"
#include "Hbar.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class TransferTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const int64_t getTestAmount() const { return mAmount; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }

private:
  const int64_t mAmount = 10LL;
  const AccountId mAccountId = AccountId(10ULL);
};

// Tests serialization of Hedera::Transfer -> proto::AccountAmount.
TEST_F(TransferTest, SerializeTransferToProtobuf)
{
  // Given
  const AccountId testAccountId = getTestAccountId();
  const int64_t testAmount = getTestAmount();
  const Hbar testHbarAmount = Hbar(testAmount, HbarUnit::TINYBAR());

  Transfer testTransfer = Transfer();
  testTransfer.setAccountId(testAccountId);
  testTransfer.setAmount(testHbarAmount);
  testTransfer.setApproved(false);

  // When
  const auto protoAccountAmountPtr = std::unique_ptr<proto::AccountAmount>(testTransfer.toProtobuf());

  // Then
  EXPECT_EQ(protoAccountAmountPtr->accountid().accountnum(), testAccountId.getAccountNum());
  EXPECT_EQ(protoAccountAmountPtr->amount(), testAmount);
  EXPECT_FALSE(protoAccountAmountPtr->is_approval());
}

// Tests deserialization of proto::AccountAmount -> Hedera::Transfer.
TEST_F(TransferTest, DeserializeTransferFromProtobuf)
{
  // Given
  const AccountId testAccountId = getTestAccountId();
  const int64_t testAmount = getTestAmount();
  proto::AccountAmount testProtoAccountAmount;
  testProtoAccountAmount.set_allocated_accountid(testAccountId.toProtobuf().release());
  testProtoAccountAmount.set_amount(testAmount);
  testProtoAccountAmount.set_is_approval(true);

  // When
  const Transfer transfer = Transfer::fromProtobuf(testProtoAccountAmount);

  // Then
  EXPECT_EQ(transfer.getAccountId(), testAccountId);
  EXPECT_EQ(transfer.getAmount().toTinybars(), testAmount);
  EXPECT_TRUE(transfer.getApproval());
}

// Tests serialization & deserialization of Hedera::Transfer -> proto::AccountAmount -> Hedera::Transfer.
TEST_F(TransferTest, ProtoTransfer)
{
  AccountId accountId(10ULL);
  int64_t amount = 10LL;

  proto::AccountAmount protoAccountAmount;
  protoAccountAmount.set_allocated_accountid(accountId.toProtobuf().release());
  protoAccountAmount.set_amount(amount);
  protoAccountAmount.set_is_approval(true);

  Transfer transfer = Transfer::fromProtobuf(protoAccountAmount);
  EXPECT_EQ(transfer.getAccountId(), accountId);
  EXPECT_EQ(transfer.getAmount().toTinybars(), amount);
  EXPECT_TRUE(transfer.getApproval());

  accountId.setAccountNum(15ULL);
  amount = 15LL;

  transfer.setAccountId(accountId);
  transfer.setAmount(Hbar(amount, HbarUnit::TINYBAR()));
  transfer.setApproved(false);

  const auto protoAccountAmountPtr = std::unique_ptr<proto::AccountAmount>(transfer.toProtobuf());
  EXPECT_EQ(protoAccountAmountPtr->accountid().accountnum(), accountId.getAccountNum());
  EXPECT_EQ(protoAccountAmountPtr->amount(), amount);
  EXPECT_FALSE(protoAccountAmountPtr->is_approval());
}
