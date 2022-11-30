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

#ifndef HEDERA_SDK_CPP_DERIVATIONPATHUTILS_H
#define HEDERA_SDK_CPP_DERIVATIONPATHUTILS_H

#include <cstdint>

namespace Hedera
{

/**
 * Utility classes dealing with wallet derivation paths, as described in BIP32, BIP44, and SLIP10
 */
class DerivationPathUtils
{
public:
  DerivationPathUtils() = delete;

  /**
   * Checks whether an integer falls into the "unhardened", or "hardened" category
   * <p>
   * Unhardened: [0, 2147483647]; Hardened: [2147483648, max]
   *
   * @param index the index to check
   *
   * @return true if the index is in the hardened range, otherwise false
   */
  static bool isHardenedChildIndex(uint32_t index);

  /**
   * Converts an index into its hardened counterpart. Throws an exception if the index is already hardened
   * <p>
   * 0 -> 2147483648, 1-> 2147483649, etc.
   *
   * @param standardIndex the index to convert
   *
   * @return the hardened index
   */
  static uint32_t getHardenedIndex(uint32_t standardIndex);
};

} // Hedera

#endif // HEDERA_SDK_CPP_DERIVATIONPATHUTILS_H
