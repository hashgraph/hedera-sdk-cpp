/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "FileInfo.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <file_get_info.pb.h>
#include <nlohmann/json.hpp>

namespace Hiero
{
//-----
FileInfo FileInfo::fromProtobuf(const proto::FileGetInfoResponse_FileInfo& proto)
{
  FileInfo fileInfo;

  if (proto.has_fileid())
  {
    fileInfo.mFileId = FileId::fromProtobuf(proto.fileid());
  }

  fileInfo.mSize = static_cast<uint64_t>(proto.size());

  if (proto.has_expirationtime())
  {
    fileInfo.mExpirationTime = internal::TimestampConverter::fromProtobuf(proto.expirationtime());
  }

  fileInfo.mIsDeleted = proto.deleted();

  if (proto.has_keys())
  {
    fileInfo.mAdminKeys = KeyList::fromProtobuf(proto.keys());
  }

  fileInfo.mMemo = proto.memo();
  fileInfo.mLedgerId = LedgerId(internal::Utilities::stringToByteVector(proto.ledger_id()));

  return fileInfo;
}

//-----
FileInfo FileInfo::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::FileGetInfoResponse_FileInfo proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
std::unique_ptr<proto::FileGetInfoResponse_FileInfo> FileInfo::toProtobuf() const
{
  auto proto = std::make_unique<proto::FileGetInfoResponse_FileInfo>();
  proto->set_allocated_fileid(mFileId.toProtobuf().release());
  proto->set_size(static_cast<int64_t>(mSize));
  proto->set_allocated_expirationtime(internal::TimestampConverter::toProtobuf(mExpirationTime));
  proto->set_deleted(mIsDeleted);
  proto->set_allocated_keys(mAdminKeys.toProtobuf().release());
  proto->set_memo(mMemo);
  proto->set_ledger_id(internal::Utilities::byteVectorToString(mLedgerId.toBytes()));
  return proto;
}

//-----
std::vector<std::byte> FileInfo::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string FileInfo::toString() const
{
  nlohmann::json json;
  json["mFileId"] = mFileId.toString();
  json["mSize"] = mSize;
  json["mExpirationTime"] = internal::TimestampConverter::toString(mExpirationTime);
  json["mIsDeleted"] = mIsDeleted;
  json["mAdminKeys"] = mAdminKeys.toString();
  json["mMemo"] = mMemo;
  json["mLedgerId"] = mLedgerId.toString();
  return json.dump();
}

//-----
std::ostream& operator<<(std::ostream& os, const FileInfo& info)
{
  os << info.toString();
  return os;
}

} // namespace Hiero
