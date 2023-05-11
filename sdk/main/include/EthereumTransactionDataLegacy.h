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
#ifndef HEDERA_SDK_CPP_ETHEREUM_TRANSACTION_DATA_LEGACY_H_
#define HEDERA_SDK_CPP_ETHEREUM_TRANSACTION_DATA_LEGACY_H_

#include "EthereumTransactionData.h"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace Hedera
{
/**
 * Wrapper class for legacy Ethereum data.
 */
class EthereumTransactionDataLegacy : public EthereumTransactionData
{
public:
  EthereumTransactionDataLegacy() = default;

  /**
   * Construct with all fields of Ethereum legacy data.
   *
   * @param nonce    The transaction nonce.
   * @param gasPrice The price of one gas.
   * @param gasLimit The amount of gas available for the transaction.
   * @param to       The receiver of the transaction.
   * @param value    The value of the transaction.
   * @param callData The call data.
   * @param v        The v value of the signature.
   * @param r        The r value of the signature.
   * @param s        The s value of the signature.
   */
  explicit EthereumTransactionDataLegacy(std::vector<std::byte> nonce,
                                         std::vector<std::byte> gasPrice,
                                         std::vector<std::byte> gasLimit,
                                         std::vector<std::byte> to,
                                         std::vector<std::byte> value,
                                         std::vector<std::byte> callData,
                                         std::vector<std::byte> v,
                                         std::vector<std::byte> r,
                                         std::vector<std::byte> s);

  /**
   * Construct an EthereumTransactionDataLegacy object from raw bytes.
   *
   * @param bytes The raw bytes of the ethereum data.
   * @return The constructed EthereumTransactionDataLegacy.
   * @throws std::invalid_argument If the input byte array isn't a list of RLP-encoded values or there aren't the
   *                               correct number of values (9).
   */
  [[nodiscard]] static EthereumTransactionDataLegacy fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Derived from EthereumTransactionData. Serialize this EthereumTransactionDataLegacy object into RLP-encoded bytes.
   *
   * @return The RLP-encoded serialized EthereumTransactionDataLegacy.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const override;

  /**
   * Derived from EthereumTransactionData. Serialize this EthereumTransactionDataLegacy object into a hex-encoded,
   * RLP-encoded string.
   *
   * @return The hex-encoded, RLP-encoded EthereumTransactionDataLegacy.
   */
  [[nodiscard]] std::string toString() const override;

  /**
   * The transaction's nonce.
   */
  std::vector<std::byte> mNonce;

  /**
   * The price for one gas.
   */
  std::vector<std::byte> mGasPrice;

  /**
   * The amount of gas available for the transaction.
   */
  std::vector<std::byte> mGasLimit;

  /**
   * The receiver of the transaction.
   */
  std::vector<std::byte> mTo;

  /**
   * The transaction value.
   */
  std::vector<std::byte> mValue;

  /**
   * The V value of the signature.
   */
  std::vector<std::byte> mV;

  /**
   * The R value of the signature.
   */
  std::vector<std::byte> mR;

  /**
   * The S value of the signature.
   */
  std::vector<std::byte> mS;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ETHEREUM_TRANSACTION_DATA_LEGACY_H_
