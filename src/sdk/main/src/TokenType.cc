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
#include "TokenType.h"

#include <basic_types.pb.h>

namespace Hedera
{
//-----
const std::unordered_map<proto::TokenType, TokenType> gProtobufTokenTypeToTokenType = {
  { proto::TokenType::FUNGIBLE_COMMON,     TokenType::FUNGIBLE_COMMON     },
  { proto::TokenType::NON_FUNGIBLE_UNIQUE, TokenType::NON_FUNGIBLE_UNIQUE }
};

//-----
const std::unordered_map<TokenType, proto::TokenType> gTokenTypeToProtobufTokenType = {
  { TokenType::FUNGIBLE_COMMON,     proto::TokenType::FUNGIBLE_COMMON     },
  { TokenType::NON_FUNGIBLE_UNIQUE, proto::TokenType::NON_FUNGIBLE_UNIQUE }
};

//-----
const std::unordered_map<TokenType, const char*> gTokenTypeToString = {
  { TokenType::FUNGIBLE_COMMON,     "FUNGIBLE_COMMON"     },
  { TokenType::NON_FUNGIBLE_UNIQUE, "NON_FUNGIBLE_UNIQUE" }
};

} // namespace Hedera
