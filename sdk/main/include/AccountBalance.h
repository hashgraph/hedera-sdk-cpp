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
#ifndef ACCOUNT_BALANCE_H_
#define ACCOUNT_BALANCE_H_

#include "Hbar.h"

namespace proto
{
class CryptoGetAccountBalanceResponse;
}

namespace Hedera
{
/**
 * Response when the client sends a node an AccountBalanceQuery.
 */
class AccountBalance
{
public:
  /**
   * Convert a CryptoGetAccountBalance protobuf object to an AccountBalance object.
   *
   * @param proto The CryptoGetAccountBalance protobuf object.
   * @return The converted AccountBalance object.
   */
  static AccountBalance fromProtobuf(const proto::CryptoGetAccountBalanceResponse& proto);

  /**
   * Extract the account balance.
   *
   * @return The account balance.
   */
  inline Hbar getBalance() const { return mBalance; }

private:
  /**
   * The account balance.
   */
  Hbar mBalance;
};

} // namespace Hedera

#endif // ACCOUNT_BALANCE_H_