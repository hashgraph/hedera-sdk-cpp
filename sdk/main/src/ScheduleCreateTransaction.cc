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
#include "impl/Node.h"
#include "impl/TimestampConverter.h"

#include <grpcpp/client_context.h>
#include <proto/schedule_create.pb.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>

namespace Hedera
{
//-----
struct ScheduleCreateTransaction::ScheduleCreateTransactionImpl
{
  // The transaction to schedule.
  WrappedTransaction mTransactionToSchedule;

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
    mImpl->mTransactionToSchedule = WrappedTransaction::fromProtobuf(body.scheduledtransactionbody());
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
  mImpl->mTransactionToSchedule = tx;
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
  return mImpl->mTransactionToSchedule;
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
  return signTransaction(generateTransactionBody(&client), client);
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

  body->set_allocated_scheduledtransactionbody(mImpl->mTransactionToSchedule.toSchedulableProtobuf().release());
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
