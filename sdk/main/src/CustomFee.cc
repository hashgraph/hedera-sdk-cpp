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
#include "CustomFee.h"
#include "CustomFixedFee.h"
#include "CustomFractionalFee.h"
#include "CustomRoyaltyFee.h"

#include <proto/custom_fees.pb.h>

namespace Hedera
{
//-----
std::unique_ptr<CustomFee> CustomFee::fromProtobuf(const proto::CustomFee& proto)
{
  switch (proto.fee_case())
  {
    case proto::CustomFee::FeeCase::kFixedFee:
      return std::make_unique<CustomFixedFee>(CustomFixedFee::fromProtobuf(proto.fixed_fee()));
    case proto::CustomFee::FeeCase::kFractionalFee:
      return std::make_unique<CustomFractionalFee>(CustomFractionalFee::fromProtobuf(proto.fractional_fee()));
    case proto::CustomFee::FeeCase::kRoyaltyFee:
      return std::make_unique<CustomRoyaltyFee>(CustomRoyaltyFee::fromProtobuf(proto.royalty_fee()));
    default:
      throw std::invalid_argument("Fee protobuf case not recognized");
  }
}

//-----
std::unique_ptr<proto::CustomFee> CustomFee::initProtobuf() const
{
  auto fee = std::make_unique<proto::CustomFee>();
  fee->set_allocated_fee_collector_account_id(mFeeCollectorAccountId.toProtobuf().release());
  fee->set_all_collectors_are_exempt(mAllCollectorsAreExempt);
  return fee;
}

} // namespace Hedera
