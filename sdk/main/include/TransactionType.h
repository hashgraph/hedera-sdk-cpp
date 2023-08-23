/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_TRANSACTION_TYPE_H_
#define HEDERA_SDK_CPP_TRANSACTION_TYPE_H_

namespace Hedera
{
/**
 * Enumeration of all transaction types.
 */
enum TransactionType : int
{
  UNKNOWN_TRANSACTION = -1,
  ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION,
  ACCOUNT_ALLOWANCE_DELETE_TRANSACTION,
  ACCOUNT_CREATE_TRANSACTION,
  ACCOUNT_DELETE_TRANSACTION,
  ACCOUNT_UPDATE_TRANSACTION,
  CONTRACT_CREATE_TRANSACTION,
  CONTRACT_DELETE_TRANSACTION,
  CONTRACT_EXECUTE_TRANSACTION,
  CONTRACT_UPDATE_TRANSACTION,
  ETHEREUM_TRANSACTION,
  FILE_APPEND_TRANSACTION,
  FILE_CREATE_TRANSACTION,
  FILE_DELETE_TRANSACTION,
  FILE_UPDATE_TRANSACTION,
  SCHEDULE_CREATE_TRANSACTION,
  SCHEDULE_DELETE_TRANSACTION,
  SCHEDULE_SIGN_TRANSACTION,
  TOKEN_ASSOCIATE_TRANSACTION,
  TOKEN_BURN_TRANSACTION,
  TOKEN_CREATE_TRANSACTION,
  TOKEN_DELETE_TRANSACTION,
  TOKEN_DISSOCIATE_TRANSACTION,
  TOKEN_FEE_SCHEDULE_UPDATE_TRANSACTION,
  TOKEN_FREEZE_TRANSACTION,
  TOKEN_GRANT_KYC_TRANSACTION,
  TOKEN_MINT_TRANSACTION,
  TOKEN_PAUSE_TRANSACTION,
  TOKEN_REVOKE_KYC_TRANSACTION,
  TOKEN_UNFREEZE_TRANSACTION,
  TOKEN_UNPAUSE_TRANSACTION,
  TOKEN_UPDATE_TRANSACTION,
  TOKEN_WIPE_TRANSACTION,
  TOPIC_CREATE_TRANSACTION,
  TOPIC_DELETE_TRANSACTION,
  TOPIC_MESSAGE_SUBMIT_TRANSACTION,
  TOPIC_UPDATE_TRANSACTION,
  TRANSFER_TRANSACTION
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_TYPE_H_
