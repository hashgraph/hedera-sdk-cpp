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
#ifndef HEDERA_SDK_CPP_IMPL_HEX_CONVERTER_H_
#define HEDERA_SDK_CPP_IMPL_HEX_CONVERTER_H_

#include <string>
#include <vector>

namespace Hedera::internal::HexConverter
{
/**
 * Convert an array of base64 bytes to their hex values and put them in a string.
 *
 * @param bytes The bytes from which to get the hex string.
 * @return A string containing the hex values of the input byte array.
 * @throws OpenSSLException If unable to convert the input bytes to hex.
 */
std::string base64ToHex(const std::vector<unsigned char>& bytes);

/**
 * Convert a hex string to the array of base64 bytes it represents.
 *
 * @param inputString The hex string to convert to a byte array.
 * @return The byte array containing the values of the input hex string.
 * @throws OpenSSLException If unable to convert the input string to a byte array.
 */
std::vector<unsigned char> hexToBase64(const std::string& inputString);

/**
 * Convert an array of bytes to their hex values and put them in a string.
 *
 * @param bytes The bytes from which to get the hex string.
 * @return A string containing the hex values of the input byte array.
 */
std::string bytesToHex(const std::vector<unsigned char>& bytes);
} // namespace Hedera::internal::HexConverter

#endif // HEDERA_SDK_CPP_IMPL_HEX_CONVERTER_H_
