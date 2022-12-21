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
#include "TransactionRecord.h"
#include "AccountId.h"
#include "impl/TimestampConverter.h"

#include <chrono>
#include <gtest/gtest.h>
#include <proto/transaction_record.pb.h>

using namespace Hedera;

class TransactionRecordTest : public ::testing::Test
{
};

TEST_F(TransactionRecordTest, ProtobufTransactionRecord)
{
  const auto accountIdTo = AccountId(3ULL);
  const auto accountIdFrom = AccountId(4ULL);
  const int64_t transferAmount = 10LL;
  const std::string txHash = "txHash";
  const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  const std::string txMemo = "txMemo";
  const uint64_t txFee = 10ULL;

  proto::TransactionRecord protoTransactionRecord;
  protoTransactionRecord.mutable_receipt()->set_allocated_accountid(accountIdFrom.toProtobuf().release());
  protoTransactionRecord.set_allocated_transactionhash(new std::string(txHash));
  protoTransactionRecord.set_allocated_consensustimestamp(internal::TimestampConverter::toProtobuf(now));
  protoTransactionRecord.set_allocated_transactionid(TransactionId::generate(accountIdFrom).toProtobuf().release());
  protoTransactionRecord.set_allocated_memo(new std::string(txMemo));
  protoTransactionRecord.set_transactionfee(txFee);

  proto::AccountAmount* aa = protoTransactionRecord.mutable_transferlist()->add_accountamounts();
  aa->set_allocated_accountid(accountIdFrom.toProtobuf().release());
  aa->set_amount(-transferAmount);

  aa = protoTransactionRecord.mutable_transferlist()->add_accountamounts();
  aa->set_allocated_accountid(accountIdTo.toProtobuf().release());
  aa->set_amount(transferAmount);

  TransactionRecord txRecord = TransactionRecord::fromProtobuf(protoTransactionRecord);
  EXPECT_TRUE(txRecord.getReceipt());
  EXPECT_TRUE(txRecord.getReceipt()->getAccountId());
  EXPECT_EQ(*txRecord.getReceipt()->getAccountId(), accountIdFrom);
  EXPECT_EQ(txRecord.getTransactionHash(), txHash);
  EXPECT_TRUE(txRecord.getConsensusTimestamp());
  EXPECT_EQ(txRecord.getConsensusTimestamp()->time_since_epoch().count(), now.time_since_epoch().count());
  EXPECT_TRUE(txRecord.getTransactionId());
  EXPECT_EQ(txRecord.getTransactionId()->getAccountId(), accountIdFrom);
  EXPECT_GE(txRecord.getTransactionId().value().getValidTransactionTime(), now);
  EXPECT_EQ(txRecord.getTransactionMemo(), txMemo);
  EXPECT_EQ(txRecord.getTransactionFee(), txFee);
  EXPECT_FALSE(txRecord.getTransferList().empty());
  EXPECT_EQ(txRecord.getTransferList().at(0).getAccountId(), accountIdFrom);
  EXPECT_EQ(txRecord.getTransferList().at(0).getAmount().toTinybars(), -transferAmount);
  EXPECT_EQ(txRecord.getTransferList().at(1).getAccountId(), accountIdTo);
  EXPECT_EQ(txRecord.getTransferList().at(1).getAmount().toTinybars(), transferAmount);
}