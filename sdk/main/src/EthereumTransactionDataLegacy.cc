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
#include "EthereumTransactionDataLegacy.h"
#include "impl/HexConverter.h"
#include "impl/Utilities.h"

#include <rlpvalue.h>
#include <stdexcept>

namespace Hedera
{
//-----
EthereumTransactionDataLegacy::EthereumTransactionDataLegacy(std::vector<std::byte> nonce,
                                                             std::vector<std::byte> gasPrice,
                                                             std::vector<std::byte> gasLimit,
                                                             std::vector<std::byte> to,
                                                             std::vector<std::byte> value,
                                                             std::vector<std::byte> callData,
                                                             std::vector<std::byte> v,
                                                             std::vector<std::byte> r,
                                                             std::vector<std::byte> s)
  : EthereumTransactionData(callData)
  , mNonce(std::move(nonce))
  , mGasPrice(std::move(gasPrice))
  , mGasLimit(std::move(gasLimit))
  , mTo(std::move(to))
  , mValue(std::move(value))
  , mV(std::move(v))
  , mR(std::move(r))
  , mS(std::move(s))
{
}

//-----
EthereumTransactionDataLegacy EthereumTransactionDataLegacy::fromBytes(const std::vector<std::byte>& bytes)
{
  RLPValue list;
  size_t consumed;
  size_t wanted;
  list.read(internal::Utilities::toTypePtr<const unsigned char>(bytes.data()), bytes.size(), consumed, wanted);

  if (!list.isArray())
  {
    throw std::invalid_argument("Input byte array does not represent a list of RLP-encoded elements");
  }

  else if (list.getValues().size() != 9)
  {
    throw std::invalid_argument("Input byte array does not contain 9 RLP-encoded elements");
  }

  return EthereumTransactionDataLegacy(internal::Utilities::stringToByteVector(list.getValues().at(0).getValStr()),
                                       internal::Utilities::stringToByteVector(list.getValues().at(1).getValStr()),
                                       internal::Utilities::stringToByteVector(list.getValues().at(2).getValStr()),
                                       internal::Utilities::stringToByteVector(list.getValues().at(3).getValStr()),
                                       internal::Utilities::stringToByteVector(list.getValues().at(4).getValStr()),
                                       internal::Utilities::stringToByteVector(list.getValues().at(5).getValStr()),
                                       internal::Utilities::stringToByteVector(list.getValues().at(6).getValStr()),
                                       internal::Utilities::stringToByteVector(list.getValues().at(7).getValStr()),
                                       internal::Utilities::stringToByteVector(list.getValues().at(8).getValStr()));
}

//-----
std::vector<std::byte> EthereumTransactionDataLegacy::toBytes() const
{
  RLPValue bytes(RLPValue::VType::VARR);
  bytes.push_backV({ RLPValue(internal::Utilities::byteVectorToString(mNonce)),
                     RLPValue(internal::Utilities::byteVectorToString(mGasPrice)),
                     RLPValue(internal::Utilities::byteVectorToString(mGasLimit)),
                     RLPValue(internal::Utilities::byteVectorToString(mTo)),
                     RLPValue(internal::Utilities::byteVectorToString(mValue)),
                     RLPValue(internal::Utilities::byteVectorToString(mV)),
                     RLPValue(internal::Utilities::byteVectorToString(mR)),
                     RLPValue(internal::Utilities::byteVectorToString(mS)) });
  return internal::Utilities::stringToByteVector(bytes.write());
}

//-----
std::string EthereumTransactionDataLegacy::toString() const
{
  return "mNonce: " + internal::HexConverter::bytesToHex(mNonce) +
         "\nmGasPrice: " + internal::HexConverter::bytesToHex(mGasPrice) +
         "\nmGasLimit: " + internal::HexConverter::bytesToHex(mGasLimit) +
         "\nmTo: " + internal::HexConverter::bytesToHex(mTo) +
         "\nmValue: " + internal::HexConverter::bytesToHex(mValue) + "\nmV: " + internal::HexConverter::bytesToHex(mV) +
         "\nmR: " + internal::HexConverter::bytesToHex(mR) + "\nmS: " + internal::HexConverter::bytesToHex(mS);
}

} // namespace Hedera
