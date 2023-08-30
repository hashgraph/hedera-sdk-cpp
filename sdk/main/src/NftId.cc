/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "NftId.h"

#include <charconv>
#include <proto/token_get_nft_info.pb.h>

namespace Hedera
{
//-----
NftId::NftId(const TokenId& id, const uint64_t& serialNumber)
  : mTokenId(id)
  , mSerialNum(serialNumber)
{
  checkSerialNum();
}

//-----
bool NftId::operator==(const NftId& other) const
{
  return (mTokenId == other.mTokenId) && (mSerialNum == other.mSerialNum);
}

//-----
NftId NftId::fromString(std::string_view id)
{
  NftId nftId;

  // Get the index of the delimiter '/'
  const size_t slashIndex = id.find('/');
  if (slashIndex == std::string::npos)
  {
    throw std::invalid_argument("Input NFT ID string malformed");
  }

  // Get the token ID (will throw std::invalid_argument if malformed)
  nftId.mTokenId = TokenId::fromString(id.substr(0, slashIndex));

  // Convert the serial number
  const std::string_view serialNumberStr = id.substr(slashIndex + 1, id.size() - slashIndex - 1);
  if (const auto result =
        std::from_chars(serialNumberStr.data(), serialNumberStr.data() + serialNumberStr.size(), nftId.mSerialNum);
      result.ec != std::errc() || result.ptr != serialNumberStr.data() + serialNumberStr.size())
  {
    throw std::invalid_argument("Input NFT ID string is malformed");
  }
  nftId.checkSerialNum();

  return nftId;
}

//-----
NftId NftId::fromProtobuf(const proto::NftID& proto)
{
  NftId nftId;

  if (proto.has_token_id())
  {
    nftId.mTokenId = TokenId::fromProtobuf(proto.token_id());
  }

  nftId.mSerialNum = static_cast<uint64_t>(proto.serial_number());
  return nftId;
}

//-----
std::unique_ptr<proto::NftID> NftId::toProtobuf() const
{
  auto nftId = std::make_unique<proto::NftID>();
  nftId->set_allocated_token_id(mTokenId.toProtobuf().release());
  nftId->set_serial_number(static_cast<int64_t>(mSerialNum));
  return nftId;
}

//-----
std::string NftId::toString() const
{
  return mTokenId.toString() + '/' + std::to_string(mSerialNum);
}

//-----
NftId& NftId::setTokenId(const TokenId& id)
{
  mTokenId = id;
  return *this;
}

//-----
NftId& NftId::setSerialNum(const uint64_t& num)
{
  mSerialNum = num;
  checkSerialNum();
  return *this;
}

//-----
void NftId::checkSerialNum() const
{
  if (mSerialNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input serial number too big");
  }
}

} // namespace Hedera