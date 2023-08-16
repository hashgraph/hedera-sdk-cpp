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
#include "WrappedTransaction.h"

#include <proto/transaction_body.pb.h>

#include <utility>

namespace Hedera
{
//-----
WrappedTransaction::WrappedTransaction(AnyPossibleTransaction transaction)
  : mTransaction(std::move(transaction))
{
}

//-----
WrappedTransaction::WrappedTransaction(const proto::TransactionBody& transaction)
{
  // clang-format off
  if (transaction.has_cryptoapproveallowance()) mTransaction = AccountAllowanceApproveTransaction(transaction);
  else if (transaction.has_cryptodeleteallowance()) mTransaction = AccountAllowanceDeleteTransaction(transaction);
  else if (transaction.has_cryptocreateaccount()) mTransaction = AccountCreateTransaction(transaction);
  else if (transaction.has_cryptodelete()) mTransaction = AccountDeleteTransaction(transaction);
  else if (transaction.has_cryptoupdateaccount()) mTransaction = AccountUpdateTransaction(transaction);
  else if (transaction.has_contractcreateinstance()) mTransaction = ContractCreateTransaction(transaction);
  else if (transaction.has_contractdeleteinstance()) mTransaction = ContractDeleteTransaction(transaction);
  else if (transaction.has_contractcall()) mTransaction = ContractExecuteTransaction(transaction);
  else if (transaction.has_contractupdateinstance()) mTransaction = ContractUpdateTransaction(transaction);
  else if (transaction.has_ethereumtransaction()) mTransaction = EthereumTransaction(transaction);
  else if (transaction.has_fileappend()) mTransaction = FileAppendTransaction(transaction);
  else if (transaction.has_filecreate()) mTransaction = FileCreateTransaction(transaction);
  else if (transaction.has_filedelete()) mTransaction = FileDeleteTransaction(transaction);
  else if (transaction.has_fileupdate()) mTransaction = FileUpdateTransaction(transaction);
  else if (transaction.has_schedulecreate()) mTransaction = ScheduleCreateTransaction(transaction);
  else if (transaction.has_scheduledelete()) mTransaction = ScheduleDeleteTransaction(transaction);
  else if (transaction.has_tokenassociate()) mTransaction = TokenAssociateTransaction(transaction);
  else if (transaction.has_tokenburn()) mTransaction = TokenBurnTransaction(transaction);
  else if (transaction.has_tokencreation()) mTransaction = TokenCreateTransaction(transaction);
  else if (transaction.has_tokendeletion()) mTransaction = TokenDeleteTransaction(transaction);
  else if (transaction.has_tokendissociate()) mTransaction = TokenDissociateTransaction(transaction);
  else if (transaction.has_token_fee_schedule_update()) mTransaction = TokenFeeScheduleUpdateTransaction(transaction);
  else if (transaction.has_tokenfreeze()) mTransaction = TokenFreezeTransaction(transaction);
  else if (transaction.has_tokengrantkyc()) mTransaction = TokenGrantKycTransaction(transaction);
  else if (transaction.has_tokenmint()) mTransaction = TokenMintTransaction(transaction);
  else if (transaction.has_token_pause()) mTransaction = TokenPauseTransaction(transaction);
  else if (transaction.has_tokenrevokekyc()) mTransaction = TokenRevokeKycTransaction(transaction);
  else if (transaction.has_tokenunfreeze()) mTransaction = TokenUnfreezeTransaction(transaction);
  else if (transaction.has_token_unpause()) mTransaction = TokenUnpauseTransaction(transaction);
  else if (transaction.has_tokenwipe()) mTransaction = TokenWipeTransaction(transaction);
  else if (transaction.has_consensuscreatetopic()) mTransaction = TopicCreateTransaction(transaction);
  else if (transaction.has_consensusdeletetopic()) mTransaction = TopicDeleteTransaction(transaction);
  else if (transaction.has_consensussubmitmessage()) mTransaction = TopicMessageSubmitTransaction(transaction);
  else if (transaction.has_consensusupdatetopic()) mTransaction = TopicUpdateTransaction(transaction);
  else if (transaction.has_cryptotransfer()) mTransaction = TransferTransaction(transaction);
  else
  {
    throw std::invalid_argument("TransactionBody contains no transaction");
  }
  // clang-format on
}

//-----
WrappedTransaction& WrappedTransaction::setTransaction(const AnyPossibleTransaction& transaction)
{
  mTransaction = transaction;
  return *this;
}

} // namespace Hedera
