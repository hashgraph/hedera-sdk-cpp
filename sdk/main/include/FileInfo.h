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
#ifndef HEDERA_SDK_CPP_FILE_INFO_H_
#define HEDERA_SDK_CPP_FILE_INFO_H_

#include "FileId.h"
#include "KeyList.h"
#include "LedgerId.h"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

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
   * Construct a FileInfo object from a byte array.
   *
   * @param bytes The byte array from which to construct a FileInfo object.
   * @return The constructed FileInfo object.
   */
  [[nodiscard]] static FileInfo fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a FileGetInfoResponse_FileInfo protobuf object from this FileInfo object.
   *
   * @return A pointer to the created FileGetInfoResponse_FileInfo protobuf object.
   */
  [[nodiscard]] std::unique_ptr<proto::FileGetInfoResponse_FileInfo> toProtobuf() const;

  /**
   * Construct a representative byte array from this FileInfo object.
   *
   * @return The byte array representing this FileInfo object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Construct a string representation of this FileInfo object.
   *
   * @return The string representation of this FileInfo object.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Write this FileInfo to an output stream.
   *
   * @param os   The output stream.
   * @param info The FileInfo to print.
   * @return The output stream with this FileInfo written to it.
   */
  friend std::ostream& operator<<(std::ostream& os, const FileInfo& info);

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
