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

// Tests serialization of Hedera::ContractId object to a proto::ContractID object.
TEST_F(SerializationAndDeserializationTests, SerializeContractIdТoProtobufTest)
{
  // Given
  const uint64_t testShardNum = getTestShardNum();
  const uint64_t testRealmNum = getTestRealmNum();
  const uint64_t testAccountNum = getTestAccountNum();
  ContractId testContractId = ContractId(testShardNum, testRealmNum, testAccountNum);

  // When
  auto protoContractId = std::unique_ptr<proto::ContractID>(testContractId.toProtobuf());

  // Then
  EXPECT_EQ(protoContractId->shardnum(), testShardNum);
  EXPECT_EQ(protoContractId->realmnum(), testRealmNum);
  EXPECT_EQ(protoContractId->contractnum(), testAccountNum);
}

// Tests deserialization of Hedera::ContractId object from a proto::ContractID object.
TEST_F(SerializationAndDeserializationTests, DeserializeContractIdFromProtobufTest)
{
  // Given
  const uint64_t testShardNum = getTestShardNum();
  const uint64_t testRealmNum = getTestRealmNum();
  const uint64_t testContractNum = getTestContractNum();
  proto::ContractID testProtoContractId = proto::ContractID();
  testProtoContractId.set_shardnum(testShardNum);
  testProtoContractId.set_realmnum(testRealmNum);
  testProtoContractId.set_contractnum(testContractNum);

  // When
  ContractId contractId = ContractId::fromProtobuf(testProtoContractId);
  
  // Then
  EXPECT_EQ(contractId.getShardNum(), testShardNum);
  EXPECT_EQ(contractId.getRealmNum(), testRealmNum);
  EXPECT_EQ(contractId.getContractNum(), testContractNum);
}

// Tests serialization of Hedera::TransactionId object to a proto::TransactionID object.
TEST_F(SerializationAndDeserializationTests, SerializeTransactionIdToProtobufTest)
{
  // Given
  const std::string testAccountIdStr = "111.222.333";
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  AccountId testAccountId(testAccountIdStr);
  TransactionId testTransactionId = TransactionId::generate(testAccountId);

  // When
  const auto protoTransactionIdPtr = std::unique_ptr<proto::TransactionID>(testTransactionId.toProtobuf());
  const auto protoTimestampPtr = std::unique_ptr<proto::Timestamp>(internal::TimestampConverter::toProtobuf(now));

  // Then
  EXPECT_EQ(static_cast<uint64_t>(protoTransactionIdPtr->accountid().shardnum()), testAccountId.getShardNum());
  EXPECT_EQ(static_cast<uint64_t>(protoTransactionIdPtr->accountid().realmnum()), testAccountId.getRealmNum());
  EXPECT_EQ(static_cast<uint64_t>(protoTransactionIdPtr->accountid().accountnum()), testAccountId.getAccountNum());
  EXPECT_EQ(protoTransactionIdPtr->transactionvalidstart().seconds(), protoTimestampPtr->seconds());
}

// Tests deserialization of Hedera::TransactionId object from a proto::TransactionID object.
TEST_F(SerializationAndDeserializationTests, DeserializeTransactionIdFromProtobufTest)
{
  // Given
  const std::string testAccountIdStr = "111.222.333";
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  AccountId testAccountId(testAccountIdStr);
  proto::TransactionID testProtoTransactionId;
  testProtoTransactionId.set_allocated_accountid(testAccountId.toProtobuf().release());
  testProtoTransactionId.set_allocated_transactionvalidstart(internal::TimestampConverter::toProtobuf(now));

  // When
  const TransactionId transactionId = TransactionId::fromProtobuf(testProtoTransactionId);

  // Then
  EXPECT_EQ(transactionId.getAccountId().getAccountNum(), testAccountId.getAccountNum());
  EXPECT_EQ(transactionId.getAccountId().getRealmNum(), testAccountId.getRealmNum());
  EXPECT_EQ(transactionId.getAccountId().getShardNum(), testAccountId.getShardNum());
  EXPECT_EQ(transactionId.getValidTransactionTime(), now);
}

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