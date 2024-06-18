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
#ifndef HEDERA_SDK_CPP_TOKEN_KEY_VALIDATION_TYPE_H_
#define HEDERA_SDK_CPP_TOKEN_KEY_VALIDATION_TYPE_H_

#include <string_view>
#include <unordered_map>

namespace proto
{
enum TokenKeyValidation : int;
}

namespace Hedera
{
/**
 * Types of validation strategies for token keys.
 */
enum class TokenKeyValidation
{
  /**
   * Currently the default behaviour. It will perform all token key validations.
   */
  FULL_VALIDATION,

  /**
   * Perform no validations at all for all passed token keys.
   */
  NO_VALIDATION
};

/**
 * Map of protobuf TokenKeyValidation to the corresponding TokenKeyValidation.
 */
extern const std::unordered_map<proto::TokenKeyValidation, TokenKeyValidation>
  gProtobufTokenKeyValidationToTokenKeyValidation;

/**
 * Map of TokenKeyValidation to the corresponding protobuf TokenKeyValidation.
 */
extern const std::unordered_map<TokenKeyValidation, proto::TokenKeyValidation>
  gTokenKeyValidationToProtobufTokenKeyValidation;

/**
 * Map of TokenKeyValidation to its corresponding string.
 */
[[maybe_unused]] extern const std::unordered_map<TokenKeyValidation, const char*> gTokenKeyValidationToString;

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_KEY_VALIDATION_TYPE_H_
