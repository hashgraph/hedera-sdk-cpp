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
#include "WrappedTransaction.h"
#include "exceptions/UninitializedException.h"

#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
WrappedTransaction::WrappedTransaction(AnyPossibleTransaction transaction)
  : mTransaction(std::move(transaction))
{
}

//-----
WrappedTransaction WrappedTransaction::fromProtobuf(const proto::TransactionBody& proto)
{
  if (proto.has_cryptoapproveallowance())
  {
    return WrappedTransaction(AccountAllowanceApproveTransaction(proto));
  }
  else if (proto.has_cryptodeleteallowance())
  {
    return WrappedTransaction(AccountAllowanceDeleteTransaction(proto));
  }
  else if (proto.has_cryptocreateaccount())
  {
    return WrappedTransaction(AccountCreateTransaction(proto));
  }
  else if (proto.has_cryptodelete())
  {
    return WrappedTransaction(AccountDeleteTransaction(proto));
  }
  else if (proto.has_cryptoupdateaccount())
  {
    return WrappedTransaction(AccountUpdateTransaction(proto));
  }
  else if (proto.has_contractcreateinstance())
  {
    return WrappedTransaction(ContractCreateTransaction(proto));
  }
  else if (proto.has_contractdeleteinstance())
  {
    return WrappedTransaction(ContractDeleteTransaction(proto));
  }
  else if (proto.has_contractcall())
  {
    return WrappedTransaction(ContractExecuteTransaction(proto));
  }
  else if (proto.has_contractupdateinstance())
  {
    return WrappedTransaction(ContractUpdateTransaction(proto));
  }
  else if (proto.has_ethereumtransaction())
  {
    return WrappedTransaction(EthereumTransaction(proto));
  }
  else if (proto.has_fileappend())
  {
    return WrappedTransaction(FileAppendTransaction(proto));
  }
  else if (proto.has_filecreate())
  {
    return WrappedTransaction(FileCreateTransaction(proto));
  }
  else if (proto.has_filedelete())
  {
    return WrappedTransaction(FileDeleteTransaction(proto));
  }
  else if (proto.has_fileupdate())
  {
    return WrappedTransaction(FileUpdateTransaction(proto));
  }
  else if (proto.has_freeze())
  {
    return WrappedTransaction(FreezeTransaction(proto));
  }
  else if (proto.has_schedulecreate())
  {
    return WrappedTransaction(ScheduleCreateTransaction(proto));
  }
  else if (proto.has_scheduledelete())
  {
    return WrappedTransaction(ScheduleDeleteTransaction(proto));
  }
  else if (proto.has_schedulesign())
  {
    return WrappedTransaction(ScheduleSignTransaction(proto));
  }
  else if (proto.has_systemdelete())
  {
    return WrappedTransaction(SystemDeleteTransaction(proto));
  }
  else if (proto.has_tokenassociate())
  {
    return WrappedTransaction(TokenAssociateTransaction(proto));
  }
  else if (proto.has_tokenburn())
  {
    return WrappedTransaction(TokenBurnTransaction(proto));
  }
  else if (proto.has_tokencreation())
  {
    return WrappedTransaction(TokenCreateTransaction(proto));
  }
  else if (proto.has_tokendeletion())
  {
    return WrappedTransaction(TokenDeleteTransaction(proto));
  }
  else if (proto.has_tokendissociate())
  {
    return WrappedTransaction(TokenDissociateTransaction(proto));
  }
  else if (proto.has_token_fee_schedule_update())
  {
    return WrappedTransaction(TokenFeeScheduleUpdateTransaction(proto));
  }
  else if (proto.has_tokenfreeze())
  {
    return WrappedTransaction(TokenFreezeTransaction(proto));
  }
  else if (proto.has_tokengrantkyc())
  {
    return WrappedTransaction(TokenGrantKycTransaction(proto));
  }
  else if (proto.has_tokenmint())
  {
    return WrappedTransaction(TokenMintTransaction(proto));
  }
  else if (proto.has_token_pause())
  {
    return WrappedTransaction(TokenPauseTransaction(proto));
  }
  else if (proto.has_tokenrevokekyc())
  {
    return WrappedTransaction(TokenRevokeKycTransaction(proto));
  }
  else if (proto.has_tokenunfreeze())
  {
    return WrappedTransaction(TokenUnfreezeTransaction(proto));
  }
  else if (proto.has_token_unpause())
  {
    return WrappedTransaction(TokenUnpauseTransaction(proto));
  }
  else if (proto.has_tokenwipe())
  {
    return WrappedTransaction(TokenWipeTransaction(proto));
  }
  else if (proto.has_consensuscreatetopic())
  {
    return WrappedTransaction(TopicCreateTransaction(proto));
  }
  else if (proto.has_consensusdeletetopic())
  {
    return WrappedTransaction(TopicDeleteTransaction(proto));
  }
  else if (proto.has_consensussubmitmessage())
  {
    return WrappedTransaction(TopicMessageSubmitTransaction(proto));
  }
  else if (proto.has_consensusupdatetopic())
  {
    return WrappedTransaction(TopicUpdateTransaction(proto));
  }
  else if (proto.has_cryptotransfer())
  {
    return WrappedTransaction(TransferTransaction(proto));
  }
  else
  {
    throw std::invalid_argument("TransactionBody does not contain a valid Transaction");
  }
}

