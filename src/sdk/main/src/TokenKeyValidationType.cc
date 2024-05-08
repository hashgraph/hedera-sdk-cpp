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
#include "TokenKeyValidationType.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
const std::unordered_map<proto::TokenKeyValidationType, TokenKeyValidationType>
  gProtobufTokenTypeToTokenKeyValidationType = {
    { proto::TokenKeyValidationType::FULL_VALIDATION, TokenKeyValidationType::FULL_VALIDATION },
    { proto::TokenKeyValidationType::NO_VALIDATION,   TokenKeyValidationType::NO_VALIDATION   }
};

//-----
const std::unordered_map<TokenKeyValidationType, proto::TokenKeyValidationType>
  gTokenKeyValidationTypeToProtobufTokenKeyValidationType = {
    { TokenKeyValidationType::FULL_VALIDATION, proto::TokenKeyValidationType::FULL_VALIDATION },
    { TokenKeyValidationType::NO_VALIDATION,   proto::TokenKeyValidationType::NO_VALIDATION   }
};

//-----
const std::unordered_map<TokenKeyValidationType, const char*> gTokenKeyValidationTypeToString = {
  { TokenKeyValidationType::FULL_VALIDATION, "FULL_VALIDATION" },
  { TokenKeyValidationType::NO_VALIDATION,   "NO_VALIDATION"   }
};
} // namespace Hedera