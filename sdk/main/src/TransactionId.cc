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

#include "helper/TimestampConverter.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
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
    id.mValidTransactionTime = TimestampConverter::fromProtobuf(proto.transactionvalidstart());
  }

  if (proto.has_accountid())
  {
    id.mAccountId = AccountId::fromProtobuf(proto.accountid());
  }

  return id;
}

//-----
proto::TransactionID* TransactionId::toProtobuf() const
{
  auto proto = new proto::TransactionID;

  if (mValidTransactionTime.has_value())
  {
    proto->set_allocated_transactionvalidstart(TimestampConverter::toProtobuf(mValidTransactionTime.value()));
  }

  if (mAccountId.has_value())
  {
    proto->set_allocated_accountid(mAccountId.value().toProtobuf());
  }

  return proto;
}

} // namespace Hedera
