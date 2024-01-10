/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_EC_GROUP_H_
#define HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_EC_GROUP_H_

#include "impl/openssl_utils/OpenSSLObjectWrapper.h"

#include <openssl/ec.h>

namespace Hedera::internal::OpenSSLUtils
{
/**
 * Wrapper class for the OpenSSL EC_GROUP object.
 */
class EC_GROUP : public OpenSSLObjectWrapper<::EC_GROUP>
{
public:
  /**
   * Construct with the input EC_GROUP, its EC_GROUP_free deleter, and its EC_GROUP_dup copier.
   *
   * @param ecGroup The EC_GROUP OpenSSL object to wrap.
   */
  explicit EC_GROUP(::EC_GROUP* ecGroup)
    : OpenSSLObjectWrapper(ecGroup, &EC_GROUP_free, &EC_GROUP_dup)
  {
  }
};

} // namespace Hedera::internal::OpenSSLUtils

#endif // HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_EC_GROUP_H_