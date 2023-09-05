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
#include "TransactionId.h"
#include "AccountId.h"
#include "impl/TimestampConverter.h"

#include <chrono>
#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class TransactionIdTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }

private:
  const AccountId mAccountId = AccountId(10ULL);
};

TEST_F(TransactionIdTest, GenerateTransactionId)
{
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  const TransactionId transactionId = TransactionId::generate(getTestAccountId());

  EXPECT_EQ(transactionId.getAccountId(), getTestAccountId());

  // No real good way to verify here, just checking that the TransactionId validTransactionTime was made after the time
  // taken above.
  EXPECT_GE(transactionId.getValidTransactionTime().time_since_epoch().count(), now.time_since_epoch().count());
}

// Tests serialization of Hedera::TransactionId -> proto::TransactionID.
TEST_F(TransactionIdTest, SerializeTransactionIdToProtobuf)
{
  // Given
  const std::string testAccountIdStr = "111.222.333";
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  AccountId testAccountId = AccountId::fromString(testAccountIdStr);
  TransactionId testTransactionId = TransactionId::generate(testAccountId);

  // When
  const auto protoTransactionIdPtr = std::unique_ptr<proto::TransactionID>(testTransactionId.toProtobuf());
  const auto protoTimestampPtr = std::unique_ptr<proto::Timestamp>(internal::TimestampConverter::toProtobuf(now));

  // Then
  EXPECT_EQ(static_cast<uint64_t>(protoTransactionIdPtr->accountid().shardnum()), testAccountId.mShardNum);
  EXPECT_EQ(static_cast<uint64_t>(protoTransactionIdPtr->accountid().realmnum()), testAccountId.mRealmNum);
  EXPECT_EQ(static_cast<uint64_t>(protoTransactionIdPtr->accountid().accountnum()), testAccountId.mAccountNum);
  EXPECT_EQ(protoTransactionIdPtr->transactionvalidstart().seconds(), protoTimestampPtr->seconds());
}

// Tests deserialization of proto::TransactionID -> Hedera::TransactionId.
TEST_F(TransactionIdTest, DeserializeTransactionIdFromProtobuf)
{
  // Given
  const std::string testAccountIdStr = "123.456.789";
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  AccountId testAccountId = AccountId::fromString(testAccountIdStr);
  proto::TransactionID testProtoTransactionId;
  testProtoTransactionId.set_allocated_accountid(testAccountId.toProtobuf().release());
  testProtoTransactionId.set_allocated_transactionvalidstart(internal::TimestampConverter::toProtobuf(now));

  // When
  const TransactionId transactionId = TransactionId::fromProtobuf(testProtoTransactionId);

  // Then
  EXPECT_EQ(transactionId.getAccountId(), testAccountId);
  EXPECT_EQ(transactionId.getValidTransactionTime(), now);
}

// Tests serialization of Hedera::TransactionId -> proto::TransactionID -> Hedera::TransactionId.
TEST_F(TransactionIdTest, ProtobufTransactionId)
{
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

  proto::TransactionID protoTransactionId;
  protoTransactionId.set_allocated_accountid(getTestAccountId().toProtobuf().release());
  protoTransactionId.set_allocated_transactionvalidstart(internal::TimestampConverter::toProtobuf(now));

  const TransactionId transactionId = TransactionId::fromProtobuf(protoTransactionId);
  EXPECT_EQ(transactionId.getAccountId(), getTestAccountId());
  EXPECT_EQ(transactionId.getValidTransactionTime(), now);

  const auto protoTransactionIdPtr = std::unique_ptr<proto::TransactionID>(transactionId.toProtobuf());
  const auto protoTimestampPtr = std::unique_ptr<proto::Timestamp>(internal::TimestampConverter::toProtobuf(now));
  EXPECT_EQ(static_cast<uint64_t>(protoTransactionIdPtr->accountid().shardnum()), getTestAccountId().mShardNum);
  EXPECT_EQ(static_cast<uint64_t>(protoTransactionIdPtr->accountid().realmnum()), getTestAccountId().mRealmNum);
  EXPECT_EQ(static_cast<uint64_t>(protoTransactionIdPtr->accountid().accountnum()), getTestAccountId().mAccountNum);
  EXPECT_EQ(protoTransactionIdPtr->transactionvalidstart().seconds(), protoTimestampPtr->seconds());
  EXPECT_EQ(protoTransactionIdPtr->transactionvalidstart().nanos(), protoTimestampPtr->nanos());
}
