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
#include "HbarTransfer.h"

#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
HbarTransfer HbarTransfer::fromProtobuf(const proto::AccountAmount& proto)
{
  HbarTransfer transfer;

  if (proto.has_accountid())
  {
    transfer.mAccountId = AccountId::fromProtobuf(proto.accountid());
  }

  transfer.mAmount = Hbar(proto.amount(), HbarUnit::TINYBAR());
  transfer.mIsApproved = proto.is_approval();

  return transfer;
}

//-----
std::unique_ptr<proto::AccountAmount> HbarTransfer::toProtobuf() const
{
  auto proto = std::make_unique<proto::AccountAmount>();
  proto->set_allocated_accountid(mAccountId.toProtobuf().release());
  proto->set_amount(mAmount.toTinybars());
  proto->set_is_approval(mIsApproved);

  return proto;
}

//-----
HbarTransfer& HbarTransfer::setAccountId(const AccountId& accountId)
{
  mAccountId = accountId;
  return *this;
}

//-----
HbarTransfer& HbarTransfer::setAmount(const Hbar& amount)
{
  mAmount = amount;
  return *this;
}

//-----
HbarTransfer& HbarTransfer::setApproved(bool approved)
{
  mIsApproved = approved;
  return *this;
}

} // namespace Hedera
