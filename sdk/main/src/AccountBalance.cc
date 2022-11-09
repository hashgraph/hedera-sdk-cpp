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
#include "AccountBalance.h"

#include <proto/crypto_get_account_balance.pb.h>

namespace Hedera
{
//-----
AccountBalance AccountBalance::fromProtobuf(const proto::CryptoGetAccountBalanceResponse& proto)
{
  AccountBalance balance;
  balance.mBalance = Hbar(proto.balance(), HbarUnit::TINYBAR());
  return balance;
}

} // namespace Hedera
