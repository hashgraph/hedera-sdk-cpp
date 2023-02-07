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
#include "impl/OpenSSLObjectWrapper.h"
#include "impl/HexConverter.h"
#include "impl/OpenSSLHasher.h"

#include <stdexcept>

namespace Hedera::internal
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
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_hex2bn"));
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
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_CTX_secure_new"));
  }

  OpenSSL_BIGNUM result(BN_secure_new());
  if (!result)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_secure_new"));
  }

  if (BN_mod_add(result.get(), get(), other.get(), modulo.get(), context.get()) <= 0)
  {
    throw std::runtime_error(internal::OpenSSLHasher::getOpenSSLErrorMessage("BN_mod_add"));
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

} // namespace Hedera::internal
