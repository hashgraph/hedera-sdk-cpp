/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "FileId.h"

#include <charconv>
#include <limits>
#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
FileId::FileId(const uint64_t& num)
  : mFileNum(num)
{
  checkFileNum();
}

//-----
FileId::FileId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num)
  : mShardNum(shard)
  , mRealmNum(realm)
  , mFileNum(num)
{
  checkShardNum();
  checkRealmNum();
  checkFileNum();
}

//-----
bool FileId::operator==(const FileId& other) const
{
  return (mShardNum == other.mShardNum) && (mRealmNum == other.mRealmNum) && (mFileNum == other.mFileNum);
}

//-----
FileId FileId::fromString(std::string_view id)
{
  FileId fileId;

  // Get the indices of the two delimiter '.'
  const size_t firstDot = id.find_first_of('.');
  const size_t secondDot = id.find_last_of('.');

  // Make sure there are at least two dots
  if (firstDot == secondDot)
  {
    throw std::invalid_argument("Input file ID string is malformed");
  }

  // Convert the shard number
  parseNum(id.substr(0, firstDot), fileId.mShardNum);
  fileId.checkShardNum();

  // Convert the realm number
  parseNum(id.substr(firstDot + 1, secondDot - firstDot - 1), fileId.mRealmNum);
  fileId.checkRealmNum();

  // Convert the shard number
  parseNum(id.substr(secondDot + 1, id.size() - secondDot - 1), fileId.mFileNum);
  fileId.checkFileNum();

  return fileId;
}

//-----
FileId FileId::fromProtobuf(const proto::FileID& proto)
{
  return FileId(static_cast<uint64_t>(proto.shardnum()),
                static_cast<uint64_t>(proto.realmnum()),
                static_cast<uint64_t>(proto.filenum()));
}

//-----
std::unique_ptr<proto::FileID> FileId::toProtobuf() const
{
  auto body = std::make_unique<proto::FileID>();
  body->set_shardnum(static_cast<int64_t>(mShardNum));
  body->set_realmnum(static_cast<int64_t>(mRealmNum));
  body->set_filenum(static_cast<int64_t>(mFileNum));
  return body;
}

//-----
std::string FileId::toString() const
{
  return std::to_string(mShardNum) + '.' + std::to_string(mRealmNum) + '.' + std::to_string(mFileNum);
}

//-----
FileId& FileId::setShardNum(const uint64_t& num)
{
  mShardNum = num;
  checkShardNum();
  return *this;
}

//-----
FileId& FileId::setRealmNum(const uint64_t& num)
{
  mRealmNum = num;
  checkRealmNum();
  return *this;
}

//-----
FileId& FileId::setFileNum(const uint64_t& num)
{
  mFileNum = num;
  checkFileNum();
  return *this;
}

//-----
void FileId::checkShardNum() const
{
  if (mShardNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input shard number is too large");
  }
}

//-----
void FileId::checkRealmNum() const
{
  if (mRealmNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input realm number is too large");
  }
}

//-----
void FileId::checkFileNum() const
{
  if (mFileNum > std::numeric_limits<int64_t>::max())
  {
    throw std::invalid_argument("Input file number is too large");
  }
}

//-----
void FileId::parseNum(std::string_view str, uint64_t& num)
{
  if (std::from_chars_result result = std::from_chars(str.data(), str.data() + str.size(), num);
      result.ec != std::errc() || result.ptr != str.data() + str.size())
  {
    throw std::invalid_argument("Input file ID string is malformed");
  }
}

} // namespace Hedera
