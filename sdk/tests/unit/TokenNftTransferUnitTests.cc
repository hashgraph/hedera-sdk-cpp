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
#include "TokenNftTransfer.h"
#include "AccountId.h"

#include <gtest/gtest.h>
#include <proto/basic_types.pb.h>

using namespace Hedera;

class TokenNftTransferUnitTests : public ::testing::Test
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
TEST_F(TokenNftTransferUnitTests, ProtobufTokenNftTransfer)
{
  TokenNftTransfer tokenNftTransfer(
    getTestNftId(), getTestSenderAccountId(), getTestReceiverAccountId(), getTestIsApproval());

  std::unique_ptr<proto::NftTransfer> protoNftTransfer = tokenNftTransfer.toProtobuf();
  EXPECT_EQ(protoNftTransfer->senderaccountid().accountnum(), getTestSenderAccountId().mAccountNum);
  EXPECT_EQ(protoNftTransfer->receiveraccountid().accountnum(), getTestReceiverAccountId().mAccountNum);
  EXPECT_EQ(protoNftTransfer->serialnumber(), getTestNftId().mSerialNum);
  EXPECT_EQ(protoNftTransfer->is_approval(), getTestIsApproval());

  protoNftTransfer->set_allocated_senderaccountid(getTestReceiverAccountId().toProtobuf().release());
  protoNftTransfer->set_allocated_receiveraccountid(getTestSenderAccountId().toProtobuf().release());
  protoNftTransfer->set_serialnumber(static_cast<int64_t>(getTestNftId().mSerialNum) - 1LL);
  protoNftTransfer->set_is_approval(!getTestIsApproval());

  tokenNftTransfer = TokenNftTransfer::fromProtobuf(*protoNftTransfer, getTestNftId().mTokenId);
  EXPECT_EQ(tokenNftTransfer.mNftId.mSerialNum, getTestNftId().mSerialNum - 1ULL);
  EXPECT_EQ(tokenNftTransfer.mSenderAccountId, getTestReceiverAccountId());
  EXPECT_EQ(tokenNftTransfer.mReceiverAccountId, getTestSenderAccountId());
  EXPECT_EQ(tokenNftTransfer.mIsApproval, !getTestIsApproval());
}
