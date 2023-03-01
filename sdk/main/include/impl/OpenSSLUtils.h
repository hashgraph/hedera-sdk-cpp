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
#ifndef HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_H_
#define HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_H_

#include <functional>
#include <iostream>
#include <memory>
#include <openssl/bn.h>
#include <openssl/crypto.h>
#include <openssl/decoder.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/evp.h>
#include <string>
#include <string_view>
#include <vector>

namespace Hedera::internal::OpenSSLUtils
{
/**********************************************************************************************************************/
/*************************************************** CLASSES **********************************************************/
/**********************************************************************************************************************/

/**
 * Templated base wrapper class to be used for OpenSSL objects that require custom deleter and copier functions.
 *
 * @tparam ObjectType  The type of OpenSSL object this class should wrap.
 * @tparam DeleterType The deleter type for the OpenSSL object.
 * @tparam CopierType The copier type for the OpenSSL object.
 */
template<class ObjectType, class DeleterType, class CopierType = std::function<ObjectType*(const ObjectType*)>>
class OpenSSLObjectWrapper
{
public:
  virtual ~OpenSSLObjectWrapper() = default;

  /**
   * Copy constructor and assignment operator use this OpenSSLObjectWrapper's custom copier to copy the wrapped OpenSSL
   * object.
   */
  OpenSSLObjectWrapper(const OpenSSLObjectWrapper& other)
    : mObject({ other.mCopier(other.mObject.get()), other.mObject.get_deleter() })
    , mCopier(other.mCopier)
  {
  }

  OpenSSLObjectWrapper& operator=(const OpenSSLObjectWrapper& other)
  {
    if (this != &other)
    {
      mObject = { other.mCopier(other.mObject.get()), other.mObject.get_deleter() };
      mCopier = other.mCopier;
    }

    return *this;
  }

  OpenSSLObjectWrapper(OpenSSLObjectWrapper&&) noexcept = default;
  OpenSSLObjectWrapper& operator=(OpenSSLObjectWrapper&&) noexcept = default;

  /**
   * Get the wrapped OpenSSL object.
   *
   * @return A pointer to the wrapped OpenSSL object, nullptr if no object exists.
   */
  [[nodiscard]] ObjectType* get() const { return mObject.get(); }

  /**
   * Release ownership of the wrapped OpenSSL object. This will cause memory leaks if ownership has not already been
   * taken by another object.
   */
  void release() { mObject.release(); }

  /**
   * Determine if this OpenSSLObjectWrapper has a valid OpenSSL object.
   *
   * @return \c TRUE if there exists an OpenSSL object, otherwise \c FALSE.
   */
  [[nodiscard]] explicit operator bool() const { return mObject != nullptr; }

protected:
  OpenSSLObjectWrapper() = default;

  /**
   * Construct with values for the object, its custom deleter, and optionally a custom copier.
   *
   * @param object  The OpenSSL object to wrap.
   * @param deleter The custom deleter for the OpenSSL object.
   */
  OpenSSLObjectWrapper(ObjectType* object, const DeleterType& deleter, const CopierType& copier = CopierType())
    : mObject({ object, deleter })
    , mCopier(copier)
  {
  }

private:
  /**
   * Pointer to the OpenSSL object with its associated deleter.
   */
  std::unique_ptr<ObjectType, DeleterType> mObject = { nullptr, DeleterType() };

  /**
   * The copier to use to copy the wrapped OpenSSL object.
   */
  CopierType mCopier;
};

/**
 * Wrapper class for the OpenSSL BIGNUM object.
 */
class BIGNUM : public OpenSSLObjectWrapper<::BIGNUM, decltype(&BN_free), decltype(&BN_dup)>
{
public:
  /**
   * Construct with the input BIGNUM, its BN_clear_free deleter, and its BN_dup copier.
   *
   * @param bignum The BIGNUM OpenSSL object to wrap.
   */
  explicit BIGNUM(::BIGNUM* bignum);

  /**
   * Create a new BIGNUM from a hex string.
   *
   * @param hexString A string representing the BIGNUM.
   * @return A newly constructed BIGNUM.
   */
  static BIGNUM fromHex(std::string_view hexString);

  /**
   * Create a new BIGNUM from a bytes vector.
   *
   * @param bytes The vector of bytes representing the BIGNUM.
   * @return A newly constructed BIGNUM.
   */
  static BIGNUM fromBytes(const std::vector<unsigned char>& bytes);

  /**
   * Add another BIGNUM to this one, and take the modulo of the sum.
   *
   * @param other  The other BIGNUM to add to this one.
   * @param modulo The modulo to take the sum to.
   * @return The modular sum.
   */
  [[nodiscard]] BIGNUM modularAdd(const BIGNUM& other, const BIGNUM& modulo) const;

  /**
   * Get a vector of bytes representing this BIGNUM.
   *
   * @return The byte vector representing this BIGNUM.
   */
  [[nodiscard]] std::vector<unsigned char> toBytes() const;
};

/**
 * Wrapper class for the OpenSSL BN_CTX object.
 */
class BN_CTX : public OpenSSLObjectWrapper<::BN_CTX, decltype(&BN_CTX_free)>
{
public:
  /**
   * No custom copier provided by OpenSSL for BN_CTX.
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
  explicit BN_CTX(::BN_CTX* bnCtx);
};

/**
 * Wrapper class for the OpenSSL EC_GROUP object.
 */
class EC_GROUP : public OpenSSLObjectWrapper<::EC_GROUP, decltype(&EC_GROUP_free), decltype(&EC_GROUP_dup)>
{
public:
  /**
   * Construct with the input EC_GROUP and its EC_GROUP_free deleter function.
   *
   * @param ecGroup The EC_GROUP OpenSSL object to wrap.
   */
  explicit EC_GROUP(::EC_GROUP* ecGroup);
};

/**
 * Wrapper class for the OpenSSL EC_POINT object.
 */
class EC_POINT : public OpenSSLObjectWrapper<::EC_POINT, decltype(&EC_POINT_free)>
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
  explicit EC_POINT(::EC_POINT* ecPoint);
};

/**
 * Wrapper class for the OpenSSL ECDSA_SIG object.
 */
class ECDSA_SIG : public OpenSSLObjectWrapper<::ECDSA_SIG, decltype(&ECDSA_SIG_free)>
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
  explicit ECDSA_SIG(::ECDSA_SIG* ecdsaSig);
};

