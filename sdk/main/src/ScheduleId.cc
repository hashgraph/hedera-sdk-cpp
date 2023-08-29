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
#include "ScheduleId.h"
#include "exceptions/IllegalStateException.h"
#include "impl/Utilities.h"

#include <charconv>
#include <limits>
#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
ScheduleId::ScheduleId(const uint64_t& num)
  : mScheduleNum(num)
{
}

//-----
ScheduleId::ScheduleId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num)
  : mShardNum(shard)
  , mRealmNum(realm)
  , mScheduleNum(num)
{
}

//-----
bool ScheduleId::operator==(const ScheduleId& other) const
{
  return (mShardNum == other.mShardNum) && (mRealmNum == other.mRealmNum) && (mScheduleNum == other.mScheduleNum);
}

//-----
ScheduleId ScheduleId::fromString(std::string_view id)
{
  ScheduleId topicId;

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
  result = std::from_chars(topicStr.data(), topicStr.data() + topicStr.size(), topicId.mScheduleNum);
  if (result.ec != std::errc() || result.ptr != topicStr.data() + topicStr.size())
  {
    throw std::invalid_argument("Input topic ID string is malformed");
  }

  return topicId;
}

//-----
ScheduleId ScheduleId::fromProtobuf(const proto::ScheduleID& proto)
{
  return ScheduleId(static_cast<uint64_t>(proto.shardnum()),
                    static_cast<uint64_t>(proto.realmnum()),
                    static_cast<uint64_t>(proto.schedulenum()));
}

//-----
ScheduleId ScheduleId::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::ScheduleID protoScheduleId;
  protoScheduleId.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(protoScheduleId);
}

//-----
std::unique_ptr<proto::ScheduleID> ScheduleId::toProtobuf() const
{
  auto protoScheduleId = std::make_unique<proto::ScheduleID>();
  protoScheduleId->set_shardnum(static_cast<int64_t>(mShardNum));
  protoScheduleId->set_realmnum(static_cast<int64_t>(mRealmNum));
  protoScheduleId->set_schedulenum(static_cast<int64_t>(mScheduleNum));
  return protoScheduleId;
}

//-----
std::vector<std::byte> ScheduleId::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string ScheduleId::toString() const
{
  return std::to_string(mShardNum) + '.' + std::to_string(mRealmNum) + '.' + std::to_string(mScheduleNum);
}

} // namespace Hedera
