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
#include "EthereumTransactionDataEip1559.h"
#include "impl/HexConverter.h"
#include "impl/Utilities.h"

#include <rlpvalue.h>
#include <stdexcept>

namespace Hedera
{
//-----
EthereumTransactionDataEip1559::EthereumTransactionDataEip1559(std::vector<std::byte> chainId,
                                                               std::vector<std::byte> nonce,
                                                               std::vector<std::byte> maxPriorityGas,
                                                               std::vector<std::byte> maxGas,
                                                               std::vector<std::byte> gasLimit,
                                                               std::vector<std::byte> to,
                                                               std::vector<std::byte> value,
                                                               std::vector<std::byte> callData,
                                                               std::vector<std::byte> accessList,
                                                               std::vector<std::byte> recoveryId,
                                                               std::vector<std::byte> r,
                                                               std::vector<std::byte> s)
  : EthereumTransactionData(std::move(callData))
  , mChainId(std::move(chainId))
  , mNonce(std::move(nonce))
  , mMaxPriorityGas(std::move(maxPriorityGas))
  , mMaxGas(std::move(maxGas))
  , mGasLimit(std::move(gasLimit))
  , mTo(std::move(to))
  , mValue(std::move(value))
  , mAccessList(std::move(accessList))
  , mRecoveryId(std::move(recoveryId))
  , mR(std::move(r))
  , mS(std::move(s))
{
}

//-----
EthereumTransactionDataEip1559 EthereumTransactionDataEip1559::fromBytes(const std::vector<std::byte>& bytes)
{
  if (bytes.empty() || bytes.at(0) != std::byte(0x02))
  {
    throw std::invalid_argument(
      "Input byte array is malformed, It should be 0x02 followed by 12 RLP-encoded elements as a list");
  }

  RLPValue list;
  size_t consumed;
  size_t wanted;
  list.read(internal::Utilities::toTypePtr<const unsigned char>(internal::Utilities::removePrefix(bytes, 1).data()),
            bytes.size() - 1,
            consumed,
            wanted);

  if (!list.isArray() || list.size() != 12)
  {
    throw std::invalid_argument(
      "Input byte array is malformed. It should be 0x02 followed by 12 RLP-encoded elements as a list");
  }

  return EthereumTransactionDataEip1559(internal::Utilities::stringToByteVector(list.getValues().at(0).getValStr()),
                                        internal::Utilities::stringToByteVector(list.getValues().at(1).getValStr()),
                                        internal::Utilities::stringToByteVector(list.getValues().at(2).getValStr()),
                                        internal::Utilities::stringToByteVector(list.getValues().at(3).getValStr()),
                                        internal::Utilities::stringToByteVector(list.getValues().at(4).getValStr()),
                                        internal::Utilities::stringToByteVector(list.getValues().at(5).getValStr()),
                                        internal::Utilities::stringToByteVector(list.getValues().at(6).getValStr()),
                                        internal::Utilities::stringToByteVector(list.getValues().at(7).getValStr()),
                                        internal::Utilities::stringToByteVector(list.getValues().at(8).getValStr()),
                                        internal::Utilities::stringToByteVector(list.getValues().at(9).getValStr()),
                                        internal::Utilities::stringToByteVector(list.getValues().at(10).getValStr()),
                                        internal::Utilities::stringToByteVector(list.getValues().at(11).getValStr()));
}

//-----
std::vector<std::byte> EthereumTransactionDataEip1559::toBytes() const
{
  RLPValue bytes(RLPValue::VType::VARR);
  bytes.push_backV({ RLPValue(internal::Utilities::byteVectorToString(mChainId)),
                     RLPValue(internal::Utilities::byteVectorToString(mNonce)),
                     RLPValue(internal::Utilities::byteVectorToString(mMaxPriorityGas)),
                     RLPValue(internal::Utilities::byteVectorToString(mMaxGas)),
                     RLPValue(internal::Utilities::byteVectorToString(mGasLimit)),
                     RLPValue(internal::Utilities::byteVectorToString(mTo)),
                     RLPValue(internal::Utilities::byteVectorToString(mValue)),
                     RLPValue(internal::Utilities::byteVectorToString(mCallData)),
                     RLPValue(),
                     RLPValue(internal::Utilities::byteVectorToString(mRecoveryId)),
                     RLPValue(internal::Utilities::byteVectorToString(mR)),
                     RLPValue(internal::Utilities::byteVectorToString(mS)) });

  return internal::Utilities::concatenateVectors(
    { { std::byte(0x02) }, internal::Utilities::stringToByteVector(bytes.write()) });
}

//-----
std::string EthereumTransactionDataEip1559::toString() const
{
  return "mChainId: " + internal::HexConverter::bytesToHex(mChainId) +
         "\nmNonce: " + internal::HexConverter::bytesToHex(mNonce) +
         "\nmMaxPriorityGas: " + internal::HexConverter::bytesToHex(mMaxPriorityGas) +
         "\nmMaxGas: " + internal::HexConverter::bytesToHex(mMaxGas) +
         "\nmGasLimit: " + internal::HexConverter::bytesToHex(mGasLimit) +
         "\nmTo: " + internal::HexConverter::bytesToHex(mTo) +
         "\nmValue: " + internal::HexConverter::bytesToHex(mValue) +
         "\nmCallData: " + internal::HexConverter::bytesToHex(mCallData) +
         "\nmAccessList: " + internal::HexConverter::bytesToHex(mAccessList) +
         "\nmRecoveryId: " + internal::HexConverter::bytesToHex(mRecoveryId) +
         "\nmR: " + internal::HexConverter::bytesToHex(mR) + "\nmS: " + internal::HexConverter::bytesToHex(mS);
}

} // namespace Hedera
