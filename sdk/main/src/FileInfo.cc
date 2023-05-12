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
#include "FileInfo.h"
#include "exceptions/BadKeyException.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <proto/file_get_info.pb.h>

namespace Hedera
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

  if (proto.has_keys() && proto.keys().keys_size() > 0)
  {
    fileInfo.mKey = PublicKey::fromProtobuf(proto.keys().keys(0));
  }

  fileInfo.mMemo = proto.memo();
  fileInfo.mLedgerId = LedgerId(internal::Utilities::stringToByteVector(proto.ledger_id()));

  return fileInfo;
}

} // namespace Hedera
