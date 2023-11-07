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
#ifndef HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_OSSL_LIB_CTX_H_
#define HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_OSSL_LIB_CTX_H_

#include "impl/openssl_utils/OpenSSLObjectWrapper.h"

#include <openssl/crypto.h>

namespace Hedera::internal::OpenSSLUtils
{
/**
 * Wrapper class for the OpenSSL OSSL_LIB_CTX object.
 */
class OSSL_LIB_CTX : public OpenSSLObjectWrapper<::OSSL_LIB_CTX>
{
public:
  /**
   * No custom copier provided by OpenSSL for OSSL_LIB_CTX.
   */
  ~OSSL_LIB_CTX() override = default;
  OSSL_LIB_CTX(const OSSL_LIB_CTX&) = delete;
  OSSL_LIB_CTX& operator=(const OSSL_LIB_CTX&) = delete;
  OSSL_LIB_CTX(OSSL_LIB_CTX&&) = default;
  OSSL_LIB_CTX& operator=(OSSL_LIB_CTX&&) = default;

  /**
   * Construct with the input OSSL_LIB_CTX and its OSSL_LIB_CTX_free deleter function.
   *
   * @param osslLibCtx The OSSL_LIB_CTX OpenSSL object to wrap.
   */
  explicit OSSL_LIB_CTX(::OSSL_LIB_CTX* osslLibCtx)
    : OpenSSLObjectWrapper(osslLibCtx, &OSSL_LIB_CTX_free)
  {
  }
};

} // namespace Hedera::internal::OpenSSLUtils

#endif // HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_OSSL_LIB_CTX_H_
