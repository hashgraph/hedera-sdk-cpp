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
#ifndef HEDERA_TCK_CPP_TOKEN_SERVICE_H_
#define HEDERA_TCK_CPP_TOKEN_SERVICE_H_

#include <nlohmann/json_fwd.hpp>

namespace Hedera::TCK::TokenService
{
/**
 * Forward declarations.
 */
class CreateTokenParams;
class UpdateTokenParams;

/**
 * Create a token.
 *
 * @param params The parameters to use to create a token.
 * @return A JSON response containing the created token ID and the status of the token creation.
 */
nlohmann::json createToken(const CreateTokenParams& params);

/**
 * Update a token.
 *
 * @param params The parameters to use to update a token.
 * @return A JSON response containing the status of the token update.
 */
nlohmann::json updateToken(const UpdateTokenParams& params);

} // namespace Hedera::TCK::TokenService

#endif // HEDERA_TCK_CPP_TOKEN_SERVICE_H_
