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
#include "ContractId.h"
#include "ExchangeRate.h"
#include "ExchangeRateSet.h"
#include "Hbar.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"
#include "Transfer.h"

#include "impl/TimestampConverter.h"

#include <chrono>
#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>
#include <proto/exchange_rate.pb.h>

using namespace Hedera;

class SerializationAndDeserializationTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const uint64_t getTestShardNum() const { return mShardNum; }
  [[nodiscard]] inline const uint64_t getTestRealmNum() const { return mRealmNum; }
  [[nodiscard]] inline const uint64_t getTestAccountNum() const { return mAccountNum; }
  [[nodiscard]] inline const uint64_t getTestContractNum() const { return mContractNum; }
  [[nodiscard]] inline const int64_t getTestAmount() const { return mAmount; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }

private:
  const uint64_t mShardNum = 111;
  const uint64_t mRealmNum = 222;
  const uint64_t mAccountNum = 333;
  const uint64_t mContractNum = 444;
  const int64_t mAmount = 10LL;
  const AccountId mAccountId = AccountId(10ULL);
  const std::chrono::system_clock::time_point mValidStart = std::chrono::system_clock::time_point();
};

// Tests serialization of Hedera::Transfer object to a proto::AccountAmount object.
TEST_F(SerializationAndDeserializationTests, SerializeTransferToProtobufTest)
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

// Tests deserialization of Hedera::Transfer object from a proto::AccountAmount object.
TEST_F(SerializationAndDeserializationTests, DeserializeTransferFromProtobufTest)
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