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
  [[nodiscard]] inline const NftId& getTestNftId() const { return mNftId; }
  [[nodiscard]] inline const AccountId& getTestSenderAccountId() const { return mSenderAccountId; }
  [[nodiscard]] inline const AccountId& getTestReceiverAccountId() const { return mReceiverAccountId; }
  [[nodiscard]] inline bool getTestIsApproval() const { return mIsApproval; }

private:
  const NftId mNftId = NftId(TokenId(1ULL), 20ULL);
  const AccountId mSenderAccountId = AccountId(300ULL);
  const AccountId mReceiverAccountId = AccountId(4000ULL);
  const bool mIsApproval = true;
};

//-----
TEST_F(TokenNftTransferTest, DefaultConstruction)
{
  TokenNftTransfer tokenNftTransfer;
  EXPECT_EQ(tokenNftTransfer.getNftId(), NftId());
  EXPECT_EQ(tokenNftTransfer.getSenderAccountId(), AccountId());
  EXPECT_EQ(tokenNftTransfer.getReceiverAccountId(), AccountId());
  EXPECT_FALSE(tokenNftTransfer.getApproval());
}

//-----
TEST_F(TokenNftTransferTest, SetGetNftId)
{
  TokenNftTransfer tokenNftTransfer;
  tokenNftTransfer.setNftId(getTestNftId());
  EXPECT_EQ(tokenNftTransfer.getNftId(), getTestNftId());
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
  tokenNftTransfer.setNftId(getTestNftId());
  tokenNftTransfer.setSenderAccountId(getTestSenderAccountId());
  tokenNftTransfer.setReceiverAccountId(getTestReceiverAccountId());
  tokenNftTransfer.setApproval(getTestIsApproval());

  std::unique_ptr<proto::NftTransfer> protoNftTransfer = tokenNftTransfer.toProtobuf();
  EXPECT_EQ(protoNftTransfer->senderaccountid().accountnum(), getTestSenderAccountId().getAccountNum());
  EXPECT_EQ(protoNftTransfer->receiveraccountid().accountnum(), getTestReceiverAccountId().getAccountNum());
  EXPECT_EQ(protoNftTransfer->serialnumber(), getTestNftId().getSerialNum());
  EXPECT_EQ(protoNftTransfer->is_approval(), getTestIsApproval());

  protoNftTransfer->set_allocated_senderaccountid(getTestReceiverAccountId().toProtobuf().release());
  protoNftTransfer->set_allocated_receiveraccountid(getTestSenderAccountId().toProtobuf().release());
  protoNftTransfer->set_serialnumber(static_cast<int64_t>(getTestNftId().getSerialNum()) - 1LL);
  protoNftTransfer->set_is_approval(!getTestIsApproval());

  tokenNftTransfer = TokenNftTransfer::fromProtobuf(*protoNftTransfer);
  EXPECT_EQ(tokenNftTransfer.getNftId().getSerialNum(), getTestNftId().getSerialNum() - 1ULL);
  EXPECT_EQ(tokenNftTransfer.getSenderAccountId(), getTestReceiverAccountId());
  EXPECT_EQ(tokenNftTransfer.getReceiverAccountId(), getTestSenderAccountId());
  EXPECT_EQ(tokenNftTransfer.getApproval(), !getTestIsApproval());
}
