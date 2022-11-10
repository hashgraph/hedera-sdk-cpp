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
#include "TransactionRecord.h"
#include "AccountId.h"

#include "helper/TimestampConverter.h"

#include <gtest/gtest.h>
#include <proto/transaction_record.pb.h>

#include <chrono>

using namespace Hedera;

class TransactionRecordTest : public ::testing::Test
{
};

TEST_F(TransactionRecordTest, ProtobufTransactionRecord)
{
  const AccountId accountIdTo = AccountId(0ULL, 0ULL, 3ULL);
  const AccountId accountIdFrom = AccountId(0ULL, 0ULL, 4ULL);
  const int64_t transferAmount = 10LL;
  const std::string txHash = "txHash";
  const std::chrono::sys_time<std::chrono::duration<double>> now = std::chrono::system_clock::now();
  const std::string txMemo = "txMemo";
  const uint64_t txFee = 10ULL;

  proto::TransactionRecord protoTransactionRecord;
  protoTransactionRecord.mutable_receipt()->set_allocated_accountid(accountIdFrom.toProtobuf());
  protoTransactionRecord.set_allocated_transactionhash(new std::string(txHash));
  protoTransactionRecord.set_allocated_consensustimestamp(TimestampConverter::toProtobuf(now));
  protoTransactionRecord.set_allocated_transactionid(TransactionId::generate(accountIdFrom).toProtobuf());
  protoTransactionRecord.set_allocated_memo(new std::string(txMemo));
  protoTransactionRecord.set_transactionfee(txFee);

  proto::AccountAmount* aa = protoTransactionRecord.mutable_transferlist()->add_accountamounts();
  aa->set_allocated_accountid(accountIdFrom.toProtobuf());
  aa->set_amount(-transferAmount);

  aa = protoTransactionRecord.mutable_transferlist()->add_accountamounts();
  aa->set_allocated_accountid(accountIdTo.toProtobuf());
  aa->set_amount(transferAmount);

  TransactionRecord txRecord = TransactionRecord::fromProtobuf(protoTransactionRecord);
  EXPECT_TRUE(txRecord.getReceipt().getAccountId().has_value());
  EXPECT_EQ(txRecord.getReceipt().getAccountId().value(), accountIdFrom);
  EXPECT_EQ(txRecord.getTransactionHash(), txHash);
  EXPECT_TRUE(txRecord.getConsensusTimestamp().has_value());
  EXPECT_EQ(txRecord.getConsensusTimestamp().value().time_since_epoch().count(), now.time_since_epoch().count());
  EXPECT_TRUE(txRecord.getTransactionId().has_value());
  EXPECT_EQ(txRecord.getTransactionId().value().getAccountId(), accountIdFrom);
  EXPECT_GE(txRecord.getTransactionId().value().getValidTransactionTime(), now);
  EXPECT_EQ(txRecord.getTransactionMemo(), txMemo);
  EXPECT_EQ(txRecord.getTransactionFee(), txFee);
  EXPECT_FALSE(txRecord.getTransferList().empty());
  EXPECT_EQ(txRecord.getTransferList().at(0).first, accountIdFrom);
  EXPECT_EQ(txRecord.getTransferList().at(0).second.toTinybars(), -transferAmount);
  EXPECT_EQ(txRecord.getTransferList().at(1).first, accountIdTo);
  EXPECT_EQ(txRecord.getTransferList().at(1).second.toTinybars(), transferAmount);
}