//-----
WrappedTransaction WrappedTransaction::fromProtobuf(const proto::SchedulableTransactionBody& proto)
{
  proto::TransactionBody txBody;
  txBody.set_memo(proto.memo());
  txBody.set_transactionfee(proto.transactionfee());

  if (proto.has_cryptoapproveallowance())
  {
    *txBody.mutable_cryptoapproveallowance() = proto.cryptoapproveallowance();
    return WrappedTransaction(AccountAllowanceApproveTransaction(txBody));
  }
  else if (proto.has_cryptodeleteallowance())
  {
    *txBody.mutable_cryptodeleteallowance() = proto.cryptodeleteallowance();
    return WrappedTransaction(AccountAllowanceDeleteTransaction(txBody));
  }
  else if (proto.has_cryptocreateaccount())
  {
    *txBody.mutable_cryptocreateaccount() = proto.cryptocreateaccount();
    return WrappedTransaction(AccountCreateTransaction(txBody));
  }
  else if (proto.has_cryptodeleteallowance())
  {
    *txBody.mutable_cryptodelete() = proto.cryptodelete();
    return WrappedTransaction(AccountDeleteTransaction(txBody));
  }
  else if (proto.has_cryptoupdateaccount())
  {
    *txBody.mutable_cryptoupdateaccount() = proto.cryptoupdateaccount();
    return WrappedTransaction(AccountUpdateTransaction(txBody));
  }
  else if (proto.has_contractcreateinstance())
  {
    *txBody.mutable_contractcreateinstance() = proto.contractcreateinstance();
    return WrappedTransaction(ContractCreateTransaction(txBody));
  }
  else if (proto.has_contractdeleteinstance())
  {
    *txBody.mutable_contractdeleteinstance() = proto.contractdeleteinstance();
    return WrappedTransaction(ContractDeleteTransaction(txBody));
  }
  else if (proto.has_contractcall())
  {
    *txBody.mutable_contractcall() = proto.contractcall();
    return WrappedTransaction(ContractExecuteTransaction(txBody));
  }
  else if (proto.has_contractupdateinstance())
  {
    *txBody.mutable_contractupdateinstance() = proto.contractupdateinstance();
    return WrappedTransaction(ContractUpdateTransaction(txBody));
  }
  else if (proto.has_fileappend())
  {
    *txBody.mutable_fileappend() = proto.fileappend();
    return WrappedTransaction(FileAppendTransaction(txBody));
  }
  else if (proto.has_filecreate())
  {
    *txBody.mutable_filecreate() = proto.filecreate();
    return WrappedTransaction(FileCreateTransaction(txBody));
  }
  else if (proto.has_filedelete())
  {
    *txBody.mutable_filedelete() = proto.filedelete();
    return WrappedTransaction(FileDeleteTransaction(txBody));
  }
  else if (proto.has_fileupdate())
  {
    *txBody.mutable_fileupdate() = proto.fileupdate();
    return WrappedTransaction(FileUpdateTransaction(txBody));
  }
  else if (proto.has_freeze())
  {
    *txBody.mutable_freeze() = proto.freeze();
    return WrappedTransaction(FreezeTransaction(txBody));
  }
  else if (proto.has_scheduledelete())
  {
    *txBody.mutable_scheduledelete() = proto.scheduledelete();
    return WrappedTransaction(ScheduleCreateTransaction(txBody));
  }
  else if (proto.has_systemdelete())
  {
    *txBody.mutable_systemdelete() = proto.systemdelete();
    return WrappedTransaction(SystemDeleteTransaction(txBody));
  }
  else if (proto.has_tokenassociate())
  {
    *txBody.mutable_tokenassociate() = proto.tokenassociate();
    return WrappedTransaction(TokenAssociateTransaction(txBody));
  }
  else if (proto.has_tokenburn())
  {
    *txBody.mutable_tokenburn() = proto.tokenburn();
    return WrappedTransaction(TokenBurnTransaction(txBody));
  }
  else if (proto.has_tokencreation())
  {
    *txBody.mutable_tokencreation() = proto.tokencreation();
    return WrappedTransaction(TokenCreateTransaction(txBody));
  }
  else if (proto.has_tokendeletion())
  {
    *txBody.mutable_tokendeletion() = proto.tokendeletion();
    return WrappedTransaction(TokenDeleteTransaction(txBody));
  }
  else if (proto.has_tokendissociate())
  {
    *txBody.mutable_tokendissociate() = proto.tokendissociate();
    return WrappedTransaction(TokenDissociateTransaction(txBody));
  }
  else if (proto.has_token_fee_schedule_update())
  {
    *txBody.mutable_token_fee_schedule_update() = proto.token_fee_schedule_update();
    return WrappedTransaction(TokenFeeScheduleUpdateTransaction(txBody));
  }
  else if (proto.has_tokenfreeze())
  {
    *txBody.mutable_tokenfreeze() = proto.tokenfreeze();
    return WrappedTransaction(TokenFreezeTransaction(txBody));
  }
  else if (proto.has_tokengrantkyc())
  {
    *txBody.mutable_tokengrantkyc() = proto.tokengrantkyc();
    return WrappedTransaction(TokenGrantKycTransaction(txBody));
  }
  else if (proto.has_tokenmint())
  {
    *txBody.mutable_tokenmint() = proto.tokenmint();
    return WrappedTransaction(TokenMintTransaction(txBody));
  }
  else if (proto.has_token_pause())
  {
    *txBody.mutable_token_pause() = proto.token_pause();
    return WrappedTransaction(TokenPauseTransaction(txBody));
  }
  else if (proto.has_tokenrevokekyc())
  {
    *txBody.mutable_tokenrevokekyc() = proto.tokenrevokekyc();
    return WrappedTransaction(TokenRevokeKycTransaction(txBody));
  }
  else if (proto.has_tokenunfreeze())
  {
    *txBody.mutable_tokenunfreeze() = proto.tokenunfreeze();
    return WrappedTransaction(TokenFreezeTransaction(txBody));
  }
  else if (proto.has_token_unpause())
  {
    *txBody.mutable_token_unpause() = proto.token_unpause();
    return WrappedTransaction(TokenUnpauseTransaction(txBody));
  }
  else if (proto.has_tokenwipe())
  {
    *txBody.mutable_tokenwipe() = proto.tokenwipe();
    return WrappedTransaction(TokenWipeTransaction(txBody));
  }
  else if (proto.has_consensuscreatetopic())
  {
    *txBody.mutable_consensuscreatetopic() = proto.consensuscreatetopic();
    return WrappedTransaction(TopicCreateTransaction(txBody));
  }
  else if (proto.has_consensusdeletetopic())
  {
    *txBody.mutable_consensusdeletetopic() = proto.consensusdeletetopic();
    return WrappedTransaction(TopicDeleteTransaction(txBody));
  }
  else if (proto.has_consensussubmitmessage())
  {
    *txBody.mutable_consensussubmitmessage() = proto.consensussubmitmessage();
    return WrappedTransaction(TopicMessageSubmitTransaction(txBody));
  }
  else if (proto.has_consensusupdatetopic())
  {
    *txBody.mutable_consensusupdatetopic() = proto.consensusupdatetopic();
    return WrappedTransaction(TopicUpdateTransaction(txBody));
  }
  else if (proto.has_cryptotransfer())
  {
    *txBody.mutable_cryptotransfer() = proto.cryptotransfer();
    return WrappedTransaction(TransferTransaction(txBody));
  }
  else
  {
    throw std::invalid_argument("TransactionBody contains no transaction");
  }
}

