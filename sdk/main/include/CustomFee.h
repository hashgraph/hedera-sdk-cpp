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
#ifndef HEDERA_SDK_CPP_CUSTOM_FEE_H_
#define HEDERA_SDK_CPP_CUSTOM_FEE_H_

#include "AccountId.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace proto
{
class CustomFee;
}

namespace Hedera
{
class Client;
}

namespace Hedera
{
/**
 * Base class for custom fees. This is assessed during a CryptoTransfer that transfers units of the token to which the
 * fee is attached. A custom fee may be either fixed, fractional, or a royalty, and must specify a fee collector account
 * to receive the assessed fees. Only positive fees may be assessed.
 */
class CustomFee
{
public:
  virtual ~CustomFee() = default;

  /**
   * Create a CustomFee object from a CustomFee protobuf object.
   *
   * @param proto The CustomFee protobuf object from which to create an CustomFee object.
   * @return A pointer to the constructed CustomFee object.
   */
  [[nodiscard]] static std::unique_ptr<CustomFee> fromProtobuf(const proto::CustomFee& proto);

  /**
   * Create a CustomFee object from a byte array.
   *
   * @param bytes The byte array from which to create an CustomFee object.
   * @return A pointer to the constructed CustomFee object.
   */
  [[nodiscard]] static std::unique_ptr<CustomFee> fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Create a clone of this CustomFee object.
   *
   * @return A pointer to the created clone of this CustomFee.
   */
  [[nodiscard]] virtual std::unique_ptr<CustomFee> clone() const = 0;

  /**
   * Construct a CustomFee protobuf object from this CustomFee object.
   *
   * @return A pointer to the created CustomFee protobuf object filled with this CustomFee object's data.
   */
  [[nodiscard]] virtual std::unique_ptr<proto::CustomFee> toProtobuf() const = 0;

  /**
   * Verify the checksums of all the entities involved in this CustomFee.
   *
   * @param client The Client that should be used to validate the checksums.
   * @throws BadEntityException This CustomFee's checksums are not valid.
   */
  virtual void validateChecksums(const Client& client) const;

  /**
   * Construct a byte array from this CustomFee object.
   *
   * @return A byte array filled with this CustomFee object's data.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Get the ID of the desired fee collector account.
   *
   * @return The ID of the desired fee collector account.
   */
  [[nodiscard]] inline AccountId getFeeCollectorAccountId() const { return mFeeCollectorAccountId; }

  /**
   * Get the fee collector exemption policy.
   *
   * @return \c TRUE if fee collectors are currently configured to be exempt from this CustomFee, otherwise \c FALSE.
   */
  [[nodiscard]] inline bool getAllCollectorsAreExempt() const { return mAllCollectorsAreExempt; }

protected:
  /**
   * Prevent public copying and moving to prevent slicing. Use the 'clone()' virtual method instead.
   */
  CustomFee() = default;
  CustomFee(const CustomFee&) = default;
  CustomFee& operator=(const CustomFee&) = default;
  CustomFee(CustomFee&&) noexcept = default;
  CustomFee& operator=(CustomFee&&) noexcept = default;

  /**
   * Create the initial protobuf CustomFee object, populated with this CustomFee's private members, to be used by
   * derived classes' toProtobuf() functions.
   *
   * @return A pointer to CustomFee protobuf object filled-in with this CustomFee's private members.
   */
  [[nodiscard]] std::unique_ptr<proto::CustomFee> initProtobuf() const;

  /**
   * The ID of the account that should receive the fee.
   */
  AccountId mFeeCollectorAccountId;

  /**
   * Should all token fee collection accounts be exempt from this fee?
   */
  bool mAllCollectorsAreExempt = false;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CUSTOM_FEE_H_
