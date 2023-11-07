/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "TokenSupplyType.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
const std::unordered_map<proto::TokenSupplyType, TokenSupplyType> gProtobufTokenSupplyTypeToTokenSupplyType = {
  {proto::TokenSupplyType::INFINITE, TokenSupplyType::INFINITE},
  { proto::TokenSupplyType::FINITE,  TokenSupplyType::FINITE  }
};

//-----
const std::unordered_map<TokenSupplyType, proto::TokenSupplyType> gTokenSupplyTypeToProtobufTokenSupplyType = {
  {TokenSupplyType::INFINITE, proto::TokenSupplyType::INFINITE},
  { TokenSupplyType::FINITE,  proto::TokenSupplyType::FINITE  }
};

//-----
const std::unordered_map<TokenSupplyType, const char*> gTokenSupplyTypeToString = {
  {TokenSupplyType::INFINITE, "INFINITE"},
  { TokenSupplyType::FINITE,  "FINITE"  }
};

} // namespace Hedera
