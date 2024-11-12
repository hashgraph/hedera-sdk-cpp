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
#ifndef HIERO_SDK_CPP_IMPL_OPENSSL_UTILS_ECDSA_SIG_H_
#define HIERO_SDK_CPP_IMPL_OPENSSL_UTILS_ECDSA_SIG_H_

#include "impl/openssl_utils/OpenSSLObjectWrapper.h"

#include <openssl/ecdsa.h>

namespace Hiero::internal::OpenSSLUtils
{
/**
 * Wrapper class for the OpenSSL ECDSA_SIG object.
 */
class ECDSA_SIG : public OpenSSLObjectWrapper<::ECDSA_SIG>
{
public:
  /**
   * No custom copier provided by OpenSSL for ECDSA_SIG.
   */
  ~ECDSA_SIG() override = default;
  ECDSA_SIG(const ECDSA_SIG&) = delete;
  ECDSA_SIG& operator=(const ECDSA_SIG&) = delete;
  ECDSA_SIG(ECDSA_SIG&&) = default;
  ECDSA_SIG& operator=(ECDSA_SIG&&) = default;

  /**
   * Construct with the input ECDSA_SIG and its ECDSA_SIG_free deleter function.
   *
   * @param ecdsaSig The ECDSA_SIG OpenSSL object to wrap.
   */
  explicit ECDSA_SIG(::ECDSA_SIG* ecdsaSig)
    : OpenSSLObjectWrapper(ecdsaSig, &ECDSA_SIG_free)
  {
  }
};

} // namespace Hiero::internal::OpenSSLUtils

#endif // HIERO_SDK_CPP_IMPL_OPENSSL_UTILS_ECDSA_SIG_H_
