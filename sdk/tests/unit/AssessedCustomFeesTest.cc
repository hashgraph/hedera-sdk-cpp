/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "AssessedCustomFee.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/custom_fees.pb.h>

using namespace Hedera;

class AssessedCustomFeeTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const int64_t& getTestAmount() const { return mTestAmount; }
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTestTokenId; }
  [[nodiscard]] inline const AccountId& getTestFeeCollectorAccountId() const { return mTestFeeCollectorAccountId; }
  [[nodiscard]] inline const std::vector<AccountId>& getTestPayerAccountIdList() const
  {
    return mTestPayerAccountIdList;
  }

private:
  const int64_t mTestAmount = 1LL;
  const TokenId mTestTokenId = TokenId(2ULL, 3ULL, 4ULL);
  const AccountId mTestFeeCollectorAccountId = AccountId(5ULL, 6ULL, 7ULL);
  const std::vector<AccountId> mTestPayerAccountIdList = { AccountId(8ULL, 9ULL, 10ULL),
                                                           AccountId(11ULL, 12ULL, 13ULL),
                                                           AccountId(14ULL, 15ULL, 16ULL) };
};

//-----
TEST_F(AssessedCustomFeeTest, FromProtobuf)
{
  // Given
  proto::AssessedCustomFee protoAssessedCustomFee;
  protoAssessedCustomFee.set_amount(getTestAmount());
  protoAssessedCustomFee.set_allocated_token_id(getTestTokenId().toProtobuf().release());
  protoAssessedCustomFee.set_allocated_fee_collector_account_id(getTestFeeCollectorAccountId().toProtobuf().release());

  for (const auto& account : getTestPayerAccountIdList())
  {
    *protoAssessedCustomFee.add_effective_payer_account_id() = *account.toProtobuf();
  }

  // When
  const AssessedCustomFee assessedCustomFee = AssessedCustomFee::fromProtobuf(protoAssessedCustomFee);

  // Then
  EXPECT_EQ(assessedCustomFee.mAmount, getTestAmount());
  ASSERT_TRUE(assessedCustomFee.mTokenId.has_value());
  EXPECT_EQ(assessedCustomFee.mTokenId, getTestTokenId());
  EXPECT_EQ(assessedCustomFee.mFeeCollectorAccountId, getTestFeeCollectorAccountId());
  EXPECT_EQ(assessedCustomFee.mPayerAccountIdList.size(), getTestPayerAccountIdList().size());
}

//-----
TEST_F(AssessedCustomFeeTest, ToProtobuf)
{
  // Given
  AssessedCustomFee assessedCustomFee;
  assessedCustomFee.mAmount = getTestAmount();
  assessedCustomFee.mTokenId = getTestTokenId();
  assessedCustomFee.mFeeCollectorAccountId = getTestFeeCollectorAccountId();
  assessedCustomFee.mPayerAccountIdList = getTestPayerAccountIdList();

  // When
  const std::unique_ptr<proto::AssessedCustomFee> protoAssessedCustomFee = assessedCustomFee.toProtobuf();

  // Then
  EXPECT_EQ(protoAssessedCustomFee->amount(), getTestAmount());
  ASSERT_TRUE(protoAssessedCustomFee->has_token_id());
  EXPECT_EQ(protoAssessedCustomFee->token_id().shardnum(), getTestTokenId().getShardNum());
  EXPECT_EQ(protoAssessedCustomFee->token_id().realmnum(), getTestTokenId().getRealmNum());
  EXPECT_EQ(protoAssessedCustomFee->token_id().tokennum(), getTestTokenId().getTokenNum());
  ASSERT_TRUE(protoAssessedCustomFee->has_fee_collector_account_id());
  EXPECT_EQ(protoAssessedCustomFee->fee_collector_account_id().shardnum(), getTestFeeCollectorAccountId().mShardNum);
  EXPECT_EQ(protoAssessedCustomFee->fee_collector_account_id().realmnum(), getTestFeeCollectorAccountId().mRealmNum);
  EXPECT_EQ(protoAssessedCustomFee->fee_collector_account_id().accountnum(),
            getTestFeeCollectorAccountId().mAccountNum);
  EXPECT_EQ(protoAssessedCustomFee->effective_payer_account_id_size(), getTestPayerAccountIdList().size());
}

//-----
TEST_F(AssessedCustomFeeTest, FromBytes)
{
  // Given
  proto::AssessedCustomFee protoAssessedCustomFee;
  protoAssessedCustomFee.set_amount(getTestAmount());
  protoAssessedCustomFee.set_allocated_token_id(getTestTokenId().toProtobuf().release());
  protoAssessedCustomFee.set_allocated_fee_collector_account_id(getTestFeeCollectorAccountId().toProtobuf().release());

  for (const auto& account : getTestPayerAccountIdList())
  {
    *protoAssessedCustomFee.add_effective_payer_account_id() = *account.toProtobuf();
  }

  // When
  const AssessedCustomFee assessedCustomFee =
    AssessedCustomFee::fromBytes(internal::Utilities::stringToByteVector(protoAssessedCustomFee.SerializeAsString()));

  // Then
  EXPECT_EQ(assessedCustomFee.mAmount, getTestAmount());
  ASSERT_TRUE(assessedCustomFee.mTokenId.has_value());
  EXPECT_EQ(assessedCustomFee.mTokenId, getTestTokenId());
  EXPECT_EQ(assessedCustomFee.mFeeCollectorAccountId, getTestFeeCollectorAccountId());
  EXPECT_EQ(assessedCustomFee.mPayerAccountIdList.size(), getTestPayerAccountIdList().size());
}

//-----
TEST_F(AssessedCustomFeeTest, ToBytes)
{
  // Given
  AssessedCustomFee assessedCustomFee;
  assessedCustomFee.mAmount = getTestAmount();
  assessedCustomFee.mTokenId = getTestTokenId();
  assessedCustomFee.mFeeCollectorAccountId = getTestFeeCollectorAccountId();
  assessedCustomFee.mPayerAccountIdList = getTestPayerAccountIdList();

  // When
  const std::vector<std::byte> bytes = assessedCustomFee.toBytes();

  // Then
  EXPECT_EQ(bytes, internal::Utilities::stringToByteVector(assessedCustomFee.toProtobuf()->SerializeAsString()));
}
