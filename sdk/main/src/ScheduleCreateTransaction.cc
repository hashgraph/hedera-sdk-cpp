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
#include "ScheduleCreateTransaction.h"
#include "WrappedTransaction.h"
#include "exceptions/UninitializedException.h"
#include "exceptions/UnsupportedOperationException.h"
#include "impl/Node.h"
#include "impl/TimestampConverter.h"

#include <grpcpp/client_context.h>
#include <proto/schedulable_transaction_body.pb.h>
#include <proto/schedule_create.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
struct ScheduleCreateTransaction::ScheduleCreateTransactionImpl
{
  // The transaction to schedule.
  proto::SchedulableTransactionBody mTransactionToSchedule;

  // The memo of the schedule entity.
  std::string mMemo;

  // The key which can be used to delete the new schedule.
  std::shared_ptr<Key> mAdminKey;

  // The ID of the account to be charged the fee for the scheduled transaction at the consensus time that it executes.
  // Defaults to the payer of this ScheduleCreateTransaction if not provided.
  std::optional<AccountId> mPayerAccountId;

  // The timestamp that the new schedule should expire.
  std::optional<std::chrono::system_clock::time_point> mExpirationTime;

  // Should the scheduled transaction be evaluated at expiration time, or when all required signatures are received?
  bool mWaitForExpiration = false;
};

//-----
ScheduleCreateTransaction::ScheduleCreateTransaction()
{
  initImpl();
}

//-----
ScheduleCreateTransaction::~ScheduleCreateTransaction() = default;

//-----
ScheduleCreateTransaction::ScheduleCreateTransaction(const ScheduleCreateTransaction& other)
  : mImpl(std::make_unique<ScheduleCreateTransactionImpl>(*other.mImpl))
{
}

//-----
ScheduleCreateTransaction& ScheduleCreateTransaction::operator=(const ScheduleCreateTransaction& other)
{
  if (this != &other)
  {
    mImpl = std::make_unique<ScheduleCreateTransactionImpl>(*other.mImpl);
  }

  return *this;
}

//-----
ScheduleCreateTransaction::ScheduleCreateTransaction(ScheduleCreateTransaction&& other) noexcept
  : mImpl(std::move(other.mImpl))
{
  // Leave the moved-from ScheduleCreateTransaction in a valid state.
  other.mImpl = std::make_unique<ScheduleCreateTransactionImpl>();
}

//-----
ScheduleCreateTransaction& ScheduleCreateTransaction::operator=(ScheduleCreateTransaction&& other) noexcept
{
  if (this != &other)
  {
    mImpl = std::make_unique<ScheduleCreateTransactionImpl>(*other.mImpl);

    // Leave the moved-from ScheduleCreateTransaction in a valid state.
    other.mImpl = std::make_unique<ScheduleCreateTransactionImpl>();
  }

  return *this;
}

//-----
ScheduleCreateTransaction::ScheduleCreateTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<ScheduleCreateTransaction>(transactionBody)
{
  initImpl();

  if (!transactionBody.has_schedulecreate())
  {
    throw std::invalid_argument("Transaction body doesn't contain ScheduleCreate data");
  }

  const proto::ScheduleCreateTransactionBody& body = transactionBody.schedulecreate();

  if (body.has_scheduledtransactionbody())
  {
    mImpl->mTransactionToSchedule = body.scheduledtransactionbody();
  }

  mImpl->mMemo = body.memo();

  if (body.has_adminkey())
  {
    mImpl->mAdminKey = Key::fromProtobuf(body.adminkey());
  }

  if (body.has_payeraccountid())
  {
    mImpl->mPayerAccountId = AccountId::fromProtobuf(body.payeraccountid());
  }

  if (body.has_expiration_time())
  {
    mImpl->mExpirationTime = internal::TimestampConverter::fromProtobuf(body.expiration_time());
  }

  mImpl->mWaitForExpiration = body.wait_for_expiry();
}

