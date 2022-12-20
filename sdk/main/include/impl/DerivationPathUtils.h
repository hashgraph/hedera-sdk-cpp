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
#ifndef HEDERA_SDK_CPP_DERIVATION_IMPL_PATH_UTILS_H_
#define HEDERA_SDK_CPP_DERIVATION_IMPL_PATH_UTILS_H_

#include <cstdint>
#include <vector>

/**
 * Utility functions dealing with wallet derivation paths, as described in BIP32, BIP44, and SLIP10
 */
namespace Hedera::internal::DerivationPathUtils
{
/**
 * Check if an index falls into the "unhardened" or "hardened" category.
 * Unhardened: [0, 2147483647]; Hardened: [2147483648, UINT32_MAX]
 *
 * @param index The index to check.
 * @return \c TRUE if the index is in the hardened range, otherwise \c FALSE.
 */
bool isHardenedChildIndex(uint32_t index);

/**
 * Convert an index into its hardened counterpart (e.g. 0 -> 2147483648, 1-> 2147483649, etc.).
 *
 * @param index The index of which to get the hardened counterpart.
 * @return The hardened index.
 * @throws std::invalid_argument If the index is already hardened.
 */
uint32_t getHardenedIndex(uint32_t index);

/**
 * Converts unsigned 32 bit int index into big endian byte array (direct implementation of ser32 function from BIP 32)
 *
 * @param childIndex The index to convert to a big endian byte array.
 * @return The big endian byte array representing the child index.
 */
std::vector<unsigned char> ser32(uint32_t childIndex);

}; // namespace Hedera::internal::DerivationPathUtils

#endif // HEDERA_SDK_CPP_DERIVATION_IMPL_PATH_UTILS_H_
