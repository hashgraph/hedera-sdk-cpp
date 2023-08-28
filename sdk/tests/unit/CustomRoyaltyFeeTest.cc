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
#include "CustomRoyaltyFee.h"
#include "AccountId.h"
#include "CustomFixedFee.h"
#include "TokenId.h"

#include <gtest/gtest.h>
#include <proto/custom_fees.pb.h>

using namespace Hedera;

class CustomRoyaltyFeeTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const AccountId& getTestFeeCollectorAccountId() const { return mTestFeeCollectorAccountId; }
  [[nodiscard]] inline bool getTestAllCollectorsAreExempt() const { return mTestAllCollectorsAreExempt; }
  [[nodiscard]] inline const int64_t& getTestNumerator() const { return mTestNumerator; }
  [[nodiscard]] inline const int64_t& getTestDenominator() const { return mTestDenominator; }
  [[nodiscard]] inline const CustomFixedFee& getTestFallbackFee() const { return mTestFallbackFee; }

private:
  const AccountId mTestFeeCollectorAccountId = AccountId(1ULL, 2ULL, 3ULL);
  const bool mTestAllCollectorsAreExempt = true;
  const int64_t mTestNumerator = 4ULL;
  const int64_t mTestDenominator = 5ULL;
  CustomFixedFee mTestFallbackFee;
};

//-----
TEST_F(CustomRoyaltyFeeTest, FromProtobuf)
{
  // Given
  proto::RoyaltyFee protoFee;
  protoFee.mutable_exchange_value_fraction()->set_numerator(getTestNumerator());
  protoFee.mutable_exchange_value_fraction()->set_denominator(getTestDenominator());
  protoFee.set_allocated_fallback_fee(getTestFallbackFee().toFixedFeeProtobuf().release());

  // When
  const CustomRoyaltyFee customRoyaltyFee = CustomRoyaltyFee::fromProtobuf(protoFee);

  // Then
  EXPECT_EQ(customRoyaltyFee.getNumerator(), getTestNumerator());
  EXPECT_EQ(customRoyaltyFee.getDenominator(), getTestDenominator());
  EXPECT_TRUE(customRoyaltyFee.getFallbackFee().has_value());
}

//-----
TEST_F(CustomRoyaltyFeeTest, Clone)
{
  // Given
  CustomRoyaltyFee customRoyaltyFee;
  customRoyaltyFee.setFeeCollectorAccountId(getTestFeeCollectorAccountId());
  customRoyaltyFee.setAllCollectorsAreExempt(getTestAllCollectorsAreExempt());

  // When
  const std::unique_ptr<CustomFee> clonedCustomRoyaltyFee = customRoyaltyFee.clone();

  // Then
  EXPECT_EQ(clonedCustomRoyaltyFee->getFeeCollectorAccountId(), getTestFeeCollectorAccountId());
  EXPECT_EQ(clonedCustomRoyaltyFee->getAllCollectorsAreExempt(), getTestAllCollectorsAreExempt());
}

//-----
TEST_F(CustomRoyaltyFeeTest, ToProtobuf)
{
  // Given
  CustomRoyaltyFee customRoyaltyFee;
  customRoyaltyFee.setFeeCollectorAccountId(getTestFeeCollectorAccountId());
  customRoyaltyFee.setAllCollectorsAreExempt(getTestAllCollectorsAreExempt());
  customRoyaltyFee.setNumerator(getTestNumerator());
  customRoyaltyFee.setDenominator(getTestDenominator());
  customRoyaltyFee.setFallbackFee(getTestFallbackFee());

  // When
  const std::unique_ptr<proto::CustomFee> protoCustomFee = customRoyaltyFee.toProtobuf();

  // Then
  EXPECT_EQ(protoCustomFee->fee_collector_account_id().shardnum(), getTestFeeCollectorAccountId().getShardNum());
  EXPECT_EQ(protoCustomFee->fee_collector_account_id().realmnum(), getTestFeeCollectorAccountId().getRealmNum());
  EXPECT_EQ(protoCustomFee->fee_collector_account_id().accountnum(), getTestFeeCollectorAccountId().getAccountNum());
  EXPECT_EQ(protoCustomFee->all_collectors_are_exempt(), getTestAllCollectorsAreExempt());

  ASSERT_EQ(protoCustomFee->fee_case(), proto::CustomFee::FeeCase::kRoyaltyFee);
  EXPECT_EQ(protoCustomFee->royalty_fee().exchange_value_fraction().numerator(), getTestNumerator());
  EXPECT_EQ(protoCustomFee->royalty_fee().exchange_value_fraction().denominator(), getTestDenominator());
  EXPECT_TRUE(protoCustomFee->royalty_fee().has_fallback_fee());
}

//-----
TEST_F(CustomRoyaltyFeeTest, GetSetNumerator)
{
  // Given
  CustomRoyaltyFee customRoyaltyFee;

  // When
  customRoyaltyFee.setNumerator(getTestNumerator());

  // Then
  EXPECT_EQ(customRoyaltyFee.getNumerator(), getTestNumerator());
}

//-----
TEST_F(CustomRoyaltyFeeTest, GetSetDenominator)
{
  // Given
  CustomRoyaltyFee customRoyaltyFee;

  // When
  customRoyaltyFee.setDenominator(getTestDenominator());

  // Then
  EXPECT_EQ(customRoyaltyFee.getDenominator(), getTestDenominator());
}

//-----
TEST_F(CustomRoyaltyFeeTest, CannotSetDenominatorToZero)
{
  // Given
  CustomRoyaltyFee customRoyaltyFee;

  // When / Then
  EXPECT_THROW(customRoyaltyFee.setDenominator(0LL), std::invalid_argument);
}

//-----
TEST_F(CustomRoyaltyFeeTest, GetSetFallbackFee)
{
  // Given
  CustomRoyaltyFee customRoyaltyFee;

  // When
  customRoyaltyFee.setFallbackFee(getTestFallbackFee());

  // Then
  EXPECT_TRUE(customRoyaltyFee.getFallbackFee().has_value());
}
