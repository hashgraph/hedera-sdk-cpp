/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_ACCOUNT_BALANCE_H_
#define HEDERA_SDK_CPP_ACCOUNT_BALANCE_H_

#include "Hbar.h"
#include "TokenId.h"

#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace proto
{
class CryptoGetAccountBalanceResponse;
}

namespace Hedera
{
/**
 * Response from a Hedera network when the client sends an AccountBalanceQuery.
 */
class AccountBalance
{
public:
  /**
   * Construct an AccountBalance object from a CryptoGetAccountBalanceResponse protobuf object.
   *
   * @param proto The CryptoGetAccountBalance protobuf object from which to construct an AccountBalance object.
   * @return The constructed AccountBalance object.
   */
  [[nodiscard]] static AccountBalance fromProtobuf(const proto::CryptoGetAccountBalanceResponse& proto);

  /**
   * Construct an AccountBalance object from a byte array.
   *
   * @param bytes The byte array from which to construct an AccountBalance object.
   * @return The constructed AccountBalance object.
   */
  [[nodiscard]] static AccountBalance fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a CryptoGetAccountBalanceResponse protobuf object from this FeeSchedules object.
   *
   * @return A pointer to the created CryptoGetAccountBalanceResponse protobuf object.
   */
  [[nodiscard]] std::unique_ptr<proto::CryptoGetAccountBalanceResponse> toProtobuf() const;

  /**
   * Construct a representative byte array from this AccountBalance object.
   *
   * @return The byte array representing this AccountBalance object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Construct a string representation of this AccountBalance object.
   *
   * @return The string representation of this AccountBalance object.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Write this AccountBalance to an output stream.
   *
   * @param os      The output stream.
   * @param balance The AccountBalance to print.
   * @return The output stream with this AccountBalance written to it.
   */
  friend std::ostream& operator<<(std::ostream& os, const AccountBalance& balance);

  /**
   * The account or contract balance.
   */
  Hbar mBalance;

  /**
   * Map of tokens with associated values.
   */
  std::unordered_map<TokenId, uint64_t> mTokens;

  /**
   * Map of token decimals with associated values.
   */
  std::unordered_map<TokenId, int> mTokenDecimals;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ACCOUNT_BALANCE_H_
