/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "impl/Utilities.h"

#include <proto/crypto_approve_allowance.pb.h>

namespace Hedera
{
//-----
TokenAllowance::TokenAllowance(const TokenId& tokenId, AccountId owner, AccountId spender, uint64_t amount)
  : mTokenId(tokenId)
  , mOwnerAccountId(std::move(owner))
  , mSpenderAccountId(std::move(spender))
  , mAmount(amount)
{
}

//-----
TokenAllowance TokenAllowance::fromProtobuf(const proto::TokenAllowance& proto)
{
  return { proto.has_tokenid() ? TokenId::fromProtobuf(proto.tokenid()) : TokenId(),
           proto.has_owner() ? AccountId::fromProtobuf(proto.owner()) : AccountId(),
           proto.has_spender() ? AccountId::fromProtobuf(proto.spender()) : AccountId(),
           static_cast<uint64_t>(proto.amount()) };
}

//-----
TokenAllowance TokenAllowance::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::TokenAllowance tokenAllowance;
  tokenAllowance.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(tokenAllowance);
}

//-----
void TokenAllowance::validateChecksums(const Client& client) const
{
  mTokenId.validateChecksum(client);
  mOwnerAccountId.validateChecksum(client);
  mSpenderAccountId.validateChecksum(client);
}

//-----
std::unique_ptr<proto::TokenAllowance> TokenAllowance::toProtobuf() const
{
  auto proto = std::make_unique<proto::TokenAllowance>();

  if (!(mTokenId == TokenId()))
  {
    proto->set_allocated_tokenid(mTokenId.toProtobuf().release());
  }

  if (!(mOwnerAccountId == AccountId()))
  {
    proto->set_allocated_owner(mOwnerAccountId.toProtobuf().release());
  }

  if (!(mSpenderAccountId == AccountId()))
  {
    proto->set_allocated_spender(mSpenderAccountId.toProtobuf().release());
  }

  proto->set_amount(static_cast<int64_t>(mAmount));
  return proto;
}

//-----
std::vector<std::byte> TokenAllowance::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

} // namespace Hedera
