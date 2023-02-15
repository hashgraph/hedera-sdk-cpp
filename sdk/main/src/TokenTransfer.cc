/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#include "TokenTransfer.h"

namespace Hedera
{
//-----
TokenTransfer& TokenTransfer::setTokenId(const TokenId& tokenId)
{
  mTokenId = tokenId;
  return *this;
}

//-----
TokenTransfer& TokenTransfer::setAccountId(const AccountId& accountId)
{
  mAccountId = accountId;
  return *this;
}

//-----
TokenTransfer& TokenTransfer::setAmount(const int64_t& amount)
{
  mAmount = amount;
  return *this;
}

//-----
TokenTransfer& TokenTransfer::setExpectedDecimals(uint32_t decimals)
{
  mExpectedDecimals = decimals;
  return *this;
}

//-----
TokenTransfer& TokenTransfer::setApproval(bool approval)
{
  mIsApproval = approval;
  return *this;
}

} // namespace Hedera
