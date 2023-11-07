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
#include "impl/DerivationPathUtils.h"

#include <iostream>
#include <stdexcept>

namespace Hedera::internal::DerivationPathUtils
{
//-----
bool isHardenedChildIndex(uint32_t index)
{
  return index >> 31 == 0x1;
}

//-----
uint32_t getHardenedIndex(uint32_t index)
{
  if (isHardenedChildIndex(index))
  {
    throw std::invalid_argument("Index is already hardened");
  }

  return ~(UINT32_MAX >> 1) | index;
}

//-----
std::vector<std::byte> indexToBigEndianArray(uint32_t childIndex)
{
  std::vector<std::byte> indexVector;
  for (int byteIndex = 3; byteIndex >= 0; --byteIndex)
  {
    indexVector.push_back(std::byte((childIndex >> (byteIndex << 3)) & 0xFF));
  }

  return indexVector;
}

} // namespaceHedera::internal::DerivationPathUtils
