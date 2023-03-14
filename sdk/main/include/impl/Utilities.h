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
#ifndef HEDERA_SDK_CPP_IMPL_UTILITIES_H_
#define HEDERA_SDK_CPP_IMPL_UTILITIES_H_

#include <vector>

namespace Hedera::internal::Utilities
{
/**
 * Helper function used to determine if a byte array is the prefix of another.
 *
 * @param bytes  The bytes for which to look for the prefix.
 * @param prefix The prefix bytes to find in the byte array.
 * @return \c TRUE If prefix is a prefix of bytes, otherwise \c FALSE.
 */
[[nodiscard]] bool isPrefixOf(const std::vector<unsigned char>& bytes, const std::vector<unsigned char>& prefix);

/**
 * Remove a certain number of bytes from the beginning of a byte vector.
 *
 * @param bytes The bytes from which to remove the prefix.
 * @param num   The number of bytes to remove.
 * @return The byte vector with the removed prefix bytes.
 */
[[nodiscard]] std::vector<unsigned char> removePrefix(const std::vector<unsigned char>& bytes, long num);

/**
 * Concatenate bytes vectors together (in the order of the arguments).
 *
 * @param vectors  The bytes vectors to concatenate together.
 * @return A byte vector containing the input byte vectors concatenated into one.
 */
[[nodiscard]] std::vector<unsigned char> concatenateVectors(const std::vector<std::vector<unsigned char>>& vectors);

} // namespace Hedera::internal::Utilities

#endif // HEDERA_SDK_CPP_IMPL_UTILITIES_H_
