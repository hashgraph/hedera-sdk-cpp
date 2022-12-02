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

#ifndef HEDERA_SDK_CPP_OPENSSLHASHER_H
#define HEDERA_SDK_CPP_OPENSSLHASHER_H

#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

namespace Hedera
{

/**
 * Utility class wrapper around openssl hashing functions
 */
class OpenSSLHasher
{
public:
  /**
   * Computes the SHA384 hash of a string
   *
   * @param data the data to compute the hash of
   *
   * @return the hash of the data
   */
  [[nodiscard]] static std::vector<unsigned char> computeSHA384(const std::string& data);

  /**
   * Computes the SHA256 hash of a byte array
   *
   * @param data the data to compute the hash of
   *
   * @return the hash of the data
   */
  [[nodiscard]] static std::vector<unsigned char> computeSHA256(const std::vector<unsigned char>& data);

  /**
   * Computes the HMAC-SHA512 hash of a key and data
   *
   * @param key the key input to the hash function
   * @param data the data input to the hash function
   *
   * @return the result of the hash function
   */
  [[nodiscard]] static std::vector<unsigned char> computeSHA512HMAC(const std::vector<unsigned char>& key,
                                                                    const std::vector<unsigned char>& data);
};

} // Hedera

#endif // HEDERA_SDK_CPP_OPENSSLHASHER_H
