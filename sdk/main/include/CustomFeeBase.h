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
#ifndef HEDERA_SDK_CPP_CUSTOM_FEE_BASE_H_
#define HEDERA_SDK_CPP_CUSTOM_FEE_BASE_H_

#include "AccountId.h"
#include "CustomFee.h"

namespace Hedera
{
/**
 * Helper class to sit between CustomFee and its derived implementations. This is done so that CustomFee isn't required
 * to be a templated type.
 */
template<typename FeeType>
class CustomFeeBase : public CustomFee
{
public:
  virtual ~CustomFeeBase() = default;

  /**
   * Set the ID of the desired fee collector account.
   *
   * @param accountId The ID of the desired fee collector account.
   * @return A reference to this derived CustomFee object, with the newly-set fee collector account ID.
   */
  FeeType& setFeeCollectorAccountId(const AccountId& accountId);

  /**
   * Set the fee collector exemption policy.
   *
   * @param exempt \c TRUE if fee collectors should be exempt from this CustomFee, otherwise \c FALSE.
   * @return A reference ot this derived CustomFee object, with the newly-set fee collector exemption policy.
   */
  FeeType& setAllCollectorsAreExempt(bool exempt);

protected:
  /**
   * Prevent public copying and moving to prevent slicing. Use the 'clone()' virtual method instead.
   */
  CustomFeeBase() = default;
  CustomFeeBase(const CustomFeeBase&) = default;
  CustomFeeBase& operator=(const CustomFeeBase&) = default;
  CustomFeeBase(CustomFeeBase&&) noexcept = default;
  CustomFeeBase& operator=(CustomFeeBase&&) noexcept = default;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CUSTOM_FEE_BASE_H_
