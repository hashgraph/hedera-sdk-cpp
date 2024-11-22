// SPDX-License-Identifier: Apache-2.0
#ifndef HIERO_TCK_CPP_TOKEN_SERVICE_H_
#define HIERO_TCK_CPP_TOKEN_SERVICE_H_

#include <nlohmann/json_fwd.hpp>

namespace Hiero::TCK::TokenService
{
/**
 * Forward declarations.
 */
class CreateTokenParams;
struct UnpauseTokenParams;

/**
 * Create a token.
 *
 * @param params The parameters to use to create a token.
 * @return A JSON response containing the created token ID and the status of the token creation.
 */
nlohmann::json createToken(const CreateTokenParams& params);

/**
 * Unpause a token.
 *
 * @param params The parameters to use to unpause a token.
 * @return A JSON response containing the status of the token unpause.
 */
nlohmann::json unpauseToken(const UnpauseTokenParams& params);

} // namespace Hiero::TCK::TokenService

#endif // HIERO_TCK_CPP_TOKEN_SERVICE_H_
