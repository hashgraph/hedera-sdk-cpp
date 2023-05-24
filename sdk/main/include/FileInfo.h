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
#ifndef HEDERA_SDK_CPP_FILE_INFO_H_
#define HEDERA_SDK_CPP_FILE_INFO_H_

#include "FileId.h"
#include "KeyList.h"
#include "LedgerId.h"

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

namespace proto
{
class FileGetInfoResponse_FileInfo;
}

namespace Hedera
{
/**
 * Contains information about a file instance.
 */
class FileInfo
{
public:
  /**
   * Construct a FileInfo object from a FileGetInfoResponse_FileInfo protobuf object.
   *
   * @param proto The FileGetInfoResponse_FileInfo protobuf object from which to construct a FileInfo object.
   * @return The constructed FileInfo object.
   */
  [[nodiscard]] static FileInfo fromProtobuf(const proto::FileGetInfoResponse_FileInfo& proto);

  /**
   * The ID of the file.
   */
  FileId mFileId;

  /**
   * The file size, in bytes.
   */
  uint64_t mSize = 0ULL;

  /**
   * The time at which this file is set to expire.
   */
  std::chrono::system_clock::time_point mExpirationTime;

  /**
   * Is this file deleted, but not yet expired?
   */
  bool mIsDeleted = false;

  /**
   * The admin keys for the file.
   */
  KeyList mAdminKeys;

  /**
   * The memo associated with the file.
   */
  std::string mMemo;

  /**
   * The ID of the ledger on which this file exists.
   */
  LedgerId mLedgerId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_FILE_INFO_H_
