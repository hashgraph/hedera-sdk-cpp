// SPDX-License-Identifier: Apache-2.0
#ifndef HIERO_TCK_CPP_ACCOUNT_SERVICE_H_
#define HIERO_TCK_CPP_ACCOUNT_SERVICE_H_

#include <nlohmann/json_fwd.hpp>

namespace Hiero::TCK::AccountService
{
/**
 * Forward declarations.
 */
class CreateAccountParams;
class DeleteAccountParams;
class UpdateAccountParams;

/**
 * Create an account.
 *
 * @param params The parameters to use to create an account.
 * @return A JSON response containing the created account ID and the status of the account creation.
 */
nlohmann::json createAccount(const CreateAccountParams& params);

/**
 * Delete an account.
 *
 * @param params The parameters to use to delete an account.
 * @return A JSON response containing the status of the account deletion.
 */
nlohmann::json deleteAccount(const DeleteAccountParams& params);

/**
 * Update an account.
 *
 * @param params The parameters to use to update an account.
 * @return A JSON response containing the status of the account update.
 */
nlohmann::json updateAccount(const UpdateAccountParams& params);

} // namespace Hiero::TCK::AccountService

#endif // HIERO_TCK_CPP_ACCOUNT_SERVICE_H_