//-----
ScheduleCreateTransaction& ScheduleCreateTransaction::setScheduledTransaction(const WrappedTransaction& tx)
{
  requireNotFrozen();

  switch (tx.getTransactionType())
  {
    case ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<AccountAllowanceApproveTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case ACCOUNT_ALLOWANCE_DELETE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<AccountAllowanceDeleteTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case ACCOUNT_CREATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<AccountCreateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case ACCOUNT_DELETE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<AccountDeleteTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case ACCOUNT_UPDATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<AccountUpdateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case CONTRACT_CREATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<ContractCreateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case CONTRACT_DELETE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<ContractDeleteTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case CONTRACT_EXECUTE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<ContractExecuteTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case CONTRACT_UPDATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<ContractUpdateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case FILE_APPEND_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<FileAppendTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case FILE_CREATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<FileCreateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case FILE_DELETE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<FileDeleteTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case FILE_UPDATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<FileUpdateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_ASSOCIATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenAssociateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_BURN_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenBurnTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_CREATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenCreateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_DELETE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenDeleteTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_DISSOCIATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenDissociateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_FEE_SCHEDULE_UPDATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenFeeScheduleUpdateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_FREEZE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenFreezeTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_GRANT_KYC_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenGrantKycTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_MINT_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenMintTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_PAUSE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenPauseTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_REVOKE_KYC_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenRevokeKycTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_UNFREEZE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenUnfreezeTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_UNPAUSE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenUnpauseTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_UPDATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenUpdateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOKEN_WIPE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TokenWipeTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOPIC_CREATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TopicCreateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOPIC_DELETE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TopicDeleteTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOPIC_MESSAGE_SUBMIT_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TopicMessageSubmitTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TOPIC_UPDATE_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TopicUpdateTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case TRANSFER_TRANSACTION:
      return setScheduledTransaction(
        tx.getTransaction<TransferTransaction>()->schedule().mImpl->mTransactionToSchedule);
    case ETHEREUM_TRANSACTION:
    case SCHEDULE_CREATE_TRANSACTION:
    {
      throw UnsupportedOperationException("Cannot schedule input transaction type");
    }
    default:
    {
      throw UninitializedException("WrappedTransaction does not currently hold a transaction");
    }
  }
}

//-----
ScheduleCreateTransaction& ScheduleCreateTransaction::setScheduledTransaction(
  const proto::SchedulableTransactionBody& transaction)
{
  requireNotFrozen();
  mImpl->mTransactionToSchedule = transaction;
  return *this;
}

//-----
ScheduleCreateTransaction& ScheduleCreateTransaction::setScheduleMemo(std::string_view memo)
{
  requireNotFrozen();
  mImpl->mMemo = memo;
  return *this;
}

//-----
ScheduleCreateTransaction& ScheduleCreateTransaction::setAdminKey(const std::shared_ptr<Key>& key)
{
  requireNotFrozen();
  mImpl->mAdminKey = key;
  return *this;
}

//-----
ScheduleCreateTransaction& ScheduleCreateTransaction::setPayerAccountId(const AccountId& accountId)
{
  requireNotFrozen();
  mImpl->mPayerAccountId = accountId;
  return *this;
}

//-----
ScheduleCreateTransaction& ScheduleCreateTransaction::setExpirationTime(
  const std::chrono::system_clock::time_point& time)
{
  requireNotFrozen();
  mImpl->mExpirationTime = time;
  return *this;
}

//-----
ScheduleCreateTransaction& ScheduleCreateTransaction::setWaitForExpiry(bool wait)
{
  requireNotFrozen();
  mImpl->mWaitForExpiration = wait;
  return *this;
}

