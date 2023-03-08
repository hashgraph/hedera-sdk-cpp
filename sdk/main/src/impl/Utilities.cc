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
//-----
bool isPrefixOf(const std::vector<unsigned char>& bytes, const std::vector<unsigned char>& prefix)
{
  if (prefix.size() > bytes.size())
  {
    return false;
  }

  for (size_t i = 0; i < prefix.size(); ++i)
  {
    if (bytes.at(i) != prefix.at(i))
    {
      return false;
    }
  }

  return true;
}

//-----
std::vector<unsigned char> removePrefix(const std::vector<unsigned char>& bytes, long num)
{
  return { bytes.cbegin() + num, bytes.cend() };
}

//-----
std::vector<unsigned char> concatenateVectors(const std::vector<unsigned char>& first,
                                              const std::vector<unsigned char>& second)
{
  std::vector<unsigned char> bytes = first;
  bytes.insert(bytes.end(), second.cbegin(), second.cend());
  return bytes;
}

//-----
std::vector<unsigned char> concatenateVectors(const std::vector<unsigned char>& first,
                                              const std::vector<unsigned char>& second,
                                              const std::vector<unsigned char>& third)
{
  return concatenateVectors(concatenateVectors(first, second), third);
}

} // namespace Hedera::internal::Utilities

#endif // HEDERA_SDK_CPP_IMPL_UTILITIES_H_