//-----
std::unique_ptr<proto::TransactionBody> WrappedTransaction::toProtobuf() const
{
  switch (getTransactionType())
  {
    case ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<AccountAllowanceApproveTransaction>()->generateTransactionBody(nullptr));
    case ACCOUNT_ALLOWANCE_DELETE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<AccountAllowanceDeleteTransaction>()->generateTransactionBody(nullptr));
    case ACCOUNT_CREATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<AccountCreateTransaction>()->generateTransactionBody(nullptr));
    case ACCOUNT_DELETE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<AccountDeleteTransaction>()->generateTransactionBody(nullptr));
    case ACCOUNT_UPDATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<AccountUpdateTransaction>()->generateTransactionBody(nullptr));
    case CONTRACT_CREATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<ContractCreateTransaction>()->generateTransactionBody(nullptr));
    case CONTRACT_DELETE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<ContractDeleteTransaction>()->generateTransactionBody(nullptr));
    case CONTRACT_EXECUTE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<ContractExecuteTransaction>()->generateTransactionBody(nullptr));
    case CONTRACT_UPDATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<ContractUpdateTransaction>()->generateTransactionBody(nullptr));
    case ETHEREUM_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<EthereumTransaction>()->generateTransactionBody(nullptr));
    case FILE_APPEND_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<FileAppendTransaction>()->generateTransactionBody(nullptr));
    case FILE_CREATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<FileCreateTransaction>()->generateTransactionBody(nullptr));
    case FILE_DELETE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<FileDeleteTransaction>()->generateTransactionBody(nullptr));
    case FILE_UPDATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<FileUpdateTransaction>()->generateTransactionBody(nullptr));
    case FREEZE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<FreezeTransaction>()->generateTransactionBody(nullptr));
    case SCHEDULE_CREATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<ScheduleCreateTransaction>()->generateTransactionBody(nullptr));
    case SCHEDULE_DELETE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<ScheduleDeleteTransaction>()->generateTransactionBody(nullptr));
    case SCHEDULE_SIGN_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<ScheduleSignTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_ASSOCIATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenAssociateTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_BURN_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenBurnTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_CREATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenCreateTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_DELETE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenDeleteTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_DISSOCIATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenDissociateTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_FEE_SCHEDULE_UPDATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenFeeScheduleUpdateTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_FREEZE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenFreezeTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_GRANT_KYC_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenGrantKycTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_MINT_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenMintTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_PAUSE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenPauseTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_REVOKE_KYC_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenRevokeKycTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_UNFREEZE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenUnfreezeTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_UNPAUSE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenUnpauseTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_UPDATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenUpdateTransaction>()->generateTransactionBody(nullptr));
    case TOKEN_WIPE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TokenWipeTransaction>()->generateTransactionBody(nullptr));
    case TOPIC_CREATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TopicCreateTransaction>()->generateTransactionBody(nullptr));
    case TOPIC_DELETE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TopicDeleteTransaction>()->generateTransactionBody(nullptr));
    case TOPIC_MESSAGE_SUBMIT_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TopicMessageSubmitTransaction>()->generateTransactionBody(nullptr));
    case TOPIC_UPDATE_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TopicUpdateTransaction>()->generateTransactionBody(nullptr));
    case TRANSFER_TRANSACTION:
      return std::make_unique<proto::TransactionBody>(
        getTransaction<TransferTransaction>()->generateTransactionBody(nullptr));
    default:
    {
      throw UninitializedException("WrappedTransaction doesn't contain a Transaction");
    }
  }
}

