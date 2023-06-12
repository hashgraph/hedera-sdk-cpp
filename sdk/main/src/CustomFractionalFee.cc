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
#include "CustomFractionalFee.h"

#include <proto/custom_fees.pb.h>

namespace Hedera
{
//-----
CustomFractionalFee CustomFractionalFee::fromProtobuf(const proto::FractionalFee& proto)
{
  CustomFractionalFee customFractionalFee;

  if (proto.has_fractional_amount())
  {
    customFractionalFee.mNumerator = proto.fractional_amount().numerator();
    customFractionalFee.mDenominator = proto.fractional_amount().denominator();
  }

  customFractionalFee.mMinAmount = static_cast<uint64_t>(proto.minimum_amount());
  customFractionalFee.mMaxAmount = static_cast<uint64_t>(proto.maximum_amount());
  customFractionalFee.mAssessmentMethod =
    proto.net_of_transfers() ? FeeAssessmentMethod::EXCLUSIVE : FeeAssessmentMethod::INCLUSIVE;

  return customFractionalFee;
}

//-----
std::unique_ptr<CustomFee> CustomFractionalFee::clone() const
{
  return std::make_unique<CustomFractionalFee>(*this);
}

//-----
std::unique_ptr<proto::CustomFee> CustomFractionalFee::toProtobuf() const
{
  std::unique_ptr<proto::CustomFee> fee = initProtobuf();
  fee->mutable_fractional_fee()->mutable_fractional_amount()->set_numerator(mNumerator);
  fee->mutable_fractional_fee()->mutable_fractional_amount()->set_denominator(mDenominator);
  fee->mutable_fractional_fee()->set_minimum_amount(static_cast<int64_t>(mMinAmount));
  fee->mutable_fractional_fee()->set_maximum_amount(static_cast<int64_t>(mMaxAmount));
  fee->mutable_fractional_fee()->set_net_of_transfers(mAssessmentMethod == FeeAssessmentMethod::EXCLUSIVE);
  return fee;
}

//-----
CustomFractionalFee& CustomFractionalFee::setNumerator(const int64_t& numerator)
{
  mNumerator = numerator;
  return *this;
}

//-----
CustomFractionalFee& CustomFractionalFee::setDenominator(const int64_t& denominator)
{
  mDenominator = denominator;
  return *this;
}

//-----
CustomFractionalFee& CustomFractionalFee::setMinimumAmount(const uint64_t& amount)
{
  mMinAmount = amount;
  return *this;
}

//-----
CustomFractionalFee& CustomFractionalFee::setMaximumAmount(const uint64_t& amount)
{
  mMaxAmount = amount;
  return *this;
}

//-----
CustomFractionalFee& CustomFractionalFee::setFeeAssessmentMethod(Hedera::FeeAssessmentMethod method)
{
  mAssessmentMethod = method;
  return *this;
}

} // namespace Hedera
