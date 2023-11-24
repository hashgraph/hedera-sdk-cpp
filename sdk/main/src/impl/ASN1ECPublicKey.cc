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

#include "impl/ASN1ECPublicKey.h"
#include "impl/HexConverter.h"
#include "impl/Utilities.h"

#include <openssl/bn.h>

#include <algorithm>

namespace Hedera::internal::asn1
{
ASN1ECPublicKey::ASN1ECPublicKey(const std::vector<std::byte>& bytes)
{
  decode(bytes);
  populateXYcoords();
}

const std::vector<std::byte> ASN1ECPublicKey::getKey() const
{
  if (ecXcoord.empty())
    throw BadKeyException("Data not decoded properly for input PEM/DER EC KEY bytes!");
  else
    return ecXcoord;
}

void ASN1ECPublicKey::populateXYcoords()
{
  std::vector<std::byte> publicKey = get(BIT_STRING);
  if (publicKey[0] == std::byte(0x00) && publicKey[1] == std::byte(0x04)) // uncompressed
  {
    ecYcoord = std::vector<std::byte>(publicKey.rbegin(), publicKey.rbegin() + ECDSA_KEY_LENGTH); // get y coordinate
    std::reverse(ecYcoord.begin(), ecYcoord.end());

    BIGNUM* yCoordBN = BN_new();
    BN_hex2bn(&yCoordBN, internal::HexConverter::bytesToHex(ecYcoord).c_str());
    // if y even pad 0x02 byte else 0x03
    ecXcoord = internal::Utilities::concatenateVectors({
  // get x coordinate
      { std::byte(0x00) },
      { !BN_is_bit_set(yCoordBN, 0) ? std::byte(0x02) : std::byte(0x03) },
      { publicKey.begin() + 2, publicKey.end() - ECDSA_KEY_LENGTH }
    });
  }

  if (publicKey[0] == std::byte(0x00) &&
      (publicKey[1] == std::byte(0x02) || publicKey[1] == std::byte(0x03))) // compressed
  {
    // Just populate x coord. Padding one to ignore the 00 starting byte.
    ecXcoord = std::vector<std::byte>(publicKey.begin(), publicKey.begin() + ECDSA_KEY_LENGTH + 2);
  }
}

} // namespace Hedera::internal:asn1
