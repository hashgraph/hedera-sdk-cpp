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
#include "impl/OpenSSLHasher.h"
#include "impl/OpenSSLObjectWrapper.h"

#include <openssl/err.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>

namespace Hedera::internal::OpenSSLHasher
{
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
  const internal::OpenSSL_EVP_MD_CTX messageDigestContext(EVP_MD_CTX_new());
  if (!messageDigestContext)
  {
    throw std::runtime_error("Digest context construction failed");
  }

  const internal::OpenSSL_EVP_MD messageDigest(EVP_MD_fetch(nullptr, "SHA512", nullptr));
  if (!messageDigest)
  {
    throw std::runtime_error("Digest construction failed");
  }

  if (EVP_DigestInit(messageDigestContext.get(), messageDigest.get()) <= 0)
  {
    throw std::runtime_error("Digest init failed");
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
    throw std::runtime_error("HMAC failed");
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

} // namespace Hedera::internal::OpenSSLHasher
