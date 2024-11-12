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
#ifndef HIERO_SDK_CPP_IMPL_OPENSSL_UTILS_EC_POINT_H_
#define HIERO_SDK_CPP_IMPL_OPENSSL_UTILS_EC_POINT_H_

#include "impl/openssl_utils/OpenSSLObjectWrapper.h"

#include <openssl/ec.h>

namespace Hiero::internal::OpenSSLUtils
{
/**
 * Wrapper class for the OpenSSL EC_POINT object.
 */
class EC_POINT : public OpenSSLObjectWrapper<::EC_POINT>
{
public:
  /**
   * No custom copier provided by OpenSSL for EC_POINT.
   */
  ~EC_POINT() override = default;
  EC_POINT(const EC_POINT&) = delete;
  EC_POINT& operator=(const EC_POINT&) = delete;
  EC_POINT(EC_POINT&&) = default;
  EC_POINT& operator=(EC_POINT&&) = default;

  /**
   * Construct with the input EC_POINT and its EC_POINT_free deleter function.
   *
   * @param ecPoint The EC_POINT OpenSSL object to wrap.
   */
  explicit EC_POINT(::EC_POINT* ecPoint)
    : OpenSSLObjectWrapper(ecPoint, &EC_POINT_free)
  {
  }
};

} // namespace Hiero::internal::OpenSSLUtils

#endif // HIERO_SDK_CPP_IMPL_OPENSSL_UTILS_EC_POINT_H_
