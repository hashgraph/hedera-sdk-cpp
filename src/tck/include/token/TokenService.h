// SPDX-License-Identifier: Apache-2.0
#ifndef HIERO_TCK_CPP_TOKEN_SERVICE_H_
#define HIERO_TCK_CPP_TOKEN_SERVICE_H_

#include <nlohmann/json_fwd.hpp>

namespace Hiero::TCK::TokenService
{
/**
 * Forward declarations.
 */
struct AssociateTokenParams;
struct CreateTokenParams;
struct DeleteTokenParams;
struct DissociateTokenParams;
struct FreezeTokenParams;
struct UnfreezeTokenParams;
struct UpdateTokenParams;

/**
 * Associate an account with tokens.
 *
 * @param params The parameters to use to associate the account and tokens.
 * @return A JSON response containing the status of the token association.
 */
nlohmann::json associateToken(const AssociateTokenParams& params);

/**
 * Create a token.
 *
 * @param params The parameters to use to create a token.
 * @return A JSON response containing the created token ID and the status of the token creation.
 */
nlohmann::json createToken(const CreateTokenParams& params);

/**
 * Delete a token.
 *
 * @param params The parameters to use to delete a token.
 * @return A JSON response containing the status of the token deletion.
 */
nlohmann::json deleteToken(const DeleteTokenParams& params);

/**
 * Dissociate an account from tokens.
 *
 * @param params The parameters to use to dissociate the account.
 * @ return A JSON response containing the status of the account dissociation.
 */
nlohmann::json dissociateToken(const DissociateTokenParams& params);

/**
 * Freeze a token on an account.
 *
 * @params The parameters to use to freeze a token.
 * @return A JSON response containing the status of the token freeze.
 */
nlohmann::json freezeToken(const FreezeTokenParams& params);

/**
 * Unfreeze a token from an account.
 *
 * @params The parameters to use to unfreeze a token.
 * @return A JSON response containing the status of the token unfreeze.
 */
nlohmann::json unfreezeToken(const UnfreezeTokenParams& params);

/**
 * Update a token.
 *
 * @param params The parameters to use to update a token.
 * @return A JSON response containing the status of the token update.
 */
nlohmann::json updateToken(const UpdateTokenParams& params);

} // namespace Hiero::TCK::TokenService

#endif // HIERO_TCK_CPP_TOKEN_SERVICE_H_
