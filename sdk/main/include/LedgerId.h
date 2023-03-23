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
#ifndef HEDERA_SDK_CPP_LEDGER_ID_H_
#define HEDERA_SDK_CPP_LEDGER_ID_H_

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace Hedera
{
class LedgerId
{
public:
  /**
   * The Hedera Mainnet ledger ID.
   */
  static const LedgerId MAINNET;

  /**
   * The Hedera Testnet ledger ID.
   */
  static const LedgerId TESTNET;

  /**
   * The Hedera Previewnet ledger ID.
   */
  static const LedgerId PREVIEWNET;

  /**
   * Construct a LedgerId from a byte array.
   *
   * @param bytes The bytes from which to construct this LedgerId.
   */
  explicit LedgerId(std::vector<std::byte> bytes);

  /**
   * Compare this LedgerId to another LedgerId and determine if they represent the same ledger.
   *
   * @param other The other LedgerId with which to compare this LedgerId.
   * @return \c TRUE if this LedgerId is the same as the input LedgerId, otherwise \c FALSE.
   */
  bool operator==(const LedgerId& other) const;

  /**
   * Construct a LedgerId object from a string. The string can be either hex-encoded bytes, or "mainnet", "testnet", or
   * "previewnet".
   *
   * @param id The ledger ID string from which to construct.
   * @return The constructed LedgerId object.
   * @throws std::invalid_argument If a LedgerId cannot be realized from the input hex-encoded string.
   */
  [[nodiscard]] static LedgerId fromString(std::string_view id);

  /**
   * Get the string representation of this LedgerId. This will either return "mainnet", "testnet", "previewnet", or the
   * hex-encoded bytes.
   *
   * @return The string representation of this LedgerId.
   * @throws OpenSSLException If OpenSSL is unable to hex-encode the bytes.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Does this LedgerId represent the LedgerId of the Hedera Mainnet?
   *
   * @return \c TRUE if this LedgerId represent the LedgerId of the Hedera Mainnet, otherwise \c FALSE.
   */
  [[nodiscard]] inline bool isMainnet() const { return *this == MAINNET; }

  /**
   * Does this LedgerId represent the LedgerId of the Hedera Testnet?
   *
   * @return \c TRUE if this LedgerId represent the LedgerId of the Hedera Testnet, otherwise \c FALSE.
   */
  [[nodiscard]] inline bool isTestnet() const { return *this == TESTNET; }

  /**
   * Does this LedgerId represent the LedgerId of the Hedera Previewnet?
   *
   * @return \c TRUE if this LedgerId represent the LedgerId of the Hedera Previewnet, otherwise \c FALSE.
   */
  [[nodiscard]] inline bool isPreviewnet() const { return *this == PREVIEWNET; }

  /**
   * Get the bytes of this LedgerId.
   *
   * @return The bytes of this LedgerId.
   */
  [[nodiscard]] inline std::vector<std::byte> toBytes() const { return mLedgerId; }

private:
  /**
   * The bytes of this LedgerId.
   */
  std::vector<std::byte> mLedgerId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_LEDGER_ID_H_
