/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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

#include "helper/TimestampConverter.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

#include <chrono>

using namespace Hedera;

class TransactionIdTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }

private:
  const AccountId mAccountId = AccountId(0ULL, 0ULL, 10ULL);
};

TEST_F(TransactionIdTest, GenerateTransactionId)
{
  const std::chrono::sys_time<std::chrono::duration<double>> now = std::chrono::system_clock::now();
  const TransactionId transactionId = TransactionId::generate(getTestAccountId());

  EXPECT_EQ(transactionId.getAccountId(), getTestAccountId());

  // No real good way to verify here, just checking that the TransactionId validTransactionTime was made after the time
  // taken above.
  EXPECT_GE(transactionId.getValidTransactionTime().time_since_epoch().count(), now.time_since_epoch().count());
}

TEST_F(TransactionIdTest, ProtobufTransactionId)
{
  const std::chrono::sys_time<std::chrono::duration<double>> now = std::chrono::system_clock::now();

  proto::TransactionID protoTransactionId;
  protoTransactionId.set_allocated_accountid(getTestAccountId().toProtobuf());
  protoTransactionId.set_allocated_transactionvalidstart(TimestampConverter::toProtobuf(now));

  const TransactionId transactionId = TransactionId::fromProtobuf(protoTransactionId);
  EXPECT_EQ(transactionId.getAccountId(), getTestAccountId());
  EXPECT_EQ(transactionId.getValidTransactionTime(), now);

  const std::unique_ptr<proto::TransactionID> protoTransactionIdPtr =
    std::unique_ptr<proto::TransactionID>(transactionId.toProtobuf());
  const std::unique_ptr<proto::Timestamp> protoTimestampPtr =
    std::unique_ptr<proto::Timestamp>(TimestampConverter::toProtobuf(now));
  EXPECT_EQ(static_cast<uint64_t>(protoTransactionIdPtr->accountid().shardnum()), getTestAccountId().getShardNum());
  EXPECT_EQ(static_cast<uint64_t>(protoTransactionIdPtr->accountid().realmnum()), getTestAccountId().getRealmNum());
  EXPECT_EQ(static_cast<uint64_t>(protoTransactionIdPtr->accountid().accountnum()), getTestAccountId().getAccountNum());
  EXPECT_EQ(protoTransactionIdPtr->transactionvalidstart().seconds(), protoTimestampPtr->seconds());
  EXPECT_EQ(protoTransactionIdPtr->transactionvalidstart().nanos(), protoTimestampPtr->nanos());
}
