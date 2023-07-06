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
#include "CustomFixedFee.h"

#include <proto/custom_fees.pb.h>

namespace Hedera
{
//-----
CustomFixedFee CustomFixedFee::fromProtobuf(const proto::FixedFee& proto)
{
  CustomFixedFee customFixedFee;
  customFixedFee.mAmount = static_cast<uint64_t>(proto.amount());

  if (proto.has_denominating_token_id())
  {
    customFixedFee.mDenominatingTokenId = TokenId::fromProtobuf(proto.denominating_token_id());
  }

  return customFixedFee;
}

//-----
std::unique_ptr<CustomFee> CustomFixedFee::clone() const
{
  return std::make_unique<CustomFixedFee>(*this);
}

//-----
std::unique_ptr<proto::CustomFee> CustomFixedFee::toProtobuf() const
{
  std::unique_ptr<proto::CustomFee> fee = initProtobuf();
  fee->mutable_fixed_fee()->set_amount(static_cast<int64_t>(mAmount));

  if (mDenominatingTokenId.has_value())
  {
    fee->mutable_fixed_fee()->set_allocated_denominating_token_id(mDenominatingTokenId->toProtobuf().release());
  }

  return fee;
}

//-----
std::unique_ptr<proto::FixedFee> CustomFixedFee::toFixedFeeProtobuf() const
{
  auto fee = std::make_unique<proto::FixedFee>();
  fee->set_amount(static_cast<int64_t>(mAmount));

  if (mDenominatingTokenId.has_value())
  {
    fee->set_allocated_denominating_token_id(mDenominatingTokenId->toProtobuf().release());
  }

  return fee;
}

//-----
CustomFixedFee& CustomFixedFee::setAmount(const uint64_t& amount)
{
  mAmount = amount;
  return *this;
}

//-----
CustomFixedFee& CustomFixedFee::setHbarAmount(const Hbar& amount)
{
  mDenominatingTokenId.reset();
  mAmount = amount.toTinybars();
  return *this;
}

//-----
CustomFixedFee& CustomFixedFee::setDenominatingTokenId(const TokenId& tokenId)
{
  mDenominatingTokenId = tokenId;
  return *this;
}

//-----
CustomFixedFee& CustomFixedFee::setDenominatingTokenToSameToken()
{
  mDenominatingTokenId = TokenId(0ULL, 0ULL, 0ULL);
  return *this;
}

} // namespace Hedera
