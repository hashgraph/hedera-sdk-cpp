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
#include "TokenNftRemoveAllowance.h"

#include <proto/crypto_delete_allowance.pb.h>

namespace Hedera
{
//-----
TokenNftRemoveAllowance::TokenNftRemoveAllowance(const TokenId& tokenId,
                                                 AccountId owner,
                                                 std::vector<uint64_t> serialNumbers)
  : mTokenId(tokenId)
  , mOwnerAccountId(std::move(owner))
  , mSerialNumbers(std::move(serialNumbers))
{
}

//-----
TokenNftRemoveAllowance TokenNftRemoveAllowance::fromProtobuf(const proto::NftRemoveAllowance& proto)
{
  TokenNftRemoveAllowance allowance;

  if (proto.has_token_id())
  {
    allowance.mTokenId = TokenId::fromProtobuf(proto.token_id());
  }

  if (proto.has_owner())
  {
    allowance.mOwnerAccountId = AccountId::fromProtobuf(proto.owner());
  }

  for (int i = 0; i < proto.serial_numbers_size(); ++i)
  {
    allowance.mSerialNumbers.push_back(static_cast<uint64_t>(proto.serial_numbers(i)));
  }

  return allowance;
}

//-----
std::unique_ptr<proto::NftRemoveAllowance> TokenNftRemoveAllowance::toProtobuf() const
{
  auto proto = std::make_unique<proto::NftRemoveAllowance>();
  proto->set_allocated_token_id(mTokenId.toProtobuf().release());
  proto->set_allocated_owner(mOwnerAccountId.toProtobuf().release());

  for (const uint64_t& serialNumber : mSerialNumbers)
  {
    proto->add_serial_numbers(static_cast<int64_t>(serialNumber));
  }

  return proto;
}

//-----
TokenNftRemoveAllowance& TokenNftRemoveAllowance::setTokenId(const TokenId& tokenId)
{
  mTokenId = tokenId;
  return *this;
}

//-----
TokenNftRemoveAllowance& TokenNftRemoveAllowance::setOwnerAccountId(const AccountId& accountId)
{
  mOwnerAccountId = accountId;
  return *this;
}

//-----
TokenNftRemoveAllowance& TokenNftRemoveAllowance::addSerialNumber(const uint64_t& serialNumber)
{
  mSerialNumbers.push_back(static_cast<int64_t>(serialNumber));
  return *this;
}

} // namespace Hedera
