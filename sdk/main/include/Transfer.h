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
#ifndef HEDERA_SDK_CPP_TRANSFER_H_
#define HEDERA_SDK_CPP_TRANSFER_H_

#include "AccountId.h"
#include "Hbar.h"

#include <memory>

namespace proto
{
class AccountAmount;
}

namespace Hedera
{
class Transfer
{
public:
  /**
   * Create a Transfer object from a AccountAmount protobuf object.
   *
   * @param proto The AccountAmount protobuf object from which to create an Transfer object.
   * @return The created Transfer object.
   */
  static Transfer fromProtobuf(const proto::AccountAmount& proto);

  /**
   * Construct a AccountAmount protobuf object from this Transfer object.
   *
   * @return A pointer to a created AccountAmount protobuf object filled with this Transfer object's data.
   */
  [[nodiscard]] std::unique_ptr<proto::AccountAmount> toProtobuf() const;

  /**
   * Set the ID of the account associated with this Transfer.
   *
   * @param accountId The ID of the account sent, received, is sending, or is receiving Hbar.
   * @return A reference to this Transfer object with the newly-set account ID.
   */
  Transfer& setAccountId(const std::shared_ptr<AccountId>& accountId);

  /**
   * Set the amount of Hbar that was transferred into (positive) or out of (negative) the account.
   *
   * @param amount The amount transferred.
   * @return A reference to this Transfer object with the newly-set transfer amount.
   */
  Transfer& setAmount(const Hbar& amount);

  /**
   * Set if this transaction is approved.
   *
   * @param approved \c TRUE if this transfer is approved, otherwise \c FALSE.
   * @return A reference to this Transfer object with the newly-set approval status.
   */
  Transfer& setApproved(bool approved);

  /**
   * Get the ID of the account associated with this Transfer.
   *
   * @return The account ID.
   */
  [[nodiscard]] inline std::shared_ptr<AccountId> getAccountId() const { return mAccountId; }

  /**
   * Get the amount that was or will be transferred.
   *
   * @return The transfer amount.
   */
  [[nodiscard]] inline Hbar getAmount() const { return mAmount; }

  /**
   * Determine if this Transfer is approved or not.
   *
   * @return \c TRUE if this transfer is approved, otherwise \c FALSE
   */
  [[nodiscard]] inline bool getApproval() const { return mIsApproved; }

private:
  /**
   * The ID of the account associated with this Transfer.
   */
  std::shared_ptr<AccountId> mAccountId;

  /**
   * The amount of Hbar transferred or to be transferred. Defaults to 0.
   */
  Hbar mAmount = Hbar(0ULL);

  /**
   * Is/Was this transfer approved or not? Defaults to \c FALSE.
   */
  bool mIsApproved = false;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSFER_H_