//-----
WrappedTransaction ScheduleCreateTransaction::getScheduledTransaction() const
{
  proto::TransactionBody txBody;
  txBody.set_transactionfee(mImpl->mTransactionToSchedule.transactionfee());
  txBody.set_memo(mImpl->mTransactionToSchedule.memo());

  if (mImpl->mTransactionToSchedule.has_cryptoapproveallowance())
  {
    *txBody.mutable_cryptoapproveallowance() = mImpl->mTransactionToSchedule.cryptoapproveallowance();
  }
  else if (mImpl->mTransactionToSchedule.has_cryptodeleteallowance())
  {
    *txBody.mutable_cryptodeleteallowance() = mImpl->mTransactionToSchedule.cryptodeleteallowance();
  }
  else if (mImpl->mTransactionToSchedule.has_cryptocreateaccount())
  {
    *txBody.mutable_cryptocreateaccount() = mImpl->mTransactionToSchedule.cryptocreateaccount();
  }
  else if (mImpl->mTransactionToSchedule.has_cryptodeleteallowance())
  {
    *txBody.mutable_cryptodelete() = mImpl->mTransactionToSchedule.cryptodelete();
  }
  else if (mImpl->mTransactionToSchedule.has_cryptoupdateaccount())
  {
    *txBody.mutable_cryptoupdateaccount() = mImpl->mTransactionToSchedule.cryptoupdateaccount();
  }
  else if (mImpl->mTransactionToSchedule.has_contractcreateinstance())
  {
    *txBody.mutable_contractcreateinstance() = mImpl->mTransactionToSchedule.contractcreateinstance();
  }
  else if (mImpl->mTransactionToSchedule.has_contractdeleteinstance())
  {
    *txBody.mutable_contractdeleteinstance() = mImpl->mTransactionToSchedule.contractdeleteinstance();
  }
  else if (mImpl->mTransactionToSchedule.has_contractcall())
  {
    *txBody.mutable_contractcall() = mImpl->mTransactionToSchedule.contractcall();
  }
  else if (mImpl->mTransactionToSchedule.has_contractupdateinstance())
  {
    *txBody.mutable_contractupdateinstance() = mImpl->mTransactionToSchedule.contractupdateinstance();
  }
  else if (mImpl->mTransactionToSchedule.has_fileappend())
  {
    *txBody.mutable_fileappend() = mImpl->mTransactionToSchedule.fileappend();
  }
  else if (mImpl->mTransactionToSchedule.has_filecreate())
  {
    *txBody.mutable_filecreate() = mImpl->mTransactionToSchedule.filecreate();
  }
  else if (mImpl->mTransactionToSchedule.has_filedelete())
  {
    *txBody.mutable_filedelete() = mImpl->mTransactionToSchedule.filedelete();
  }
  else if (mImpl->mTransactionToSchedule.has_fileupdate())
  {
    *txBody.mutable_fileupdate() = mImpl->mTransactionToSchedule.fileupdate();
  }
  else if (mImpl->mTransactionToSchedule.has_tokenassociate())
  {
    *txBody.mutable_tokenassociate() = mImpl->mTransactionToSchedule.tokenassociate();
  }
  else if (mImpl->mTransactionToSchedule.has_tokenburn())
  {
    *txBody.mutable_tokenburn() = mImpl->mTransactionToSchedule.tokenburn();
  }
  else if (mImpl->mTransactionToSchedule.has_tokencreation())
  {
    *txBody.mutable_tokencreation() = mImpl->mTransactionToSchedule.tokencreation();
  }
  else if (mImpl->mTransactionToSchedule.has_tokendeletion())
  {
    *txBody.mutable_tokendeletion() = mImpl->mTransactionToSchedule.tokendeletion();
  }
  else if (mImpl->mTransactionToSchedule.has_tokendissociate())
  {
    *txBody.mutable_tokendissociate() = mImpl->mTransactionToSchedule.tokendissociate();
  }
  else if (mImpl->mTransactionToSchedule.has_token_fee_schedule_update())
  {
    *txBody.mutable_token_fee_schedule_update() = mImpl->mTransactionToSchedule.token_fee_schedule_update();
  }
  else if (mImpl->mTransactionToSchedule.has_tokenfreeze())
  {
    *txBody.mutable_tokenfreeze() = mImpl->mTransactionToSchedule.tokenfreeze();
  }
  else if (mImpl->mTransactionToSchedule.has_tokengrantkyc())
  {
    *txBody.mutable_tokengrantkyc() = mImpl->mTransactionToSchedule.tokengrantkyc();
  }
  else if (mImpl->mTransactionToSchedule.has_tokenmint())
  {
    *txBody.mutable_tokenmint() = mImpl->mTransactionToSchedule.tokenmint();
  }
  else if (mImpl->mTransactionToSchedule.has_token_pause())
  {
    *txBody.mutable_token_pause() = mImpl->mTransactionToSchedule.token_pause();
  }
  else if (mImpl->mTransactionToSchedule.has_tokenrevokekyc())
  {
    *txBody.mutable_tokenrevokekyc() = mImpl->mTransactionToSchedule.tokenrevokekyc();
  }
  else if (mImpl->mTransactionToSchedule.has_tokenunfreeze())
  {
    *txBody.mutable_tokenunfreeze() = mImpl->mTransactionToSchedule.tokenunfreeze();
  }
  else if (mImpl->mTransactionToSchedule.has_token_unpause())
  {
    *txBody.mutable_token_unpause() = mImpl->mTransactionToSchedule.token_unpause();
  }
  else if (mImpl->mTransactionToSchedule.has_tokenwipe())
  {
    *txBody.mutable_tokenwipe() = mImpl->mTransactionToSchedule.tokenwipe();
  }
  else if (mImpl->mTransactionToSchedule.has_consensuscreatetopic())
  {
    *txBody.mutable_consensuscreatetopic() = mImpl->mTransactionToSchedule.consensuscreatetopic();
  }
  else if (mImpl->mTransactionToSchedule.has_consensusdeletetopic())
  {
    *txBody.mutable_consensusdeletetopic() = mImpl->mTransactionToSchedule.consensusdeletetopic();
  }
  else if (mImpl->mTransactionToSchedule.has_consensussubmitmessage())
  {
    *txBody.mutable_consensussubmitmessage() = mImpl->mTransactionToSchedule.consensussubmitmessage();
  }
  else if (mImpl->mTransactionToSchedule.has_consensusupdatetopic())
  {
    *txBody.mutable_consensusupdatetopic() = mImpl->mTransactionToSchedule.consensusupdatetopic();
  }
  else if (mImpl->mTransactionToSchedule.has_cryptotransfer())
  {
    *txBody.mutable_cryptotransfer() = mImpl->mTransactionToSchedule.cryptotransfer();
  }
  else
  {
    throw std::invalid_argument("TransactionBody contains no transaction");
  }

  return WrappedTransaction(txBody);
}

