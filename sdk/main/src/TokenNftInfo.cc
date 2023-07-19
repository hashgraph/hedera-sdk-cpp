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
#include "TokenNftInfo.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <proto/token_get_nft_info.pb.h>

namespace Hedera
{
//-----
TokenNftInfo TokenNftInfo::fromProtobuf(const proto::TokenNftInfo& proto)
{
  TokenNftInfo tokenNftInfo;

  if (proto.has_nftid())
  {
    tokenNftInfo.mNftId = NftId::fromProtobuf(proto.nftid());
  }

  if (proto.has_accountid())
  {
    tokenNftInfo.mAccountId = AccountId::fromProtobuf(proto.accountid());
  }

  if (proto.has_creationtime())
  {
    tokenNftInfo.mCreationTime = internal::TimestampConverter::fromProtobuf(proto.creationtime());
  }

  tokenNftInfo.mMetadata = internal::Utilities::stringToByteVector(proto.metadata());
  tokenNftInfo.mLedgerId = LedgerId(internal::Utilities::stringToByteVector(proto.ledger_id()));

  if (proto.has_spender_id())
  {
    tokenNftInfo.mSpenderId = AccountId::fromProtobuf(proto.spender_id());
  }

  return tokenNftInfo;
}

//-----
TokenNftInfo TokenNftInfo::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::TokenNftInfo proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
std::unique_ptr<proto::TokenNftInfo> TokenNftInfo::toProtobuf() const
{
  auto protoTokenNftInfo = std::make_unique<proto::TokenNftInfo>();
  protoTokenNftInfo->set_allocated_nftid(mNftId.toProtobuf().release());
  protoTokenNftInfo->set_allocated_accountid(mAccountId.toProtobuf().release());
  protoTokenNftInfo->set_allocated_creationtime(internal::TimestampConverter::toProtobuf(mCreationTime));
  protoTokenNftInfo->set_metadata(internal::Utilities::byteVectorToString(mMetadata));
  protoTokenNftInfo->set_ledger_id(internal::Utilities::byteVectorToString(mLedgerId.toBytes()));

  if (mSpenderId.has_value())
  {
    protoTokenNftInfo->set_allocated_spender_id(mSpenderId->toProtobuf().release());
  }

  return protoTokenNftInfo;
}

//-----
std::vector<std::byte> TokenNftInfo::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

} // namespace Hedera