//-----
std::unique_ptr<proto::SchedulableTransactionBody> WrappedTransaction::toSchedulableProtobuf() const
{
  proto::TransactionBody txBody = *toProtobuf();

  auto schedulableTxBody = std::make_unique<proto::SchedulableTransactionBody>();
  schedulableTxBody->set_transactionfee(txBody.transactionfee());
  schedulableTxBody->set_memo(txBody.memo());

  if (txBody.has_cryptoapproveallowance())
  {
    schedulableTxBody->set_allocated_cryptoapproveallowance(txBody.release_cryptoapproveallowance());
  }
  else if (txBody.has_cryptodeleteallowance())
  {
    schedulableTxBody->set_allocated_cryptodeleteallowance(txBody.release_cryptodeleteallowance());
  }
  else if (txBody.has_cryptocreateaccount())
  {
    schedulableTxBody->set_allocated_cryptocreateaccount(txBody.release_cryptocreateaccount());
  }
  else if (txBody.has_cryptodeleteallowance())
  {
    schedulableTxBody->set_allocated_cryptodelete(txBody.release_cryptodelete());
  }
  else if (txBody.has_cryptoupdateaccount())
  {
    schedulableTxBody->set_allocated_cryptoupdateaccount(txBody.release_cryptoupdateaccount());
  }
  else if (txBody.has_contractcreateinstance())
  {
    schedulableTxBody->set_allocated_contractcreateinstance(txBody.release_contractcreateinstance());
  }
  else if (txBody.has_contractdeleteinstance())
  {
    schedulableTxBody->set_allocated_contractdeleteinstance(txBody.release_contractdeleteinstance());
  }
  else if (txBody.has_contractcall())
  {
    schedulableTxBody->set_allocated_contractcall(txBody.release_contractcall());
  }
  else if (txBody.has_contractupdateinstance())
  {
    schedulableTxBody->set_allocated_contractupdateinstance(txBody.release_contractupdateinstance());
  }
  else if (txBody.has_fileappend())
  {
    schedulableTxBody->set_allocated_fileappend(txBody.release_fileappend());
  }
  else if (txBody.has_filecreate())
  {
    schedulableTxBody->set_allocated_filecreate(txBody.release_filecreate());
  }
  else if (txBody.has_filedelete())
  {
    schedulableTxBody->set_allocated_filedelete(txBody.release_filedelete());
  }
  else if (txBody.has_fileupdate())
  {
    schedulableTxBody->set_allocated_fileupdate(txBody.release_fileupdate());
  }
  else if (txBody.has_freeze())
  {
    schedulableTxBody->set_allocated_freeze(txBody.release_freeze());
  }
  else if (txBody.has_scheduledelete())
  {
    schedulableTxBody->set_allocated_scheduledelete(txBody.release_scheduledelete());
  }
  else if (txBody.has_systemdelete())
  {
    schedulableTxBody->set_allocated_systemdelete(txBody.release_systemdelete());
  }
  else if (txBody.has_tokenassociate())
  {
    schedulableTxBody->set_allocated_tokenassociate(txBody.release_tokenassociate());
  }
  else if (txBody.has_tokenburn())
  {
    schedulableTxBody->set_allocated_tokenburn(txBody.release_tokenburn());
  }
  else if (txBody.has_tokencreation())
  {
    schedulableTxBody->set_allocated_tokencreation(txBody.release_tokencreation());
  }
  else if (txBody.has_tokendeletion())
  {
    schedulableTxBody->set_allocated_tokendeletion(txBody.release_tokendeletion());
  }
  else if (txBody.has_tokendissociate())
  {
    schedulableTxBody->set_allocated_tokendissociate(txBody.release_tokendissociate());
  }
  else if (txBody.has_token_fee_schedule_update())
  {
    schedulableTxBody->set_allocated_token_fee_schedule_update(txBody.release_token_fee_schedule_update());
  }
  else if (txBody.has_tokenfreeze())
  {
    schedulableTxBody->set_allocated_tokenfreeze(txBody.release_tokenfreeze());
  }
  else if (txBody.has_tokengrantkyc())
  {
    schedulableTxBody->set_allocated_tokengrantkyc(txBody.release_tokengrantkyc());
  }
  else if (txBody.has_tokenmint())
  {
    schedulableTxBody->set_allocated_tokenmint(txBody.release_tokenmint());
  }
  else if (txBody.has_token_pause())
  {
    schedulableTxBody->set_allocated_token_pause(txBody.release_token_pause());
  }
  else if (txBody.has_tokenrevokekyc())
  {
    schedulableTxBody->set_allocated_tokenrevokekyc(txBody.release_tokenrevokekyc());
  }
  else if (txBody.has_tokenunfreeze())
  {
    schedulableTxBody->set_allocated_tokenunfreeze(txBody.release_tokenunfreeze());
  }
  else if (txBody.has_token_unpause())
  {
    schedulableTxBody->set_allocated_token_unpause(txBody.release_token_unpause());
  }
  else if (txBody.has_tokenwipe())
  {
    schedulableTxBody->set_allocated_tokenwipe(txBody.release_tokenwipe());
  }
  else if (txBody.has_consensuscreatetopic())
  {
    schedulableTxBody->set_allocated_consensuscreatetopic(txBody.release_consensuscreatetopic());
  }
  else if (txBody.has_consensusdeletetopic())
  {
    schedulableTxBody->set_allocated_consensusdeletetopic(txBody.release_consensusdeletetopic());
  }
  else if (txBody.has_consensussubmitmessage())
  {
    schedulableTxBody->set_allocated_consensussubmitmessage(txBody.release_consensussubmitmessage());
  }
  else if (txBody.has_consensusupdatetopic())
  {
    schedulableTxBody->set_allocated_consensusupdatetopic(txBody.release_consensusupdatetopic());
  }
  else if (txBody.has_cryptotransfer())
  {
    schedulableTxBody->set_allocated_cryptotransfer(txBody.release_cryptotransfer());
  }
  else
  {
    throw UninitializedException("WrappedTransaction doesn't contain a valid schedulable Transaction");
  }

  return schedulableTxBody;
}

//-----
WrappedTransaction& WrappedTransaction::setTransaction(const AnyPossibleTransaction& transaction)
{
  mTransaction = transaction;
  return *this;
}

} // namespace Hedera
