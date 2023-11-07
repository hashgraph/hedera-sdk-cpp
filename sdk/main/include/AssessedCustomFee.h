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
#ifndef HEDERA_SDK_CPP_ASSESSED_CUSTOM_FEE_H_
#define HEDERA_SDK_CPP_ASSESSED_CUSTOM_FEE_H_

#include "AccountId.h"
#include "TokenId.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

namespace proto
{
class AssessedCustomFee;
}

namespace Hedera
{
/**
 * A custom transfer fee that was assessed during the handling of a CryptoTransfer.
 */
class AssessedCustomFee
{
public:
  /**
   * Create an AssessedCustomFee object from a AssessedCustomFee protobuf object.
   *
   * @param proto The AssessedCustomFee protobuf object from which to create an AssessedCustomFee object.
   * @return The constructed AssessedCustomFee object.
   */
  [[nodiscard]] static AssessedCustomFee fromProtobuf(const proto::AssessedCustomFee& proto);

  /**
   * Create an AssessedCustomFee object from an array of bytes.
   *
   * @param bytes The bytes from which to create an AssessedCustomFee object.
   * @return The constructed AssessedCustomFee object.
   */
  [[nodiscard]] static AssessedCustomFee fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a AssessedCustomFee protobuf object from this AssessedCustomFee object.
   *
   * @return A pointer to the created AssessedCustomFee protobuf object filled with this AssessedCustomFee object's
   *         data.
   */
  [[nodiscard]] std::unique_ptr<proto::AssessedCustomFee> toProtobuf() const;

  /**
   * Create a byte array representation of this AssessedCustomFee object.
   *
   * @return A byte array representation of this AssessedCustomFee object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * The number of units assessed for the fee.
   */
  int64_t mAmount;

  /**
   * The denomination of the fee. The denomination is Hbar if left unset.
   */
  std::optional<TokenId> mTokenId;

  /**
   * The ID of the account to receive the assessed fee.
   */
  AccountId mFeeCollectorAccountId;

  /**
   * The IDs of the accounts who paid this fee (i.e. their balances would be higher if not for this fee).
   */
  std::vector<AccountId> mPayerAccountIdList;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ASSESSED_CUSTOM_FEE_H_
