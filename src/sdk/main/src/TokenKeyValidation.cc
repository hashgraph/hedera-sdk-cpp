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
#include "TokenKeyValidation.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
const std::unordered_map<proto::TokenKeyValidation, TokenKeyValidation>
  gProtobufTokenKeyValidationToTokenKeyValidation = {
    {proto::TokenKeyValidation::FULL_VALIDATION, TokenKeyValidation::FULL_VALIDATION},
    { proto::TokenKeyValidation::NO_VALIDATION,  TokenKeyValidation::NO_VALIDATION  }
};

//-----
const std::unordered_map<TokenKeyValidation, proto::TokenKeyValidation>
  gTokenKeyValidationToProtobufTokenKeyValidation = {
    {TokenKeyValidation::FULL_VALIDATION, proto::TokenKeyValidation::FULL_VALIDATION},
    { TokenKeyValidation::NO_VALIDATION,  proto::TokenKeyValidation::NO_VALIDATION  }
};

//-----
const std::unordered_map<TokenKeyValidation, const char*> gTokenKeyValidationToString = {
  {TokenKeyValidation::FULL_VALIDATION, "FULL_VALIDATION"},
  { TokenKeyValidation::NO_VALIDATION,  "NO_VALIDATION"  }
};
} // namespace Hedera