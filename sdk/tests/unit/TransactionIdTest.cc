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
#include "impl/Utilities.h"

#include <chrono>
#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class TransactionIdTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mTestAccountId; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestValidStartTime() const
  {
    return mTestValidStartTime;
  }
  [[nodiscard]] inline bool getTestScheduled() const { return mTestScheduled; }
  [[nodiscard]] inline int getTestNonce() const { return mTestNonce; }

private:
  const AccountId mTestAccountId = AccountId(1ULL);
  const std::chrono::system_clock::time_point mTestValidStartTime = std::chrono::system_clock::now();
  const bool mTestScheduled = true;
  const int mTestNonce = 2;
};

//-----
TEST_F(TransactionIdTest, WithValidStart)
{
  // Given / When
  const TransactionId transactionId = TransactionId::withValidStart(getTestAccountId(), getTestValidStartTime());

  // Then
  EXPECT_EQ(transactionId.mAccountId, getTestAccountId());
  EXPECT_EQ(transactionId.mValidTransactionTime, getTestValidStartTime());
}

//-----
TEST_F(TransactionIdTest, GenerateTransactionId)
{
  // Given
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

  // When
  const TransactionId transactionId = TransactionId::generate(getTestAccountId());

  // Then
  EXPECT_EQ(transactionId.mAccountId, getTestAccountId());
  // No real good way to verify here, just checking that the TransactionId validTransactionTime was made after the time
  // taken above.
  EXPECT_GE(transactionId.mValidTransactionTime.time_since_epoch().count(), now.time_since_epoch().count());
}

//-----
TEST_F(TransactionIdTest, FromProtobuf)
{
  // Given
  proto::TransactionID proto;
  proto.set_allocated_accountid(getTestAccountId().toProtobuf().release());
  proto.set_allocated_transactionvalidstart(internal::TimestampConverter::toProtobuf(getTestValidStartTime()));
  proto.set_scheduled(getTestScheduled());
  proto.set_nonce(getTestNonce());

  // When
  const TransactionId transactionId = TransactionId::fromProtobuf(proto);

  // Then
  EXPECT_EQ(transactionId.mAccountId, getTestAccountId());
  EXPECT_EQ(transactionId.mValidTransactionTime, getTestValidStartTime());
  EXPECT_EQ(transactionId.getScheduled(), getTestScheduled());
  EXPECT_EQ(transactionId.getNonce(), getTestNonce());
}

//-----
TEST_F(TransactionIdTest, FromString)
{
  // Given
  const std::chrono::system_clock::duration durationSinceEpoch = getTestValidStartTime().time_since_epoch();
  const std::chrono::seconds secondsSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch);
  const std::string validStartTimeStr =
    std::to_string(secondsSinceEpoch.count()) + '.' +
    std::to_string(
      std::chrono::duration_cast<std::chrono::nanoseconds>(durationSinceEpoch - secondsSinceEpoch).count());

  // When
  const TransactionId transactionId = TransactionId::fromString(
    getTestAccountId().toString() + '@' + validStartTimeStr + (getTestScheduled() ? "?scheduled" : "") +
    (getTestNonce() != 0 ? ('/' + std::to_string(getTestNonce())) : ""));

  // Then
  EXPECT_EQ(transactionId.mAccountId, getTestAccountId());
  EXPECT_EQ(transactionId.mValidTransactionTime, getTestValidStartTime());
  EXPECT_EQ(transactionId.getScheduled(), getTestScheduled());
  EXPECT_EQ(transactionId.getNonce(), getTestNonce());
}

//-----
TEST_F(TransactionIdTest, FromBytes)
{
  // Given
  proto::TransactionID proto;
  proto.set_allocated_accountid(getTestAccountId().toProtobuf().release());
  proto.set_allocated_transactionvalidstart(internal::TimestampConverter::toProtobuf(getTestValidStartTime()));
  proto.set_scheduled(getTestScheduled());
  proto.set_nonce(getTestNonce());

  // When
  const TransactionId transactionId =
    TransactionId::fromBytes(internal::Utilities::stringToByteVector(proto.SerializeAsString()));

  // Then
  EXPECT_EQ(transactionId.mAccountId, getTestAccountId());
  EXPECT_EQ(transactionId.mValidTransactionTime, getTestValidStartTime());
  EXPECT_EQ(transactionId.getScheduled(), getTestScheduled());
  EXPECT_EQ(transactionId.getNonce(), getTestNonce());
}

//-----
TEST_F(TransactionIdTest, ToProtobuf)
{
  // Given
  TransactionId transactionId;
  transactionId.mAccountId = getTestAccountId();
  transactionId.mValidTransactionTime = getTestValidStartTime();
  transactionId.setScheduled(getTestScheduled());
  transactionId.setNonce(getTestNonce());

  // When
  const std::unique_ptr<proto::TransactionID> proto = transactionId.toProtobuf();

  // Then
  EXPECT_EQ(AccountId::fromProtobuf(proto->accountid()), getTestAccountId());
  EXPECT_EQ(internal::TimestampConverter::fromProtobuf(proto->transactionvalidstart()), getTestValidStartTime());
  EXPECT_EQ(proto->scheduled(), getTestScheduled());
  EXPECT_EQ(proto->nonce(), getTestNonce());
}

//-----
TEST_F(TransactionIdTest, ToString)
{
  // Given
  const std::chrono::system_clock::duration durationSinceEpoch = getTestValidStartTime().time_since_epoch();
  const std::chrono::seconds secondsSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch);
  const std::string validStartTimeStr =
    std::to_string(secondsSinceEpoch.count()) + '.' +
    std::to_string(
      std::chrono::duration_cast<std::chrono::nanoseconds>(durationSinceEpoch - secondsSinceEpoch).count());

  TransactionId transactionId;
  transactionId.mAccountId = getTestAccountId();
  transactionId.mValidTransactionTime = getTestValidStartTime();
  transactionId.setScheduled(getTestScheduled());
  transactionId.setNonce(getTestNonce());

  // When
  const std::string str = transactionId.toString();

  // Then
  EXPECT_EQ(str,
            getTestAccountId().toString() + '@' + validStartTimeStr + (getTestScheduled() ? "?scheduled" : "") +
              (getTestNonce() != 0 ? ('/' + std::to_string(getTestNonce())) : ""));
}

//-----
TEST_F(TransactionIdTest, ToBytes)
{
  // Given
  TransactionId transactionId;
  transactionId.mAccountId = getTestAccountId();
  transactionId.mValidTransactionTime = getTestValidStartTime();
  transactionId.setScheduled(getTestScheduled());
  transactionId.setNonce(getTestNonce());

  // When
  const std::vector<std::byte> bytes = transactionId.toBytes();

  // Then
  EXPECT_EQ(bytes, internal::Utilities::stringToByteVector(transactionId.toProtobuf()->SerializeAsString()));
}

//-----
TEST_F(TransactionIdTest, SetGetScheduled)
{
  // Given
  TransactionId transactionId;

  // When
  transactionId.setScheduled(getTestScheduled());

  // Then
  EXPECT_EQ(transactionId.getScheduled(), getTestScheduled());
}

//-----
TEST_F(TransactionIdTest, SetGetNonce)
{
  // Given
  TransactionId transactionId;

  // When
  transactionId.setNonce(getTestNonce());

  // Then
  EXPECT_EQ(transactionId.getNonce(), getTestNonce());
}
