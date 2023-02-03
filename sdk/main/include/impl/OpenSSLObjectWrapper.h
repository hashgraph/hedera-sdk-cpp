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
#ifndef HEDERA_SDK_CPP_IMPL_OPENSSL_OBJECT_WRAPPER_H_
#define HEDERA_SDK_CPP_IMPL_OPENSSL_OBJECT_WRAPPER_H_

#include <memory>
#include <openssl/decoder.h>
#include <openssl/ec.h>
#include <openssl/evp.h>

namespace Hedera::internal
{
/**
 * Templated base wrapper class to be used for OpenSSL objects that require custom deleter functions.
 *
 * @tparam ObjectType  The type of OpenSSL object this class should wrap.
 * @tparam DeleterType The deleter type for the OpenSSL object.
 */
template<typename ObjectType, typename DeleterType>
class OpenSSLObjectWrapper
{
public:
  /**
   * Get the wrapped OpenSSL object.
   *
   * @return A pointer to the wrapped OpenSSL object, nullptr if no object exists.
   */
  [[nodiscard]] ObjectType* get() const { return mObject.get(); }

  /**
   * Determine if this OpenSSLObjectWrapper has a valid OpenSSL object.
   *
   * @return \c TRUE if there exists an OpenSSL object, otherwise \c FALSE.
   */
  [[nodiscard]] explicit operator bool() const { return mObject != nullptr; }

protected:
  /**
   * Construct with values for the object and its custom deleter.
   *
   * @param object  The OpenSSL object to wrap.
   * @param deleter The custom deleter for the OpenSSL object.
   */
  OpenSSLObjectWrapper(ObjectType* object, DeleterType deleter)
    : mObject({ object, deleter })
  {
  }

private:
  /**
   * Pointer to the OpenSSL object with its associated deleter.
   */
  std::unique_ptr<ObjectType, DeleterType> mObject = { nullptr, DeleterType() };
};

/**
 * Wrapper class for the OpenSSL ECDSA_SIG object.
 */
class OpenSSL_ECDSA_SIG : public OpenSSLObjectWrapper<ECDSA_SIG, decltype(&ECDSA_SIG_free)>
{
public:
  /**
   * Default constructor sets the ECDSA_SIG_free deleter function.
   */
  OpenSSL_ECDSA_SIG()
    : OpenSSLObjectWrapper(nullptr, &ECDSA_SIG_free)
  {
  }

  /**
   * Construct with the input ECDSA_SIG and its ECDSA_SIG_free deleter function.
   *
   * @param evpPkey The ECDSA_SIG OpenSSL object to wrap.
   */
  explicit OpenSSL_ECDSA_SIG(ECDSA_SIG* ecdsaSig)
    : OpenSSLObjectWrapper(ecdsaSig, &ECDSA_SIG_free)
  {
  }
};

/**
 * Wrapper class for the OpenSSL EVP_MD object.
 */
class OpenSSL_EVP_MD : public OpenSSLObjectWrapper<EVP_MD, decltype(&EVP_MD_free)>
{
public:
  /**
   * Default constructor sets the EVP_MD_free deleter function.
   */
  OpenSSL_EVP_MD()
    : OpenSSLObjectWrapper(nullptr, &EVP_MD_free)
  {
  }

  /**
   * Construct with the input EVP_MD and its EVP_MD_free deleter function.
   *
   * @param evpPkey The EVP_MD OpenSSL object to wrap.
   */
  explicit OpenSSL_EVP_MD(EVP_MD* evpMd)
    : OpenSSLObjectWrapper(evpMd, &EVP_MD_free)
  {
  }
};

/**
 * Wrapper class for the OpenSSL EVP_MD_CTX object.
 */
class OpenSSL_EVP_MD_CTX : public OpenSSLObjectWrapper<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)>
{
public:
  /**
   * Default constructor sets the EVP_MD_CTX_free deleter function.
   */
  OpenSSL_EVP_MD_CTX()
    : OpenSSLObjectWrapper(nullptr, &EVP_MD_CTX_free)
  {
  }

