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
#ifndef ACCOUNT_BALANCE_H_
#define ACCOUNT_BALANCE_H_

/**
 * Libary includes
 */
#include "Hbar.h"

/**
 * Protobuf forward declarations
 */
namespace proto
{
class CryptoGetAccountBalanceResponse;
} // namespace proto

namespace Hedera
{
/**
 * This class represents the account balance object
 */
class AccountBalance
{
public:
  /**
   * Default constructor. Defaults to a balance of 0.
   */
  AccountBalance();

  /**
   * Construct with a balance.
   *
   * @param balance The balance with which to initialize.
   */
  AccountBalance(const Hbar& balance);

  /**
   * Convert the protobuf object to an AccountBalance object.
   *
   * @param proto The protobuf response object.
   * @return      The converted account balance object.
   */
  static AccountBalance fromProtobuf(
    const proto::CryptoGetAccountBalanceResponse& protobuf);

  /**
   * Convert this AccountBalance object into a protobuf object.
   *
   * @return The protobuf object.
   */
  proto::CryptoGetAccountBalanceResponse toProtobuf() const;

  /**
   * Set the account balance.
   *
   * @param hbars The new amount of Hbars with which to set the account balance.
   */
  inline void setBalance(const Hbar& hbars)
  {
    if (hbars.toTinybars() < 0)
    {
      // TODO: throw
    }

    mBalance = hbars;
  }

  /**
   * Get the account balance.
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