//----
std::string ScheduleCreateTransaction::getScheduleMemo() const
{
  return mImpl->mMemo;
}

//----
std::shared_ptr<Key> ScheduleCreateTransaction::getAdminKey() const
{
  return mImpl->mAdminKey;
}

//----
std::optional<AccountId> ScheduleCreateTransaction::getPayerAccountId() const
{
  return mImpl->mPayerAccountId;
}

//----
std::optional<std::chrono::system_clock::time_point> ScheduleCreateTransaction::getExpirationTime() const
{
  return mImpl->mExpirationTime;
}

//----
bool ScheduleCreateTransaction::isWaitForExpiry() const
{
  return mImpl->mWaitForExpiration;
}

//-----
proto::Transaction ScheduleCreateTransaction::makeRequest(const Client& client,
                                                          const std::shared_ptr<internal::Node>&) const
{
  proto::TransactionBody txBody = generateTransactionBody(&client);
  addToBody(txBody);
  return signTransaction(txBody, client);
}

//-----
grpc::Status ScheduleCreateTransaction::submitRequest(const Client& client,
                                                      const std::chrono::system_clock::time_point& deadline,
                                                      const std::shared_ptr<internal::Node>& node,
                                                      proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kScheduleCreate, makeRequest(client, node), deadline, response);
}

//-----
void ScheduleCreateTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_schedulecreate(build());
}

//-----
proto::ScheduleCreateTransactionBody* ScheduleCreateTransaction::build() const
{
  auto body = std::make_unique<proto::ScheduleCreateTransactionBody>();
  *body->mutable_scheduledtransactionbody() = mImpl->mTransactionToSchedule;

  body->set_memo(mImpl->mMemo);

  if (mImpl->mAdminKey)
  {
    body->set_allocated_adminkey(mImpl->mAdminKey->toProtobufKey().release());
  }

  if (mImpl->mPayerAccountId.has_value())
  {
    body->set_allocated_payeraccountid(mImpl->mPayerAccountId->toProtobuf().release());
  }

  if (mImpl->mExpirationTime.has_value())
  {
    body->set_allocated_expiration_time(internal::TimestampConverter::toProtobuf(mImpl->mExpirationTime.value()));
  }

  body->set_wait_for_expiry(mImpl->mWaitForExpiration);

  return body.release();
}

//-----
void ScheduleCreateTransaction::initImpl()
{
  mImpl = std::make_unique<ScheduleCreateTransactionImpl>();
}

} // namespace Hedera
