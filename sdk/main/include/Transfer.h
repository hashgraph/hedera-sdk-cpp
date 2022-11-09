/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#ifndef TRANSFER_H_
#define TRANSFER_H_

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
   * Construct a Transfer from a protobuf AccountAmount.
   *
   * @param proto The protobuf AccountAmount.
   * @return A Transfer with the protobuf AccountAmount data.
   */
  static Transfer fromProtobuf(const proto::AccountAmount& proto);

  /**
   * Construct a protobuf AccountAmount from a Transfer
   *
   * @return Pointer to a protobuf AccountAmount with this Transfer's data.
   */
  proto::AccountAmount* toProtobuf() const;

  /**
   * Set the account ID.
   *
   * @param accountId The account ID to set.
   * @return Reference to this Transfer object.
   */
  Transfer& setAccountId(const std::shared_ptr<AccountId>& accountId);

  /**
   * Set the transfer amount.
   *
   * @param amount The transfer amount to set.
   * @return Reference to this Transfer object.
   */
  Transfer& setAmount(const Hbar& amount);

  /**
   * Set if this transaction is approved.
   *
   * @param approved \c TRUE if this transfer is approved, otherwise \c FALSE
   * @return Reference to this Transfer object.
   */
  Transfer& setApproved(bool approved);

  /**
   * Extract the account ID.
   *
   * @return The account ID.
   */
  inline std::shared_ptr<AccountId> getAccountId() const { return mAccountId; }

  /**
   * Extract the transfer amount.
   *
   * @return The transfer amount.
   */
  inline Hbar getAmount() const { return mAmount; }

  /**
   * Determine if this transfer is approved or not.
   *
   * @return \c TRUE if this transfer is approved, otherwise \c FALSE
   */
  inline bool getApproval() const { return mIsApproved; }

private:
  /**
   * The account ID associated with this transfer.
   */
  std::shared_ptr<AccountId> mAccountId;

  /**
   * The amount of Hbar transferred.
   */
  Hbar mAmount;

  /**
   * Is this transfer approved or not?
   */
  bool mIsApproved;
};

} // namespace Hedera

#endif // TRANSFER_H_
