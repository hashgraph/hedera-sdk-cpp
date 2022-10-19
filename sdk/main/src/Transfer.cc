/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#include "Transfer.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
Transfer Transfer::fromProtobuf(const proto::AccountAmount& proto)
{
  Transfer transfer;

  if (proto.has_accountid())
  {
    transfer.mAccountId = AccountId::fromProtobuf(proto.accountid());
  }

  transfer.mAmount = Hbar::from(proto.amount());
  transfer.mIsApproved = proto.is_approval();

  return transfer;
}

//-----
std::shared_ptr<proto::AccountAmount> Transfer::toProtobuf() const
{
  auto proto = std::make_shared<proto::AccountAmount>();

  if (mAccountId.has_value())
  {
    proto->set_allocated_accountid(mAccountId->toProtobuf());
  }

  proto->set_amount(mAmount.toTinybars());
  proto->set_is_approval(mIsApproved);

  return proto;
}

//-----
Transfer& Transfer::setAccountId(const Hedera::AccountId& accountId)
{
  mAccountId = accountId;
  return *this;
}

//-----
Transfer& Transfer::setAmount(const Hedera::Hbar& amount)
{
  mAmount = amount;
  return *this;
}

//-----
Transfer& Transfer::setApproved(bool approved)
{
  mIsApproved = approved;
  return *this;
}

} // namespace Hedera
