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
#include "TopicId.h"
#include "exceptions/IllegalStateException.h"
#include "exceptions/OpenSSLException.h"
#include "impl/HexConverter.h"
#include "impl/Utilities.h"

#include <charconv>
#include <limits>
#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
TopicId::TopicId(const uint64_t& num)
  : mTopicNum(num)
{
}

//-----
TopicId::TopicId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num)
  : mShardNum(shard)
  , mRealmNum(realm)
  , mTopicNum(num)
{
}

//-----
bool TopicId::operator==(const TopicId& other) const
{
  return (mShardNum == other.mShardNum) && (mRealmNum == other.mRealmNum) && (mTopicNum == other.mTopicNum);
}

//-----
TopicId TopicId::fromString(std::string_view id)
{
  TopicId topicId;

  // Get the indices of the two delimiter '.'
  const size_t firstDot = id.find_first_of('.');
  const size_t secondDot = id.find_last_of('.');

  // Make sure there are at least two dots
  if (firstDot == secondDot)
  {
    throw std::invalid_argument("Input topic ID string is malformed");
  }

  // Grab the three strings
  const std::string_view shardStr = id.substr(0, firstDot);
  const std::string_view realmStr = id.substr(firstDot + 1, secondDot - firstDot - 1);
  const std::string_view topicStr = id.substr(secondDot + 1, id.size() - secondDot - 1);

  // Convert the shard number
  auto result = std::from_chars(shardStr.data(), shardStr.data() + shardStr.size(), topicId.mShardNum);
  if (result.ec != std::errc() || result.ptr != shardStr.data() + shardStr.size())
  {
    throw std::invalid_argument("Input topic ID string is malformed");
  }

  // Convert the realm number
  result = std::from_chars(realmStr.data(), realmStr.data() + realmStr.size(), topicId.mRealmNum);
  if (result.ec != std::errc() || result.ptr != realmStr.data() + realmStr.size())
  {
    throw std::invalid_argument("Input topic ID string is malformed");
  }

  // Convert the topic number
  result = std::from_chars(topicStr.data(), topicStr.data() + topicStr.size(), topicId.mTopicNum);
  if (result.ec != std::errc() || result.ptr != topicStr.data() + topicStr.size())
  {
    throw std::invalid_argument("Input topic ID string is malformed");
  }

  return topicId;
}

//-----
TopicId TopicId::fromSolidityAddress(std::string_view addr)
{
  TopicId topicId;

  if (addr.size() != 40)
  {
    throw std::invalid_argument("Input string doesn't represent a solidity address");
  }

  std::vector<std::byte> bytes;
  try
  {
    bytes = internal::HexConverter::hexToBytes(addr);
  }
  catch (const OpenSSLException&)
  {
    throw std::invalid_argument("Input string doesn't represent a solidity address");
  }

  // First four bytes are the shard ID, the next eight the realm ID, and the last eight are the topic ID.
  topicId.mShardNum = static_cast<uint64_t>(*internal::Utilities::toTypePtr<uint32_t>(bytes.data()));
  topicId.mRealmNum = *internal::Utilities::toTypePtr<uint64_t>(bytes.data() + 4);
  topicId.mTopicNum = *internal::Utilities::toTypePtr<uint64_t>(bytes.data() + 12);

  return topicId;
}

//-----
TopicId TopicId::fromProtobuf(const proto::TopicID& proto)
{
  return TopicId(static_cast<uint64_t>(proto.shardnum()),
                 static_cast<uint64_t>(proto.realmnum()),
                 static_cast<uint64_t>(proto.topicnum()));
}

//-----
TopicId TopicId::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::TopicID protoTopicId;
  protoTopicId.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(protoTopicId);
}

//-----
std::string TopicId::toSolidityAddress() const
{
  if (mShardNum > std::numeric_limits<uint32_t>::max())
  {
    throw IllegalStateException("Shard number requires more than 32 bits to serialize");
  }

  const auto shardNumPtr = internal::Utilities::toTypePtr<std::byte>(&mShardNum);
  const auto realmNumPtr = internal::Utilities::toTypePtr<std::byte>(&mRealmNum);
  const auto topicNumPtr = internal::Utilities::toTypePtr<std::byte>(&mTopicNum);

  return internal::HexConverter::bytesToHex(
    internal::Utilities::concatenateVectors({ std::vector<std::byte>(shardNumPtr, shardNumPtr + 4),
                                              std::vector<std::byte>(realmNumPtr, realmNumPtr + 8),
                                              std::vector<std::byte>(topicNumPtr, topicNumPtr + 8) }));
}

//-----
std::unique_ptr<proto::TopicID> TopicId::toProtobuf() const
{
  auto protoTopicId = std::make_unique<proto::TopicID>();
  protoTopicId->set_shardnum(static_cast<int64_t>(mShardNum));
  protoTopicId->set_realmnum(static_cast<int64_t>(mRealmNum));
  protoTopicId->set_topicnum(static_cast<int64_t>(mTopicNum));
  return protoTopicId;
}

//-----
std::vector<std::byte> TopicId::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string TopicId::toString() const
{
  return std::to_string(mShardNum) + '.' + std::to_string(mRealmNum) + '.' + std::to_string(mTopicNum);
}

} // namespace Hedera
