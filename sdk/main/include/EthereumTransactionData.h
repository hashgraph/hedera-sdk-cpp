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
#ifndef HEDERA_SDK_CPP_ETHEREUM_TRANSACTION_DATA_H_
#define HEDERA_SDK_CPP_ETHEREUM_TRANSACTION_DATA_H_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace Hedera
{
/**
 * Wrapper class for Ethereum data.
 */
class EthereumTransactionData
{
public:
  EthereumTransactionData() = default;
  virtual ~EthereumTransactionData() = default;

  /**
   * Construct with call data.
   *
   * @param callData The call data.
   */
  explicit EthereumTransactionData(std::vector<std::byte> callData);

  /**
   * Construct an EthereumTransactionData object from raw bytes.
   *
   * @param bytes The raw bytes of the ethereum data.
   * @return A pointer to the constructed EthereumTransactionData.
   */
  [[nodiscard]] static std::unique_ptr<EthereumTransactionData> fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Serialize this EthereumTransactionData object into RLP-encoded bytes.
   *
   * @return The RLP-encoded serialized EthereumTransactionData.
   */
  [[nodiscard]] virtual std::vector<std::byte> toBytes() const = 0;

  /**
   * Serialize this EthereumTransactionData object into a hex-encoded, RLP-encoded string.
   *
   * @return The hex-encoded, RLP-encoded EthereumTransactionData.
   */
  [[nodiscard]] virtual std::string toString() const = 0;

  /**
   * The call data.
   */
  std::vector<std::byte> mCallData;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ETHEREUM_TRANSACTION_DATA_H_
