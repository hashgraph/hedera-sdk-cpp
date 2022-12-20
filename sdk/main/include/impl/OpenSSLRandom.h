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
#ifndef HEDERA_SDK_CPP_IMPL_OPENSSL_RANDOM_H_
#define HEDERA_SDK_CPP_IMPL_OPENSSL_RANDOM_H_

#include <vector>
#include <functional>
#include <stdexcept>

/**
 * Utility class wrapping around OpenSSL random functions
 */
namespace Hedera::internal::OpenSSLRandom
{
/**
 * Get a vector of random bytes.
 *
 * @param count The number of random bytes to generate. Must be positive.
 * @return The vector of random bytes.
 * @throws std::invalid_argument If the input count is negative.
 * @throws std::runtime_error If OpenSSL fails to generate random bytes.
 */
std::vector<unsigned char> getRandomBytes(int count);

} // namespace Hedera::internal::OpenSSLRandom

#endif // HEDERA_SDK_CPP_IMPL_OPENSSL_RANDOM_H_
