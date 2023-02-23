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
#include "impl/OpenSSLUtils.h"
#include "exceptions/OpenSSLException.h"
#include "impl/HexConverter.h"

#include <openssl/err.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdexcept>

namespace Hedera::internal::OpenSSLUtils
{
//-----
OpenSSL_BIGNUM::OpenSSL_BIGNUM(BIGNUM* bignum)
  : OpenSSLObjectWrapper(bignum, &BN_clear_free)
{
}

//-----
OpenSSL_BIGNUM OpenSSL_BIGNUM::fromHex(const std::string& hexString)
{
  BIGNUM* bigNum = nullptr;
  if (BN_hex2bn(&bigNum, hexString.c_str()) <= 0)
  {
    throw OpenSSLException(getOpenSSLErrorMessage("BN_hex2bn"));
  }

  return OpenSSL_BIGNUM(bigNum);
}

//-----
OpenSSL_BIGNUM OpenSSL_BIGNUM::fromBytes(const std::vector<unsigned char>& bytes)
{
  // go through hex rather than using big-endian openssl functions
  return fromHex(HexConverter::base64ToHex(bytes));
}

//-----
OpenSSL_BIGNUM OpenSSL_BIGNUM::modularAdd(const OpenSSL_BIGNUM& other, const OpenSSL_BIGNUM& modulo) const
{
  const OpenSSL_BN_CTX context(BN_CTX_secure_new());
  if (!context)
  {
    throw OpenSSLException(getOpenSSLErrorMessage("BN_CTX_secure_new"));
  }

  OpenSSL_BIGNUM result(BN_secure_new());
  if (!result)
  {
    throw OpenSSLException(getOpenSSLErrorMessage("BN_secure_new"));
  }

  if (BN_mod_add(result.get(), get(), other.get(), modulo.get(), context.get()) <= 0)
  {
    throw OpenSSLException(getOpenSSLErrorMessage("BN_mod_add"));
  }

  return result;
}

//-----
std::vector<unsigned char> OpenSSL_BIGNUM::toBytes() const
{
  char* hex = BN_bn2hex(get());
  const std::string hexString(hex);
  OPENSSL_free(hex);

  return HexConverter::hexToBase64(hexString);
}

//-----
OpenSSL_BN_CTX::OpenSSL_BN_CTX(BN_CTX* bnCtx)
  : OpenSSLObjectWrapper(bnCtx, &BN_CTX_free)
{
}

//-----
OpenSSL_EC_GROUP::OpenSSL_EC_GROUP(EC_GROUP* ecGroup)
  : OpenSSLObjectWrapper(ecGroup, &EC_GROUP_free)
{
}

//-----
OpenSSL_EC_POINT::OpenSSL_EC_POINT(EC_POINT* ecPoint)
  : OpenSSLObjectWrapper(ecPoint, &EC_POINT_free)
{
}

//-----
OpenSSL_ECDSA_SIG::OpenSSL_ECDSA_SIG(ECDSA_SIG* ecdsaSig)
  : OpenSSLObjectWrapper(ecdsaSig, &ECDSA_SIG_free)
{
}

//-----
OpenSSL_EVP_MD::OpenSSL_EVP_MD(EVP_MD* evpMd)
  : OpenSSLObjectWrapper(evpMd, &EVP_MD_free)
{
}

//-----
OpenSSL_EVP_MD_CTX::OpenSSL_EVP_MD_CTX(EVP_MD_CTX* evpMdCtx)
  : OpenSSLObjectWrapper(evpMdCtx, &EVP_MD_CTX_free)
{
}

//-----
OpenSSL_EVP_PKEY::OpenSSL_EVP_PKEY(EVP_PKEY* evpPkey)
  : OpenSSLObjectWrapper(evpPkey, &EVP_PKEY_free)
{
}

//-----
OpenSSL_EVP_PKEY_CTX::OpenSSL_EVP_PKEY_CTX(EVP_PKEY_CTX* evpPkeyCtx)
  : OpenSSLObjectWrapper(evpPkeyCtx, &EVP_PKEY_CTX_free)
{
}

//-----
OpenSSL_OSSL_LIB_CTX::OpenSSL_OSSL_LIB_CTX(OSSL_LIB_CTX* osslLibCtx)
  : OpenSSLObjectWrapper(osslLibCtx, &OSSL_LIB_CTX_free)
{
}

//-----
OpenSSL_OSSL_DECODER_CTX::OpenSSL_OSSL_DECODER_CTX(OSSL_DECODER_CTX* osslDecoderCtx)
  : OpenSSLObjectWrapper(osslDecoderCtx, &OSSL_DECODER_CTX_free)
{
}

//-----
std::vector<unsigned char> computeSHA384(const std::string& data)
{
  auto outputBytes = std::vector<unsigned char>(48);
  auto rawData = reinterpret_cast<const unsigned char*>(data.c_str());
  SHA384(rawData, data.size(), &outputBytes.front());

  return outputBytes;
}

//-----
std::vector<unsigned char> computeSHA256(const std::vector<unsigned char>& data)
{
  auto outputBytes = std::vector<unsigned char>(32);

  const unsigned char* rawData = &data.front();
  SHA256(rawData, data.size(), &outputBytes.front());

  return outputBytes;
}

//-----
std::vector<unsigned char> computeSHA512HMAC(const std::vector<unsigned char>& key,
                                             const std::vector<unsigned char>& data)
{
  const OpenSSL_EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw OpenSSLException(getOpenSSLErrorMessage("EVP_MD_CTX_new"));
  }

  const OpenSSL_EVP_MD messageDigest(EVP_MD_fetch(nullptr, "SHA512", nullptr));
  if (!messageDigest)
  {
    throw OpenSSLException(getOpenSSLErrorMessage("EVP_MD_fetch"));
  }

  if (EVP_DigestInit(messageDigestContext.get(), messageDigest.get()) <= 0)
  {
    throw OpenSSLException(getOpenSSLErrorMessage("EVP_DigestInit"));
  }

  std::vector<unsigned char> digest(64);

  unsigned int digestSize;
  unsigned char* hmac = HMAC(messageDigest.get(),
                             &key.front(),
                             static_cast<int>(key.size()),
                             &data.front(),
                             static_cast<int>(data.size()),
                             &digest.front(),
                             &digestSize);

  if (!hmac)
  {
    throw OpenSSLException(getOpenSSLErrorMessage("HMAC"));
  }

  return { hmac, hmac + 64 };
}

//-----
std::string getOpenSSLErrorMessage(const std::string& functionName)
{
  unsigned long errorCode = ERR_get_error();

  // no error was found
  if (errorCode == 0)
  {
    return "Error occurred in [" + functionName + "]";
  }

  char errorStringRaw[256];

  ERR_error_string_n(errorCode, errorStringRaw, 256);

  return "Error occurred in [" + functionName + "]: " + std::string{ errorStringRaw };
}

//-----
std::vector<unsigned char> getRandomBytes(int count)
{
  if (count <= 0)
  {
    throw std::invalid_argument("The number of random bytes to generate must be positive");
  }

  std::vector<unsigned char> randomBytes(count);

  if (RAND_priv_bytes(&randomBytes.front(), count) <= 0)
  {
    throw OpenSSLException(getOpenSSLErrorMessage("RAND_priv_bytes"));
  }

  return randomBytes;
}

} // namespace Hedera::internal::OpenSSLUtils
