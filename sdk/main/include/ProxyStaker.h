/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_PROXY_STAKER_H_
#define HEDERA_SDK_CPP_PROXY_STAKER_H_

#include "AccountId.h"
#include "Hbar.h"

#include <cstdint>

namespace proto
{
class ProxyStaker;
}

namespace Hedera
{
/**
 * Information about a single account that is proxy staking.
 */
class ProxyStaker
{
public:
  ProxyStaker() = default;

  /**
   * Construct with an account ID and an amount.
   *
   * @param accountID The account ID that is staking.
   * @param amount    The amount the account is staking.
   */
  ProxyStaker(AccountId accountId, int64_t amount);

  /**
   * Construct a ProxyStaker object from a ProxyStaker protobuf object.
   *
   * @param proto The ProxyStaker protobuf object from which to construct a ProxyStaker object.
   * @return The constructed ProxyStaker object.
   */
  [[nodiscard]] static ProxyStaker fromProtobuf(const proto::ProxyStaker& proto);

  /**
   * The ID of the account that is proxy staking.
   */
  AccountId mAccountId;

  /**
   * The number of Hbars that are currently proxy staked.
   */
  Hbar mAmount;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_PROXY_STAKER_H_
