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
#ifndef HEDERA_SDK_CPP_BAD_ENTITY_ID_EXCEPTION_H_
#define HEDERA_SDK_CPP_BAD_ENTITY_ID_EXCEPTION_H_

#include <exception>
#include <string>
#include <string_view>

namespace Hedera
{
/**
 * Exception that is thrown when a key could not be realized from a given input.
 */
class BadEntityIdException : public std::exception
{
public:
  /**
   * Construct with an entity, its expected checksum, and its calculated checksum.
   *
   * @param shard              The shard of the entity.
   * @param realm              The realm of the entity.
   * @param num                The num of the entity.
   * @param expectedChecksum   The checksum against which the calculated checksum was validated.
   * @param calculatedChecksum The checksum that was calculated of the input entity.
   */
  explicit BadEntityIdException(uint64_t shard,
                                uint64_t realm,
                                uint64_t num,
                                std::string_view expectedChecksum,
                                std::string_view calculatedChecksum)
    : mError(std::string("Expected checksum ") + expectedChecksum.data() + " for entity " + std::to_string(shard) +
             '.' + std::to_string(realm) + '.' + std::to_string(num) + " does not match its calculated checksum " +
             calculatedChecksum.data())
  {
  }

  /**
   * Get the descriptor message for this error.
   *
   * @return The descriptor message for this error.
   */
  [[nodiscard]] const char* what() const noexcept override { return mError.data(); };

private:
  /**
   * Descriptive error message.
   */
  std::string mError;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_BAD_ENTITY_ID_EXCEPTION_H_
