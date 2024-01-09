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
#include "TopicId.h"
#include "Client.h"
#include "LedgerId.h"
#include "impl/EntityIdHelper.h"
#include "impl/Utilities.h"

#include <limits>
#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
TopicId::TopicId(uint64_t num)
  : mTopicNum(num)
{
}

//-----
TopicId::TopicId(uint64_t shard, uint64_t realm, uint64_t num, std::string_view checksum)
  : mShardNum(shard)
  , mRealmNum(realm)
  , mTopicNum(num)
  , mChecksum(checksum)
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
  return TopicId(internal::EntityIdHelper::getShardNum(id),
                 internal::EntityIdHelper::getRealmNum(id),
                 internal::EntityIdHelper::getEntityNum(id),
                 internal::EntityIdHelper::getChecksum(id));
}

//-----
TopicId TopicId::fromSolidityAddress(std::string_view address)
{
  return internal::EntityIdHelper::fromSolidityAddress<TopicId>(
    internal::EntityIdHelper::decodeSolidityAddress(address));
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
  proto::TopicID proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
void TopicId::validateChecksum(const Client& client) const
{
  if (!mChecksum.empty())
  {
    internal::EntityIdHelper::validate(mShardNum, mRealmNum, mTopicNum, client, mChecksum);
  }
}

//-----
std::unique_ptr<proto::TopicID> TopicId::toProtobuf() const
{
  auto body = std::make_unique<proto::TopicID>();
  body->set_shardnum(static_cast<int64_t>(mShardNum));
  body->set_realmnum(static_cast<int64_t>(mRealmNum));
  body->set_topicnum(static_cast<int64_t>(mTopicNum));
  return body;
}

//-----
std::string TopicId::toSolidityAddress() const
{
  return internal::EntityIdHelper::toSolidityAddress(mShardNum, mRealmNum, mTopicNum);
}

//-----
std::string TopicId::toString() const
{
  return internal::EntityIdHelper::toString(mShardNum, mRealmNum, mTopicNum);
}

//-----
std::string TopicId::toStringWithChecksum(const Client& client) const
{
  if (mChecksum.empty())
  {
    mChecksum = internal::EntityIdHelper::checksum(internal::EntityIdHelper::toString(mShardNum, mRealmNum, mTopicNum),
                                                   client.getLedgerId());
  }

  return internal::EntityIdHelper::toString(mShardNum, mRealmNum, mTopicNum, mChecksum);
}

//-----
std::vector<std::byte> TopicId::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

} // namespace Hedera
