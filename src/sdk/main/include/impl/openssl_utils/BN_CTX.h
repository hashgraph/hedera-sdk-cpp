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
#ifndef HIERO_SDK_CPP_IMPL_OPENSSL_UTILS_BN_CTX_H_
#define HIERO_SDK_CPP_IMPL_OPENSSL_UTILS_BN_CTX_H_

#include "impl/openssl_utils/OpenSSLObjectWrapper.h"

#include <openssl/bn.h>

namespace Hiero::internal::OpenSSLUtils
{
/**
 * Wrapper class for the OpenSSL BN_CTX object.
 */
class BN_CTX : public OpenSSLObjectWrapper<::BN_CTX>
{
public:
  /**
   * No custom copier provided by OpenSSL for BN_CTX, so copying is disallowed.
   */
  ~BN_CTX() override = default;
  BN_CTX(const BN_CTX&) = delete;
  BN_CTX& operator=(const BN_CTX&) = delete;
  BN_CTX(BN_CTX&&) = default;
  BN_CTX& operator=(BN_CTX&&) = default;

  /**
   * Construct with the input BN_CTX and its BN_CTX_free deleter function.
   *
   * @param bnCtx The BN_CTX OpenSSL object to wrap.
   */
  explicit BN_CTX(::BN_CTX* bnCtx)
    : OpenSSLObjectWrapper(bnCtx, &BN_CTX_free)
  {
  }
};

} // namespace Hiero::internal::OpenSSLUtils

#endif // HIERO_SDK_CPP_IMPL_OPENSSL_UTILS_BN_CTX_H_