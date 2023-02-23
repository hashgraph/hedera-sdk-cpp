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

#include <memory>
#include <openssl/bn.h>
#include <openssl/crypto.h>
#include <openssl/decoder.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/evp.h>
#include <string>
#include <vector>

namespace Hedera::internal::OpenSSLUtils
{
/**********************************************************************************************************************/
/*************************************************** CLASSES **********************************************************/
/**********************************************************************************************************************/

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
 * Wrapper class for the OpenSSL BIGNUM object.
 */
class BIGNUM : public OpenSSLObjectWrapper<::BIGNUM, decltype(&BN_clear_free)>
{
public:
  /**
   * Construct with the input BIGNUM and its BN_clear_free deleter function.
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
  static BIGNUM fromHex(const std::string& hexString);

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
   * Construct with the input BN_CTX and its BN_CTX_free deleter function.
   *
   * @param bnCtx The BN_CTX OpenSSL object to wrap.
   */
  explicit BN_CTX(::BN_CTX* bnCtx);
};

/**
 * Wrapper class for the OpenSSL EC_GROUP object.
 */
class EC_GROUP : public OpenSSLObjectWrapper<::EC_GROUP, decltype(&EC_GROUP_free)>
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
   * Construct with the input EVP_MD_CTX and its EVP_MD_CTX_free deleter function.
   *
   * @param evpMdCtx The EVP_MD_CTX OpenSSL object to wrap.
   */
  explicit EVP_MD_CTX(::EVP_MD_CTX* evpMdCtx);
};

/**
 * Wrapper class for the OpenSSL EVP_PKEY object.
 */
class EVP_PKEY : public OpenSSLObjectWrapper<::EVP_PKEY, decltype(&EVP_PKEY_free)>
{
public:
  /**
   * Construct with the input EVP_PKEY and its EVP_PKEY_free deleter function.
   *
   * @param evpPkey The EVP_PKEY OpenSSL object to wrap.
   */
  explicit EVP_PKEY(::EVP_PKEY* evpPkey);
};

/**
 * Wrapper class for the OpenSSL EVP_PKEY_CTX object.
 */
class EVP_PKEY_CTX : public OpenSSLObjectWrapper<::EVP_PKEY_CTX, decltype(&EVP_PKEY_CTX_free)>
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
 * Compute the SHA384 hash of a string.
 *
 * @param data The string of which to compute the hash.
 * @return The hash of the data.
 */
[[nodiscard]] std::vector<unsigned char> computeSHA384(const std::string& data);

/**
 * Computes the SHA256 hash of a byte array.
 *
 * @param data The data of which to compute the hash.
 * @return The hash of the data.
 */
[[nodiscard]] std::vector<unsigned char> computeSHA256(const std::vector<unsigned char>& data);

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
[[nodiscard]] std::string getErrorMessage(const std::string& functionName);

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
