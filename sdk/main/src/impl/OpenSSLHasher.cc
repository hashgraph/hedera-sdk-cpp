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
  EVP_MD_CTX* messageDigestContext = EVP_MD_CTX_new();

  if (messageDigestContext == nullptr)
  {
    throw std::runtime_error("Digest context construction failed");
  }

  EVP_MD* messageDigest = EVP_MD_fetch(nullptr, "SHA512", nullptr);

  if (messageDigest == nullptr)
  {
    EVP_MD_CTX_free(messageDigestContext);
    throw std::runtime_error("Digest construction failed");
  }

  if (EVP_DigestInit(messageDigestContext, messageDigest) <= 0)
  {
    EVP_MD_CTX_free(messageDigestContext);
    EVP_MD_free(messageDigest);
    throw std::runtime_error("Digest init failed");
  }

  std::vector<unsigned char> digest(64);

  unsigned int digestSize;
  unsigned char* hmac = HMAC(messageDigest,
                             &key.front(),
                             static_cast<int>(key.size()),
                             &data.front(),
                             static_cast<int>(data.size()),
                             &digest.front(),
                             &digestSize);

  if (hmac == nullptr)
  {
    EVP_MD_CTX_free(messageDigestContext);
    EVP_MD_free(messageDigest);
    throw std::runtime_error("HMAC failed");
  }

  EVP_MD_CTX_free(messageDigestContext);
  EVP_MD_free(messageDigest);

  return { hmac, hmac + 64 };
}

} // namespace Hedera::internal::OpenSSLHasher
