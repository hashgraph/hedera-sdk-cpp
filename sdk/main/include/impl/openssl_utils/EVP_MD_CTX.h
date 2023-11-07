/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_EVP_MD_CTX_H_
#define HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_EVP_MD_CTX_H_

#include "impl/openssl_utils/OpenSSLObjectWrapper.h"

#include <openssl/evp.h>

namespace Hedera::internal::OpenSSLUtils
{
/**
 * Wrapper class for the OpenSSL EVP_MD_CTX object.
 */
class EVP_MD_CTX : public OpenSSLObjectWrapper<::EVP_MD_CTX>
{
public:
  /**
   * No custom copier provided by OpenSSL for EVP_MD_CTX.
   */
  ~EVP_MD_CTX() override = default;
  EVP_MD_CTX(const EVP_MD_CTX&) = delete;
  EVP_MD_CTX& operator=(const EVP_MD_CTX&) = delete;
  EVP_MD_CTX(EVP_MD_CTX&&) = default;
  EVP_MD_CTX& operator=(EVP_MD_CTX&&) = default;

  /**
   * Construct with the input EVP_MD_CTX and its EVP_MD_CTX_free deleter function.
   *
   * @param evpMdCtx The EVP_MD_CTX OpenSSL object to wrap.
   */
  explicit EVP_MD_CTX(::EVP_MD_CTX* evpMdCtx)
    : OpenSSLObjectWrapper(evpMdCtx, &EVP_MD_CTX_free)
  {
  }
};

} // namespace Hedera::internal::OpenSSLUtils

#endif // HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_EVP_MD_CTX_H_
