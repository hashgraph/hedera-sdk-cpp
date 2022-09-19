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

/**
 * Library includes
 */
#include "AccountId.h"

/**
 * STL includes
 */
#include <string>

/**
 * Hedera forward declarations
 */
namespace Hedera
{
template<typename T>
class InitType;

class Client;
} // namespace Hedera

namespace Hedera
{
namespace EntityIdHelper
{
AccountId
fromString(const std::string& id)
{
  return AccountId();
}

AccountId
fromSolidityAddress(const std::string& address)
{
  return AccountId();
}

std::string
toSolidityAddress(const unsigned long long& shard,
                  const unsigned long long& realm,
                  const unsigned long long& num)
{
  return std::string();
}

void
validate(const unsigned long long& shard,
         const unsigned long long& realm,
         const unsigned long long& num,
         const Client& client,
         const InitType<std::string>& checksum)
{
}

std::string
toString(const unsigned long long& shard,
         const unsigned long long& realm,
         const unsigned long long& num)
{
  return std::string();
}

std::string
toStringWithChecksum(const unsigned long long& shard,
                     const unsigned long long& realm,
                     const unsigned long long& num,
                     const Client& client,
                     const InitType<std::string>& checksum)
{
  return std::string();
}

} // namespace EntityIdHelper
} // namespace Hedera

#endif // ENTITY_ID_HELPER_H_