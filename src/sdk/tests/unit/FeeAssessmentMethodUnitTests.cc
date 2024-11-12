/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "FeeAssessmentMethod.h"

#include <gtest/gtest.h>

using namespace Hiero;

class FeeAssessmentMethodUnitTests : public ::testing::Test
{
};

//-----
TEST_F(FeeAssessmentMethodUnitTests, FeeAssessmentMethodToString)
{
  // Given / When / Then
  ASSERT_TRUE(gFeeAssessmentMethodToString.find(FeeAssessmentMethod::INCLUSIVE) != gFeeAssessmentMethodToString.end());
  ASSERT_TRUE(gFeeAssessmentMethodToString.find(FeeAssessmentMethod::EXCLUSIVE) != gFeeAssessmentMethodToString.end());
  EXPECT_EQ(gFeeAssessmentMethodToString.at(FeeAssessmentMethod::INCLUSIVE), "INCLUSIVE");
  EXPECT_EQ(gFeeAssessmentMethodToString.at(FeeAssessmentMethod::EXCLUSIVE), "EXCLUSIVE");
}