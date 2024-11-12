/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#ifndef HIERO_SDK_CPP_IMPL_HEX_CONVERTER_H_
#define HIERO_SDK_CPP_IMPL_HEX_CONVERTER_H_

#include <string>
#include <string_view>
#include <vector>

namespace Hiero::internal::HexConverter
{
/**
 * Convert an array of bytes to a hex string.
 *
 * @param bytes The bytes from which to get the hex string.
 * @return A string containing the hex values of the input byte array.
 * @throws OpenSSLException If OpenSSL is unable to convert the input bytes to hex.
 */
std::string bytesToHex(const std::vector<std::byte>& bytes);

/**
 * Convert a hex string to the array of bytes it represents.
 *
 * @param hex The hex string to convert to a byte array.
 * @return The byte array containing the values of the input hex string.
 * @throws OpenSSLException If OpenSSL is unable to convert the input string to a byte array.
 */
std::vector<std::byte> hexToBytes(std::string_view hex);

/**
 * Convert a Base64-encoded string to a hexadecimal string.
 *
 * This function takes a Base64-encoded string as input and returns a hexadecimal
 * string.
 *
 * @param base64 The Base64-encoded string to convert.
 * @return The hexadecimal representation of the input.
 */
std::string base64ToHex(std::string_view base64);

} // namespace Hiero::internal::HexConverter

#endif // HIERO_SDK_CPP_IMPL_HEX_CONVERTER_H_
