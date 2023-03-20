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
#ifndef HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_EVP_PKEY_CTX_H_
#define HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_EVP_PKEY_CTX_H_

#include "impl/openssl_utils/OpenSSLObjectWrapper.h"

#include <openssl/evp.h>

namespace Hedera::internal::OpenSSLUtils
{
/**
 * Wrapper class for the OpenSSL EVP_PKEY_CTX object.
 */
class EVP_PKEY_CTX : public OpenSSLObjectWrapper<::EVP_PKEY_CTX>
{
public:
  /**
   * Construct with the input EVP_PKEY_CTX, its EVP_PKEY_CTX_free deleter function, and its EVP_PKEY_CTX_dup copier
   * function.
   *
   * @param evpPkeyCtx The EVP_PKEY_CTX OpenSSL object to wrap.
   */
  explicit EVP_PKEY_CTX(::EVP_PKEY_CTX* evpPkeyCtx)
    : OpenSSLObjectWrapper(evpPkeyCtx, &EVP_PKEY_CTX_free, &EVP_PKEY_CTX_dup)
  {
  }
};

} // namespace Hedera::internal::OpenSSLUtils

#endif // HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_EVP_PKEY_CTX_H_
