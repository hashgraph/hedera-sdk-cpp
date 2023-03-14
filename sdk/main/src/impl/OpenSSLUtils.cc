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
#include "impl/openssl_utils/OpenSSLUtils.h"
#include "exceptions/OpenSSLException.h"
#include "impl/openssl_utils/EVP_MD.h"
#include "impl/openssl_utils/EVP_MD_CTX.h"

#include <openssl/err.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <stdexcept>

namespace Hedera::internal::OpenSSLUtils
{
namespace
{
// The size of a SHA256 hash (in bytes).
constexpr const size_t SHA256_HASH_SIZE = 32ULL;
// The size of a SHA384 hash (in bytes).
constexpr const size_t SHA384_HASH_SIZE = 48ULL;
// The size of a SHA512 hash (in bytes).
constexpr const size_t SHA512_HMAC_HASH_SIZE = 64ULL;
// The size of an OpenSSL error message.
constexpr const size_t ERROR_MSG_SIZE = 256ULL;
}

//-----
std::vector<unsigned char> computeSHA256(const std::vector<unsigned char>& data)
{
  auto outputBytes = std::vector<unsigned char>(SHA256_HASH_SIZE);
  SHA256(data.data(), data.size(), outputBytes.data());
  return outputBytes;
}

//-----
std::vector<unsigned char> computeSHA384(const std::vector<unsigned char>& data)
{
  auto outputBytes = std::vector<unsigned char>(SHA384_HASH_SIZE);
  SHA384(data.data(), data.size(), outputBytes.data());
  return outputBytes;
}

//-----
std::vector<unsigned char> computeSHA512HMAC(const std::vector<unsigned char>& key,
                                             const std::vector<unsigned char>& data)
{
  EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw OpenSSLException(getErrorMessage("EVP_MD_CTX_new"));
  }

  const EVP_MD messageDigest(EVP_MD_fetch(nullptr, "SHA512", nullptr));
  if (!messageDigest)
  {
    throw OpenSSLException(getErrorMessage("EVP_MD_fetch"));
  }

  if (EVP_DigestInit(messageDigestContext.get(), messageDigest.get()) <= 0)
  {
    throw OpenSSLException(getErrorMessage("EVP_DigestInit"));
  }

  std::vector<unsigned char> digest(SHA512_HMAC_HASH_SIZE);
  if (!HMAC(messageDigest.get(),
            key.data(),
            static_cast<int>(key.size()),
            data.data(),
            static_cast<int>(data.size()),
            digest.data(),
            nullptr))
  {
    throw OpenSSLException(getErrorMessage("HMAC"));
  }

  return digest;
}

//-----
std::string getErrorMessage(std::string_view functionName)
{
  const unsigned long errorCode = ERR_get_error();

  // no error was found
  if (errorCode == 0)
  {
    return std::string("Error occurred in [").append(functionName).append("]");
  }

  std::string errorString(ERROR_MSG_SIZE, '\0');
  ERR_error_string_n(errorCode, errorString.data(), ERROR_MSG_SIZE);

  return std::string("Error occurred in [").append(functionName).append("]: ") + errorString;
}

//-----
std::vector<unsigned char> getRandomBytes(int count)
{
  if (count <= 0)
  {
    throw std::invalid_argument("The number of random bytes to generate must be positive");
  }

  std::vector<unsigned char> randomBytes(count);

  if (RAND_priv_bytes(randomBytes.data(), count) <= 0)
  {
    throw OpenSSLException(getErrorMessage("RAND_priv_bytes"));
  }

  return randomBytes;
}

} // namespace Hedera::internal::OpenSSLUtils