/**
 * Wrapper class for the OpenSSL EVP_MD object.
 */
class EVP_MD : public OpenSSLObjectWrapper<::EVP_MD, decltype(&EVP_MD_free)>
{
public:
  /**
   * No custom copier provided by OpenSSL for EVP_MD.
   */
  ~EVP_MD() override = default;
  EVP_MD(const EVP_MD&) = delete;
  EVP_MD& operator=(const EVP_MD&) = delete;
  EVP_MD(EVP_MD&&) = default;
  EVP_MD& operator=(EVP_MD&&) = default;

  /**
   * Construct with the input EVP_MD and its EVP_MD_free deleter function.
   *
   * @param evpMd The EVP_MD OpenSSL object to wrap.
   */
  explicit EVP_MD(::EVP_MD* evpMd);
};

/**
 * Wrapper class for the OpenSSL EVP_MD_CTX object.
 */
class EVP_MD_CTX : public OpenSSLObjectWrapper<::EVP_MD_CTX, decltype(&EVP_MD_CTX_free)>
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
  explicit EVP_MD_CTX(::EVP_MD_CTX* evpMdCtx);
};

/**
 * Wrapper class for the OpenSSL EVP_PKEY object.
 */
class EVP_PKEY : public OpenSSLObjectWrapper<::EVP_PKEY, decltype(&EVP_PKEY_free), decltype(&EVP_PKEY_dup)>
{
public:
  EVP_PKEY() = default;

  /**
   * Construct with the input EVP_PKEY, its EVP_PKEY_free deleter function, and EVP_PKEY_dup copier function.
   *
   * @param evpPkey The EVP_PKEY OpenSSL object to wrap.
   */
  explicit EVP_PKEY(::EVP_PKEY* evpPkey);
};

/**
 * Wrapper class for the OpenSSL EVP_PKEY_CTX object.
 */
class EVP_PKEY_CTX
  : public OpenSSLObjectWrapper<::EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free), decltype(&EVP_PKEY_CTX_dup)>
{
public:
  /**
   * Construct with the input EVP_PKEY_CTX and its EVP_PKEY_CTX_free deleter function.
   *
   * @param evpPkeyCtx The EVP_PKEY_CTX OpenSSL object to wrap.
   */
  explicit EVP_PKEY_CTX(::EVP_PKEY_CTX* evpPkeyCtx);
};

/**
 * Wrapper class for the OpenSSL OSSL_LIB_CTX object.
 */
class OSSL_LIB_CTX : public OpenSSLObjectWrapper<::OSSL_LIB_CTX, decltype(&OSSL_LIB_CTX_free)>
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
  explicit OSSL_LIB_CTX(::OSSL_LIB_CTX* osslLibCtx);
};

/**
 * Wrapper class for the OpenSSL OSSL_DECODER_CTX object.
 */
class OSSL_DECODER_CTX : public OpenSSLObjectWrapper<::OSSL_DECODER_CTX, decltype(&OSSL_DECODER_CTX_free)>
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
  explicit OSSL_DECODER_CTX(::OSSL_DECODER_CTX* osslDecoderCtx);
};

/**********************************************************************************************************************/
/***************************************************** FUNCTIONS ******************************************************/
/**********************************************************************************************************************/

/**
 * Computes the SHA256 hash of a byte array.
 *
 * @param data The byte array from which to compute the hash.
 * @return The SHA256 hash of the data.
 */
[[nodiscard]] std::vector<unsigned char> computeSHA256(const std::vector<unsigned char>& data);

/**
 * Compute the SHA384 hash of a byte array.
 *
 * @param data The byte array from which to compute the hash.
 * @return The SHA384 hash of the data.
 */
[[nodiscard]] std::vector<unsigned char> computeSHA384(const std::vector<unsigned char>& data);

/**
 * Compute the HMAC-SHA512 hash of a key and data.
 *
 * @param key  The key input to the hash function.
 * @param data The data input to the hash function.
 * @return The hash of the data and key.
 * @throws OpenSSLException If OpenSSL is unable to compute the HMAC-SHA512 hash of the given inputs.
 */
[[nodiscard]] std::vector<unsigned char> computeSHA512HMAC(const std::vector<unsigned char>& key,
                                                           const std::vector<unsigned char>& data);

/**
 * Gets an error message for an OpenSSL error. Includes as much detail as possible.
 *
 * @param functionName The name of the OpenSSL function which caused the error.
 * @return An error string.
 */
[[nodiscard]] std::string getErrorMessage(std::string_view functionName);

/**
 * Get a vector of random bytes.
 *
 * @param count The number of random bytes to generate. Must be positive.
 * @return The vector of random bytes.
 * @throws std::invalid_argument If the input count is negative.
 * @throws OpenSSLException If OpenSSL is unable to generate random bytes.
 */
std::vector<unsigned char> getRandomBytes(int count);

} // namespace Hedera::internal::Utils

#endif // HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_H_
