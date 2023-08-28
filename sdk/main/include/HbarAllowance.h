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
#ifndef HEDERA_SDK_CPP_HBAR_ALLOWANCE_H_
#define HEDERA_SDK_CPP_HBAR_ALLOWANCE_H_

#include "AccountId.h"
#include "Hbar.h"

#include <memory>

namespace proto
{
class CryptoAllowance;
}

namespace Hedera
{
/**
 * An approved allowance of Hbar transfers for a spender.
 */
class HbarAllowance
{
public:
  HbarAllowance() = default;

  /**
   * Construct with an owner, spender, and an amount.
   *
   * @param owner   The ID of the account approving an allowance of its Hbars.
   * @param spender The ID of the account being allowed to spend the Hbars.
   * @param amount  The amount of Hbars that are being allowed to be spent.
   */
  HbarAllowance(AccountId owner, AccountId spender, const Hbar& amount);

  /**
   * Construct an HbarAllowance object from a CryptoAllowance protobuf object.
   *
   * @param proto The CryptoAllowance protobuf object from which to construct an HbarAllowance object.
   * @return The constructed HbarAllowance object.
   */
  [[nodiscard]] static HbarAllowance fromProtobuf(const proto::CryptoAllowance& proto);

  /**
   * Construct a CryptoAllowance protobuf object from this HbarAllowance object.
   *
   * @return A pointer to a constructed CryptoAllowance protobuf object filled with this HbarAllowance object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::CryptoAllowance> toProtobuf() const;

  /**
   * Set the ID of the account approving an allowance of its Hbars.
   *
   * @param accountId The ID of the account approving an allowance of its Hbars.
   * @return A reference to this HbarAllowance object with the newly-set owner account ID.
   */
  HbarAllowance& setOwnerAccountId(const AccountId& accountId);

  /**
   * Set the ID of the account being allowed to spend the Hbars.
   *
   * @param accountId The ID of the account being allowed to spend the Hbars.
   * @return A reference to this HbarAllowance object with the newly-set spender account ID.
   */
  HbarAllowance& setSpenderAccountId(const AccountId& accountId);

  /**
   * Set the amount of Hbars that are being allowed to be spent.
   *
   * @param amount The amount of Hbars that are being allowed to be spent.
   * @return A reference to this HbarAllowance object with the newly-set amount.
   */
  HbarAllowance& setAmount(const Hbar& amount);

  /**
   * Set the ID of the account approving an allowance of its Hbars.
   *
   * @return The ID of the account approving an allowance of its Hbars.
   */
  [[nodiscard]] inline AccountId getOwnerAccountId() const { return mOwnerAccountId; }

  /**
   * Get the ID of the account being allowed to spend the Hbars.
   *
   * @return The ID of the account being allowed to spend the Hbars.
   */
  [[nodiscard]] inline AccountId getSpenderAccountId() const { return mSpenderAccountId; }

  /**
   * Get the amount of Hbars that are being allowed to be spent.
   *
   * @return The amount of Hbars that are being allowed to be spent.
   */
  [[nodiscard]] inline Hbar getAmount() const { return mAmount; }

private:
  /**
   * The ID of the account approving an allowance of its Hbars.
   */
  AccountId mOwnerAccountId;

  /**
   * The ID of the account being allowed to spend the Hbars.
   */
  AccountId mSpenderAccountId;

  /**
   * The amount of Hbars that are being allowed to be spent.
   */
  Hbar mAmount;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_HBAR_ALLOWANCE_H_
