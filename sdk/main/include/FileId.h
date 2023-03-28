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
#ifndef HEDERA_SDK_CPP_FILE_ID_H_
#define HEDERA_SDK_CPP_FILE_ID_H_

#include <memory>
#include <string>
#include <string_view>

namespace proto
{
class FileID;
}

namespace Hedera
{
/**
 * The ID for a file on Hedera.
 */
class FileId
{
public:
  FileId() = default;

  /**
   * Construct with a file number.
   *
   * @param num The desired file number.
   * @throws std::invalid_argument If the file number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  explicit FileId(const uint64_t& num);

  /**
   * Construct with a shard, realm, and file number.
   *
   * @param shard The desired shard number.
   * @param realm The desired realm number.
   * @param num   The desired file number.
   * @throws std::invalid_argument If any number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  explicit FileId(const uint64_t& shard, const uint64_t& realm, const uint64_t& num);

  /**
   * Compare this FileId to another FileId and determine if they represent the same file.
   *
   * @param other The other FileId with which to compare this FileId.
   * @return \c TRUE if this FileId is the same as the input FileId, otherwise \c FALSE.
   */
  bool operator==(const FileId& other) const;

  /**
   * Construct an FileId object from a string of the form "<shard>.<realm>.<num>".
   *
   * @param id The file ID string from which to construct.
   * @return The constructed FileId object.
   * @throws std::invalid_argument If the input string is malformed.
   */
  [[nodiscard]] static FileId fromString(std::string_view id);

  /**
   * Create an FileId object from an FileId protobuf object.
   *
   * @param proto The FileId protobuf object from which to create an FileId object.
   * @return The constructed FileId object.
   */
  [[nodiscard]] static FileId fromProtobuf(const proto::FileID& proto);

  /**
   * Construct an FileId protobuf object from this FileId object.
   *
   * @return A pointer to the created FileId protobuf object filled with this FileId object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::FileID> toProtobuf() const;

  /**
   * Get a string representation of this FileId object with the form "<shard>.<realm>.<num>".
   *
   * @return A string representation of this FileId.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Set the shard number.
   *
   * @param num The desired shard number to set.
   * @return A reference to this FileId object with the newly-set shard number.
   * @throws std::invalid_argument If the shard number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  FileId& setShardNum(const uint64_t& num);

  /**
   * Set the realm number.
   *
   * @param num The realm number to set.
   * @return A reference to this FileId object with the newly-set realm number.
   * @throws std::invalid_argument If the realm number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  FileId& setRealmNum(const uint64_t& num);

  /**
   * Set the file number.
   *
   * @param num The file number to set.
   * @return A reference to this FileId object with the newly-set file number.
   * @throws std::invalid_argument If the account number is too big (max value is std::numeric_limits<int64_t>::max()).
   */
  FileId& setFileNum(const uint64_t& num);

  /**
   * Get the shard number.
   *
   * @return The shard number.
   */
  [[nodiscard]] inline uint64_t getShardNum() const { return mShardNum; }

  /**
   * Get the realm number.
   *
   * @return The realm number.
   */
  [[nodiscard]] inline uint64_t getRealmNum() const { return mRealmNum; }

  /**
   * Get the file number.
   *
   * @return The file number.
   */
  [[nodiscard]] inline uint64_t getFileNum() const { return mFileNum; }

private:
  /**
   * Parse the input string and attempt to put it into the input number.
   *
   * @param str The string to parse.
   * @param num The number in which to put the parsed string.
   * @throws std::invalid_argument If unable to parse the string and put it into the input number.
   */
  static void parseNum(std::string_view str, uint64_t& num);

  /**
   * Check if the shard, realm, or file numbers (respectively) are too big.
   *
   * @throws std::invalid_argument If the shard, realm, or file number (respectively) is too big.
   */
  void checkShardNum() const;
  void checkRealmNum() const;
  void checkFileNum() const;

  /**
   * The shard number.
   */
  uint64_t mShardNum = 0ULL;

  /**
   * The realm number.
   */
  uint64_t mRealmNum = 0ULL;

  /**
   * The file number.
   */
  uint64_t mFileNum = 0ULL;
};

} // namespace Hedera

namespace std
{
template<>
struct hash<Hedera::FileId>
{
  /**
   * Operator override to enable use of FileId as map key
   */
  size_t operator()(const Hedera::FileId& id) const { return hash<string>()(id.toString()); }
};

} // namespace std

#endif // HEDERA_SDK_CPP_FILE_ID_H_
