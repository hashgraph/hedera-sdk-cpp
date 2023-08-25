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
#include "TransactionId.h"
#include "impl/TimestampConverter.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
TransactionId TransactionId::withValidStart(const AccountId& accountId,
                                            const std::chrono::system_clock::time_point& start)
{
  TransactionId transactionId;
  transactionId.mAccountId = accountId;
  transactionId.mValidTransactionTime = start;
  return transactionId;
}

//-----
TransactionId TransactionId::generate(const AccountId& accountId)
{
  TransactionId transactionId;
  transactionId.mAccountId = accountId;
  transactionId.mValidTransactionTime = std::chrono::system_clock::now();
  return transactionId;
}

//-----
TransactionId TransactionId::fromProtobuf(const proto::TransactionID& proto)
{
  TransactionId id;

  if (proto.has_transactionvalidstart())
  {
    id.mValidTransactionTime = internal::TimestampConverter::fromProtobuf(proto.transactionvalidstart());
  }

  if (proto.has_accountid())
  {
    id.mAccountId = AccountId::fromProtobuf(proto.accountid());
  }

  return id;
}

//-----
bool TransactionId::operator==(const TransactionId& other) const
{
  return (mValidTransactionTime == other.mValidTransactionTime) && (mAccountId == other.mAccountId);
}

//-----
std::unique_ptr<proto::TransactionID> TransactionId::toProtobuf() const
{
  auto proto = std::make_unique<proto::TransactionID>();
  proto->set_allocated_transactionvalidstart(internal::TimestampConverter::toProtobuf(mValidTransactionTime));
  proto->set_allocated_accountid(mAccountId.toProtobuf().release());
  return proto;
}

//-----
std::string TransactionId::toString() const
{
  const auto secondsSinceEpoch =
    std::chrono::duration_cast<std::chrono::seconds>(mValidTransactionTime.time_since_epoch());
  return mAccountId.toString() + '@' + std::to_string(secondsSinceEpoch.count()) + '.' +
         std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(mValidTransactionTime.time_since_epoch() -
                                                                             secondsSinceEpoch)
                          .count());
}

} // namespace Hedera
