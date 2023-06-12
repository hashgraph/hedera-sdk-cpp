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
#include "CustomFeeBase.h"

#include <proto/custom_fees.pb.h>

namespace Hedera
{
//-----
template<typename FeeType>
FeeType& CustomFeeBase<FeeType>::setFeeCollectorAccountId(const AccountId& accountId)
{
  mFeeCollectorAccountId = accountId;
  return static_cast<FeeType&>(*this);
}

//-----
template<typename FeeType>
FeeType& CustomFeeBase<FeeType>::setAllCollectorsAreExempt(bool exempt)
{
  mAllCollectorsAreExempt = exempt;
  return static_cast<FeeType&>(*this);
}

//-----
template<typename FeeType>
std::unique_ptr<proto::CustomFee> CustomFeeBase<FeeType>::initProtobuf() const
{
  auto fee = std::make_unique<proto::CustomFee>();
  fee->set_allocated_fee_collector_account_id(mFeeCollectorAccountId.toProtobuf().release());
  fee->set_all_collectors_are_exempt(mAllCollectorsAreExempt);
  return fee;
}

} // namespace Hedera
