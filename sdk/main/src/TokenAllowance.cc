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
#include "TokenAllowance.h"

#include <proto/crypto_approve_allowance.pb.h>

namespace Hedera
{
//-----
TokenAllowance::TokenAllowance(const TokenId& tokenId, AccountId owner, AccountId spender, const uint64_t& amount)
  : mTokenId(tokenId)
  , mOwnerAccountId(std::move(owner))
  , mSpenderAccountId(std::move(spender))
  , mAmount(amount)
{
}

//-----
TokenAllowance TokenAllowance::fromProtobuf(const proto::TokenAllowance& proto)
{
  TokenAllowance allowance;

  if (proto.has_tokenid())
  {
    allowance.mTokenId = TokenId::fromProtobuf(proto.tokenid());
  }

  if (proto.has_owner())
  {
    allowance.mOwnerAccountId = AccountId::fromProtobuf(proto.owner());
  }

  if (proto.has_spender())
  {
    allowance.mSpenderAccountId = AccountId::fromProtobuf(proto.spender());
  }

  allowance.mAmount = static_cast<uint64_t>(proto.amount());
  return allowance;
}

//-----
std::unique_ptr<proto::TokenAllowance> TokenAllowance::toProtobuf() const
{
  auto proto = std::make_unique<proto::TokenAllowance>();
  proto->set_allocated_tokenid(mTokenId.toProtobuf().release());
  proto->set_allocated_owner(mOwnerAccountId.toProtobuf().release());
  proto->set_allocated_spender(mSpenderAccountId.toProtobuf().release());
  proto->set_amount(static_cast<int64_t>(mAmount));
  return proto;
}

//-----
TokenAllowance& TokenAllowance::setTokenId(const TokenId& tokenId)
{
  mTokenId = tokenId;
  return *this;
}

//-----
TokenAllowance& TokenAllowance::setOwnerAccountId(const AccountId& accountId)
{
  mOwnerAccountId = accountId;
  return *this;
}

//-----
TokenAllowance& TokenAllowance::setSpenderAccountId(const AccountId& accountId)
{
  mSpenderAccountId = accountId;
  return *this;
}

//-----
TokenAllowance& TokenAllowance::setAmount(const uint64_t& amount)
{
  mAmount = amount;
  return *this;
}

} // namespace Hedera
