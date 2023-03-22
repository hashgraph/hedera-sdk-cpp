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
#include "HbarAllowance.h"

#include <proto/crypto_approve_allowance.pb.h>

namespace Hedera
{
//-----
HbarAllowance::HbarAllowance(AccountId owner, AccountId spender, const Hbar& amount)
  : mOwnerAccountId(std::move(owner))
  , mSpenderAccountId(std::move(spender))
  , mAmount(amount)
{
}

//-----
HbarAllowance HbarAllowance::fromProtobuf(const proto::CryptoAllowance& proto)
{
  HbarAllowance allowance;

  if (proto.has_owner())
  {
    allowance.mOwnerAccountId = AccountId::fromProtobuf(proto.owner());
  }

  if (proto.has_spender())
  {
    allowance.mSpenderAccountId = AccountId::fromProtobuf(proto.spender());
  }

  allowance.mAmount = Hbar(proto.amount(), HbarUnit::TINYBAR());
  return allowance;
}

//-----
std::unique_ptr<proto::CryptoAllowance> HbarAllowance::toProtobuf() const
{
  auto proto = std::make_unique<proto::CryptoAllowance>();
  proto->set_allocated_owner(mOwnerAccountId.toProtobuf().release());
  proto->set_allocated_spender(mSpenderAccountId.toProtobuf().release());
  proto->set_amount(mAmount.toTinybars());
  return proto;
}

//-----
HbarAllowance& HbarAllowance::setOwnerAccountId(const AccountId& accountId)
{
  mOwnerAccountId = accountId;
  return *this;
}

//-----
HbarAllowance& HbarAllowance::setSpenderAccountId(const AccountId& accountId)
{
  mSpenderAccountId = accountId;
  return *this;
}

//-----
HbarAllowance& HbarAllowance::setAmount(const Hbar& amount)
{
  mAmount = amount;
  return *this;
}

} // namespace Hedera
