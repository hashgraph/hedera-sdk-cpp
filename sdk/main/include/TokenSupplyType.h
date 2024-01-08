/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_TOKEN_SUPPLY_TYPE_H_
#define HEDERA_SDK_CPP_TOKEN_SUPPLY_TYPE_H_

#include <string_view>
#include <unordered_map>

namespace proto
{
enum TokenSupplyType : int;
}

namespace Hedera
{
/**
 * Possible token supply types (IWA Compatibility). Indicates how many tokens can have during its lifetime.
 */
enum class TokenSupplyType
{
  /**
   * Indicates that tokens have an upper bound of std::numeric_limit<uint64_t>::max().
   */
  INFINITE,
  /**
   * Indicates that tokens have an upper bound of maxSupply, provided on token creation.
   */
  FINITE
};

/**
 * Map of protobuf TokenSupplyTypes to the corresponding TokenSupplyType.
 */
extern const std::unordered_map<proto::TokenSupplyType, TokenSupplyType> gProtobufTokenSupplyTypeToTokenSupplyType;

/**
 * Map of TokenSupplyTypes to the corresponding protobuf TokenSupplyType.
 */
extern const std::unordered_map<TokenSupplyType, proto::TokenSupplyType> gTokenSupplyTypeToProtobufTokenSupplyType;

/**
 * Map of TokenSupplyType to its corresponding string.
 */
[[maybe_unused]] extern const std::unordered_map<TokenSupplyType, const char*> gTokenSupplyTypeToString;

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_SUPPLY_TYPE_H_
