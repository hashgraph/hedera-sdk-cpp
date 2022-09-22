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
#include "AccountBalance.h"

#include "crypto_get_account_balance.pb.h"

namespace Hedera
{
//-----
AccountBalance::AccountBalance()
  : mBalance(0LL)
{
}

//-----
AccountBalance::AccountBalance(const Hbar& balance)
{
  setBalance(balance);
}

//-----
AccountBalance
AccountBalance::fromProtobuf(
  const proto::CryptoGetAccountBalanceResponse& protobuf)
{
  return AccountBalance(Hbar::fromTinybars(protobuf.balance()));
}

//-----
void
AccountBalance::setBalance(const Hbar& hbars)
{
  if (hbars.toTinybars() < 0)
  {
    // TODO: throw
  }

  mBalance = hbars;
}

} // namespace Hedera
