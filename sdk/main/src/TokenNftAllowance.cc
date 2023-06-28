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
#include "TokenNftAllowance.h"

#include <proto/crypto_approve_allowance.pb.h>

namespace Hedera
{
//-----
TokenNftAllowance::TokenNftAllowance(const TokenId& tokenId,
                                     AccountId owner,
                                     AccountId spender,
                                     std::vector<uint64_t> serialNumbers,
                                     std::optional<bool> allowAll,
                                     std::optional<AccountId> delegatingSpender)
  : mTokenId(tokenId)
  , mOwnerAccountId(std::move(owner))
  , mSpenderAccountId(std::move(spender))
  , mSerialNumbers(std::move(serialNumbers))
  , mApprovedForAll(allowAll)
  , mDelegatingSpenderAccountId(std::move(delegatingSpender))
{
}

//-----
TokenNftAllowance TokenNftAllowance::fromProtobuf(const proto::NftAllowance& proto)
{
  TokenNftAllowance allowance;

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

  for (int i = 0; i < proto.serial_numbers_size(); ++i)
  {
    allowance.mSerialNumbers.push_back(static_cast<uint64_t>(proto.serial_numbers(i)));
  }

  if (proto.has_approved_for_all())
  {
    allowance.mApprovedForAll = proto.approved_for_all().value();
  }

  if (proto.has_delegating_spender())
  {
    allowance.mDelegatingSpenderAccountId = AccountId::fromProtobuf(proto.delegating_spender());
  }

  return allowance;
}

//-----
std::unique_ptr<proto::NftAllowance> TokenNftAllowance::toProtobuf() const
{
  auto proto = std::make_unique<proto::NftAllowance>();
  proto->set_allocated_tokenid(mTokenId.toProtobuf().release());
  proto->set_allocated_owner(mOwnerAccountId.toProtobuf().release());
  proto->set_allocated_spender(mSpenderAccountId.toProtobuf().release());

  for (const uint64_t& serialNumber : mSerialNumbers)
  {
    proto->add_serial_numbers(static_cast<int64_t>(serialNumber));
  }

  if (mApprovedForAll.has_value())
  {
    auto value = std::make_unique<google::protobuf::BoolValue>();
    value->set_value(*mApprovedForAll);
    proto->set_allocated_approved_for_all(value.release());
  }

  if (mDelegatingSpenderAccountId.has_value())
  {
    proto->set_allocated_delegating_spender(mDelegatingSpenderAccountId->toProtobuf().release());
  }

  return proto;
}

//-----
TokenNftAllowance& TokenNftAllowance::setTokenId(const TokenId& tokenId)
{
  mTokenId = tokenId;
  return *this;
}

//-----
TokenNftAllowance& TokenNftAllowance::setOwnerAccountId(const AccountId& accountId)
{
  mOwnerAccountId = accountId;
  return *this;
}

//-----
TokenNftAllowance& TokenNftAllowance::setSpenderAccountId(const AccountId& accountId)
{
  mSpenderAccountId = accountId;
  return *this;
}

//-----
TokenNftAllowance& TokenNftAllowance::addSerialNumber(const uint64_t& serialNumber)
{
  mSerialNumbers.push_back(static_cast<int64_t>(serialNumber));
  return *this;
}

//-----
TokenNftAllowance& TokenNftAllowance::approveForAll(bool allowAll)
{
  mApprovedForAll = allowAll;
  if (allowAll)
  {
    mSerialNumbers.clear();
  }

  return *this;
}

//-----
TokenNftAllowance& TokenNftAllowance::setDelegatingSpenderAccountId(const AccountId& accountId)
{
  mDelegatingSpenderAccountId = accountId;
  return *this;
}

} // namespace Hedera
