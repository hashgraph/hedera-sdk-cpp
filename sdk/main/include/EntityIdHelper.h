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
#ifndef ENTITY_ID_HELPER_H_
#define ENTITY_ID_HELPER_H_

#include "AccountId.h"

#include <optional>
#include <string>

namespace Hedera
{
class Client;
}

namespace Hedera
{
namespace EntityIdHelper
{
AccountId fromString(const std::string& id)
{
  return AccountId();
}

AccountId fromSolidityAddress(const std::string& address)
{
  return AccountId();
}

std::string toSolidityAddress(const int64_t& shard, const int64_t& realm, const int64_t& num)
{
  return std::string();
}

void validate(const int64_t& shard,
              const int64_t& realm,
              const int64_t& num,
              const Client& client,
              const std::optional<std::string>& checksum)
{
}

std::string toString(const int64_t& shard, const int64_t& realm, const int64_t& num)
{
  return std::string();
}

std::string toStringWithChecksum(const int64_t& shard,
                                 const int64_t& realm,
                                 const int64_t& num,
                                 const Client& client,
                                 const std::optional<std::string>& checksum)
{
  return std::string();
}

} // namespace EntityIdHelper
} // namespace Hedera

#endif // ENTITY_ID_HELPER_H_