  /**
   * Construct with the input EVP_MD_CTX and its EVP_MD_CTX_free deleter function.
   *
   * @param evpPkey The EVP_MD_CTX OpenSSL object to wrap.
   */
  explicit OpenSSL_EVP_MD_CTX(EVP_MD_CTX* evpMdCtx)
    : OpenSSLObjectWrapper(evpMdCtx, &EVP_MD_CTX_free)
  {
  }
};

/**
 * Wrapper class for the OpenSSL EVP_PKEY object.
 */
class OpenSSL_EVP_PKEY : public OpenSSLObjectWrapper<EVP_PKEY, decltype(&EVP_PKEY_free)>
{
public:
  /**
   * Default constructor sets the EVP_PKEY_free deleter function.
   */
  OpenSSL_EVP_PKEY()
    : OpenSSLObjectWrapper(nullptr, &EVP_PKEY_free)
  {
  }

  /**
   * Construct with the input EVP_PKEY and its EVP_PKEY_free deleter function.
   *
   * @param evpPkey The EVP_PKEY OpenSSL object to wrap.
   */
  explicit OpenSSL_EVP_PKEY(EVP_PKEY* evpPkey)
    : OpenSSLObjectWrapper(evpPkey, &EVP_PKEY_free)
  {
  }
};

/**
 * Wrapper class for the OpenSSL EVP_PKEY_CTX object.
 */
class OpenSSL_EVP_PKEY_CTX : public OpenSSLObjectWrapper<EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)>
{
public:
  /**
   * Default constructor sets the EVP_PKEY_CTX_free deleter function.
   */
  OpenSSL_EVP_PKEY_CTX()
    : OpenSSLObjectWrapper(nullptr, &EVP_PKEY_CTX_free)
  {
  }

  /**
   * Construct with the input EVP_PKEY_CTX and its EVP_PKEY_CTX_free deleter function.
   *
   * @param evpPkey The EVP_PKEY_CTX OpenSSL object to wrap.
   */
  explicit OpenSSL_EVP_PKEY_CTX(EVP_PKEY_CTX* evpPkeyCtx)
    : OpenSSLObjectWrapper(evpPkeyCtx, &EVP_PKEY_CTX_free)
  {
  }
};

/**
 * Wrapper class for the OpenSSL OSSL_LIB_CTX object.
 */
class OpenSSL_OSSL_LIB_CTX : public OpenSSLObjectWrapper<OSSL_LIB_CTX, decltype(&OSSL_LIB_CTX_free)>
{
public:
  /**
   * Default constructor sets the OSSL_LIB_CTX_free deleter function.
   */
  OpenSSL_OSSL_LIB_CTX()
    : OpenSSLObjectWrapper(nullptr, &OSSL_LIB_CTX_free)
  {
  }

  /**
   * Construct with the input OSSL_LIB_CTX and its OSSL_LIB_CTX_free deleter function.
   *
   * @param evpPkey The OSSL_LIB_CTX OpenSSL object to wrap.
   */
  explicit OpenSSL_OSSL_LIB_CTX(OSSL_LIB_CTX* osslLibCtx)
    : OpenSSLObjectWrapper(osslLibCtx, &OSSL_LIB_CTX_free)
  {
  }
};

/**
 * Wrapper class for the OpenSSL OSSL_DECODER_CTX object.
 */
class OpenSSL_OSSL_DECODER_CTX : public OpenSSLObjectWrapper<OSSL_DECODER_CTX, decltype(&OSSL_DECODER_CTX_free)>
{
public:
  /**
   * Default constructor sets the OSSL_DECODER_CTX_free deleter function.
   */
  OpenSSL_OSSL_DECODER_CTX()
    : OpenSSLObjectWrapper(nullptr, &OSSL_DECODER_CTX_free)
  {
  }

  /**
   * Construct with the input OSSL_DECODER_CTX and its OSSL_DECODER_CTX_free deleter function.
   *
   * @param evpPkey The OSSL_DECODER_CTX OpenSSL object to wrap.
   */
  explicit OpenSSL_OSSL_DECODER_CTX(OSSL_DECODER_CTX* osslDecoderCtx)
    : OpenSSLObjectWrapper(osslDecoderCtx, &OSSL_DECODER_CTX_free)
  {
  }
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_OPENSSL_OBJECT_WRAPPER_H_
