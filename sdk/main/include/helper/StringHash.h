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
#ifndef HELPER_STRINGHASH_H_
#define HELPER_STRINGHASH_H_

#include <string_view>

namespace Hedera
{
/**
 * Helper transparent hasher object used during heterogeneous comparisons where a
 * std::string is used as key to prevent temporary std::string objects to be
 * created during comparison when using a non-std::string object as a key (such
 * as a char* or std::string_view).
 *
 * See SonarLint rule cpp:S6045 for further explanation and description.
 */
struct StringHash
{
  using is_transparent = void; // enables heterogeneous lookup

  ~StringHash() = default;

  std::size_t operator()(std::string_view sv) const
  {
    std::hash<std::string_view> hasher;
    return hasher(sv);
  }
};

} // namespace Hedera

#endif // HELPER_STRINGHASH_H_