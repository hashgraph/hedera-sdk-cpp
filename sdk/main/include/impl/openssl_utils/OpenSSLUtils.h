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
#ifndef HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_OPENSSL_UTILS_H_
#define HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_OPENSSL_UTILS_H_

#include <string>
#include <string_view>
#include <vector>

namespace Hedera::internal::OpenSSLUtils
{
/**
 * Computes the SHA256 hash of a byte array.
 *
 * @param data The byte array of which to compute the hash.
 * @return The SHA256 hash of the data.
 */
[[nodiscard]] std::vector<std::byte> computeSHA256(const std::vector<std::byte>& data);

/**
 * Compute the SHA384 hash of a byte array.
 *
 * @param data The byte array of which to compute the hash.
 * @return The SHA384 hash of the data.
 */
[[nodiscard]] std::vector<std::byte> computeSHA384(const std::vector<std::byte>& data);

/**
 * Compute the HMAC-SHA512 hash of a key and data.
 *
 * @param key  The key input to the hash function.
 * @param data The data input to the hash function.
 * @return The HMAC-SHA512 hash of the data and key.
 * @throws OpenSSLException If OpenSSL is unable to compute the HMAC-SHA512 hash of the given inputs.
 */
[[nodiscard]] std::vector<std::byte> computeSHA512HMAC(const std::vector<std::byte>& key,
                                                       const std::vector<std::byte>& data);

/**
 * Gets an error message for an OpenSSL error. Includes as much detail as possible.
 *
 * @param functionName The name of the OpenSSL function which caused the error.
 * @return An error string.
 */
[[nodiscard]] std::string getErrorMessage(std::string_view functionName);

/**
 * Get a vector of random bytes.
 *
 * @param count The number of random bytes to generate. Must be positive.
 * @return The vector of random bytes.
 * @throws std::invalid_argument If the input count is negative.
 * @throws OpenSSLException If OpenSSL is unable to generate random bytes.
 */
[[nodiscard]] std::vector<std::byte> getRandomBytes(int count);

} // namespace Hedera::internal::OpenSSLUtils

#endif // HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_OPENSSL_UTILS_H_
