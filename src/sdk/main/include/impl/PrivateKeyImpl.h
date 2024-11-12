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
#ifndef HIERO_SDK_CPP_IMPL_PRIVATE_KEY_IMPL_H_
#define HIERO_SDK_CPP_IMPL_PRIVATE_KEY_IMPL_H_

#include "PrivateKey.h"
#include "impl/openssl_utils/EVP_PKEY.h"

#include <memory>
#include <vector>

namespace Hiero
{
/**
 * The implementation of the PrivateKey object.
 */
struct PrivateKey::PrivateKeyImpl
{
  /**
   * The wrapped OpenSSL key object.
   */
  internal::OpenSSLUtils::EVP_PKEY mKey;

  /**
   * The chain code. If this is empty, then this PrivateKey will not support derivation.
   */
  std::vector<std::byte> mChainCode;

  /**
   * A pointer to the PublicKey object that corresponds to this PrivateKey.
   */
  std::shared_ptr<PublicKey> mPublicKey = nullptr;
};

} // namespace Hiero

#endif // HIERO_SDK_CPP_IMPL_PRIVATE_KEY_IMPL_H_
