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
#include "TokenNftTransfer.h"
#include "AccountId.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class TokenNftTransferTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestSenderAccountId() const { return mSenderAccountId; }
  [[nodiscard]] inline const AccountId& getTestReceiverAccountId() const { return mReceiverAccountId; }
  [[nodiscard]] inline const uint64_t& getTestNftSerialNumber() const { return mNftSerialNumber; }
  [[nodiscard]] inline bool getTestIsApproval() const { return mIsApproval; }

private:
  const AccountId mSenderAccountId = AccountId(10ULL);
  const AccountId mReceiverAccountId = AccountId(200ULL);
  const uint64_t mNftSerialNumber = 3000ULL;
  const bool mIsApproval = true;
};

//-----
TEST_F(TokenNftTransferTest, DefaultConstruction)
{
  TokenNftTransfer tokenNftTransfer;
  EXPECT_EQ(tokenNftTransfer.getSenderAccountId(), AccountId());
  EXPECT_EQ(tokenNftTransfer.getReceiverAccountId(), AccountId());
  EXPECT_EQ(tokenNftTransfer.getNftSerialNumber(), 0ULL);
  EXPECT_FALSE(tokenNftTransfer.getApproval());
}

//-----
TEST_F(TokenNftTransferTest, SetGetSenderAccountId)
{
  TokenNftTransfer tokenNftTransfer;
  tokenNftTransfer.setSenderAccountId(getTestSenderAccountId());
  EXPECT_EQ(tokenNftTransfer.getSenderAccountId(), getTestSenderAccountId());
}

//-----
TEST_F(TokenNftTransferTest, SetGetReceiverAccountId)
{
  TokenNftTransfer tokenNftTransfer;
  tokenNftTransfer.setReceiverAccountId(getTestReceiverAccountId());
  EXPECT_EQ(tokenNftTransfer.getReceiverAccountId(), getTestReceiverAccountId());
}

//-----
TEST_F(TokenNftTransferTest, SetGetNftSerialNumber)
{
  TokenNftTransfer tokenNftTransfer;
  tokenNftTransfer.setNftSerialNumber(getTestNftSerialNumber());
  EXPECT_EQ(tokenNftTransfer.getNftSerialNumber(), getTestNftSerialNumber());
  EXPECT_THROW(tokenNftTransfer.setNftSerialNumber(std::numeric_limits<int64_t>::max() + 1ULL), std::invalid_argument);
}

//-----
TEST_F(TokenNftTransferTest, SetGetApproval)
{
  TokenNftTransfer tokenNftTransfer;
  tokenNftTransfer.setApproval(getTestIsApproval());
  EXPECT_EQ(tokenNftTransfer.getApproval(), getTestIsApproval());
}

//-----
TEST_F(TokenNftTransferTest, ProtobufTokenNftTransfer)
{
  TokenNftTransfer tokenNftTransfer;
  tokenNftTransfer.setSenderAccountId(getTestSenderAccountId());
  tokenNftTransfer.setReceiverAccountId(getTestReceiverAccountId());
  tokenNftTransfer.setNftSerialNumber(getTestNftSerialNumber());
  tokenNftTransfer.setApproval(getTestIsApproval());

  std::unique_ptr<proto::NftTransfer> protoNftTransfer = tokenNftTransfer.toProtobuf();
  EXPECT_EQ(protoNftTransfer->senderaccountid().accountnum(), getTestSenderAccountId().getAccountNum());
  EXPECT_EQ(protoNftTransfer->receiveraccountid().accountnum(), getTestReceiverAccountId().getAccountNum());
  EXPECT_EQ(protoNftTransfer->serialnumber(), getTestNftSerialNumber());
  EXPECT_EQ(protoNftTransfer->is_approval(), getTestIsApproval());

  protoNftTransfer->set_allocated_senderaccountid(getTestReceiverAccountId().toProtobuf().release());
  protoNftTransfer->set_allocated_receiveraccountid(getTestSenderAccountId().toProtobuf().release());
  protoNftTransfer->set_serialnumber(getTestNftSerialNumber() - 1ULL);
  protoNftTransfer->set_is_approval(!getTestIsApproval());

  tokenNftTransfer = TokenNftTransfer::fromProtobuf(*protoNftTransfer);
  EXPECT_EQ(tokenNftTransfer.getSenderAccountId(), getTestReceiverAccountId());
  EXPECT_EQ(tokenNftTransfer.getReceiverAccountId(), getTestSenderAccountId());
  EXPECT_EQ(tokenNftTransfer.getNftSerialNumber(), getTestNftSerialNumber() - 1ULL);
  EXPECT_EQ(tokenNftTransfer.getApproval(), !getTestIsApproval());
}
