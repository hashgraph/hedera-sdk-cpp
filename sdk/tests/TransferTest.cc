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
};

TEST_F(TransferTest, ProtoTransferTest)
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
