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
#ifndef HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_OSSL_DECODER_CTX_H_
#define HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_OSSL_DECODER_CTX_H_

#include "impl/openssl_utils/OpenSSLObjectWrapper.h"

#include <openssl/decoder.h>

namespace Hedera::internal::OpenSSLUtils
{
/**
 * Wrapper class for the OpenSSL OSSL_DECODER_CTX object.
 */
class OSSL_DECODER_CTX : public OpenSSLObjectWrapper<::OSSL_DECODER_CTX>
{
public:
  /**
   * No custom copier provided by OpenSSL for OSSL_LIB_CTX.
   */
  ~OSSL_DECODER_CTX() override = default;
  OSSL_DECODER_CTX(const OSSL_DECODER_CTX&) = delete;
  OSSL_DECODER_CTX& operator=(const OSSL_DECODER_CTX&) = delete;
  OSSL_DECODER_CTX(OSSL_DECODER_CTX&&) = default;
  OSSL_DECODER_CTX& operator=(OSSL_DECODER_CTX&&) = default;

  /**
   * Construct with the input OSSL_DECODER_CTX and its OSSL_DECODER_CTX_free deleter function.
   *
   * @param osslDecoderCtx The OSSL_DECODER_CTX OpenSSL object to wrap.
   */
  explicit OSSL_DECODER_CTX(::OSSL_DECODER_CTX* osslDecoderCtx)
    : OpenSSLObjectWrapper(osslDecoderCtx, &OSSL_DECODER_CTX_free)
  {
  }
};

} // namespace Hedera::internal::OpenSSLUtils

#endif // HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_OSSL_DECODER_CTX_H_
