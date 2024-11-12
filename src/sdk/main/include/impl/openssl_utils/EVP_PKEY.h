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
#ifndef HIERO_SDK_CPP_IMPL_OPENSSL_UTILS_EVP_PKEY_H_
#define HIERO_SDK_CPP_IMPL_OPENSSL_UTILS_EVP_PKEY_H_

#include "impl/openssl_utils/OpenSSLObjectWrapper.h"

#include <openssl/evp.h>

namespace Hiero::internal::OpenSSLUtils
{
/**
 * Wrapper class for the OpenSSL EVP_PKEY object.
 */
class EVP_PKEY : public OpenSSLObjectWrapper<::EVP_PKEY, decltype(&EVP_PKEY_dup)>
{
public:
  EVP_PKEY() = default;

  /**
   * Construct with the input EVP_PKEY, its EVP_PKEY_free deleter function, and its EVP_PKEY_dup copier function.
   *
   * @param evpPkey The EVP_PKEY OpenSSL object to wrap.
   */
  explicit EVP_PKEY(::EVP_PKEY* evpPkey)
    : OpenSSLObjectWrapper(evpPkey, &EVP_PKEY_free, &EVP_PKEY_dup)
  {
  }
};

} // namespace Hiero::internal::OpenSSLUtils

#endif // HIERO_SDK_CPP_IMPL_OPENSSL_UTILS_EVP_PKEY_H_
