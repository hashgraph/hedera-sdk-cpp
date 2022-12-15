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
#ifndef HEDERA_SDK_CPP_IMPL_OPENSSL_HASHER_H_
#define HEDERA_SDK_CPP_IMPL_OPENSSL_HASHER_H_

#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

namespace Hedera::internal::OpenSSLHasher
{
/**
 * Compute the SHA384 hash of a string.
 *
 * @param data The string of which to compute the hash.
 * @return The hash of the data.
 */
[[nodiscard]] std::vector<unsigned char> computeSHA384(const std::string& data);

/**
 * Computes the SHA256 hash of a byte array.
 *
 * @param data The data of which to compute the hash.
 * @return The hash of the data.
 */
[[nodiscard]] std::vector<unsigned char> computeSHA256(const std::vector<unsigned char>& data);

/**
 * Compute the HMAC-SHA512 hash of a key and data.
 *
 * @param key  The key input to the hash function.
 * @param data The data input to the hash function.
 * @return The hash of the data and key.
 * @throws std::runtime_error If OpenSSL has can't compute the HMAC-SHA512 hash for whatever reason.
 */
[[nodiscard]] std::vector<unsigned char> computeSHA512HMAC(const std::vector<unsigned char>& key,
                                                           const std::vector<unsigned char>& data);

/**
 * Gets an error message for an OpenSSL error. Includes as much detail as possible
 *
 * @param functionName the name of the OpenSSL function which caused the error
 *
 * @return an error string
 */
[[nodiscard]] std::string getOpenSSLErrorMessage(const std::string& functionName);
} // namespace Hedera::internal::OpenSSLHasher

#endif // HEDERA_SDK_CPP_IMPL_OPENSSL_HASHER_H_
