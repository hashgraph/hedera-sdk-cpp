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

#include "exceptions/BadKeyException.h"

#include "impl/ASN1ECPrivateKey.h"
#include "impl/HexConverter.h"

namespace Hedera::internal::asn1
{
ASN1ECPrivateKey::ASN1ECPrivateKey(const std::vector<std::byte>& bytes)
{
  if (bytes.size() >= MAX_ENCRYPTED_KEY_LENGHT)
  {
    throw BadKeyException("Over maximum possible input bytes for EC Key!");
  }
  else
  {
    decode(bytes);
  }
}

std::vector<std::byte> ASN1ECPrivateKey::getKey() const
{
  std::vector<std::byte> privateKey = get(OCTET_STRING);
  if (privateKey.size() < EC_KEY_LENGTH)
  {
    throw BadKeyException("Data not decoded properly for input PEM/DER EC KEY bytes!");
  }
  if (privateKey.size() > EC_KEY_LENGTH) // remove redundant padded bytes if any
  {
    privateKey = std::vector<std::byte>(privateKey.end() - EC_KEY_LENGTH, privateKey.end());
  }

  return privateKey;
}

} // namespace Hedera::internal:asn1
