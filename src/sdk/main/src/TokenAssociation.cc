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
#include "TokenAssociation.h"
#include "impl/Utilities.h"

#include <custom_fees.pb.h>
#include <nlohmann/json.hpp>

namespace Hedera
{
//-----
TokenAssociation TokenAssociation::fromProtobuf(const proto::TokenAssociation& proto)
{
  TokenAssociation tokenAssociation;

  if (proto.has_account_id())
  {
    tokenAssociation.mAccountId = AccountId::fromProtobuf(proto.account_id());
  }

  if (proto.has_token_id())
  {
    tokenAssociation.mTokenId = TokenId::fromProtobuf(proto.token_id());
  }

  return tokenAssociation;
}

//-----
TokenAssociation TokenAssociation::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::TokenAssociation tokenAssociation;
  tokenAssociation.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(tokenAssociation);
}

//-----
std::unique_ptr<proto::TokenAssociation> TokenAssociation::toProtobuf() const
{
  auto tokenAssociation = std::make_unique<proto::TokenAssociation>();
  tokenAssociation->set_allocated_account_id(mAccountId.toProtobuf().release());
  tokenAssociation->set_allocated_token_id(mTokenId.toProtobuf().release());
  return tokenAssociation;
}

//-----
std::vector<std::byte> TokenAssociation::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string TokenAssociation::toString() const
{
  nlohmann::json json;
  json["mAccountId"] = mAccountId.toString();
  json["mTokenId"] = mTokenId.toString();
  return json.dump();
}

} // namespace Hedera