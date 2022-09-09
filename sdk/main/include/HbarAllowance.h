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
#ifndef HBAR_ALLOWANCE_H_
#define HBAR_ALLOWANCE_H_

/**
 * Protobuf forward declarations
 */
namespace proto
{
class CryptoAllowance;
} // namespace proto

/**
 * Hedera forward declarations
 */
namespace Hedera
{
class AccountId;
class Hbar;
} // namespace Hedera

namespace Hedera
{
/**
 * An approved allowance of Hbar transfers for a spender.
 */
class HbarAllowance
{
public:
  HbarAllowance() {}

  HbarAllowance(const InitType<AccountId>& ownerAccountId,
                const AccountId& spenderAccountId,
                const Hbar& amount)
  {
  }

  static HbarAllowance fromProtobuf(const proto::CryptoAllowance& proto)
  {
    return HbarAllowance();
  }
};

} // namespace Hedera

#endif // HBAR_ALLOWANCE_H_