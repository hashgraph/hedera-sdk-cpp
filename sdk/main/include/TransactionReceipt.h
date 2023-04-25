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
#ifndef HEDERA_SDK_CPP_TRANSACTION_RECEIPT_H_
#define HEDERA_SDK_CPP_TRANSACTION_RECEIPT_H_

#include "AccountId.h"
#include "ContractId.h"
#include "ExchangeRateSet.h"
#include "FileId.h"
#include "Status.h"

#include <optional>

namespace proto
{
class TransactionReceipt;
}

namespace Hedera
{
/**
 * The summary of a transaction's result so far. If the transaction has not reached consensus, this result will be
 * necessarily incomplete.
 */
class TransactionReceipt
{
public:
  /**
   * Construct a TransactionReceipt object from a TransactionReceipt protobuf object.
   *
   * @param proto The TransactionReceipt protobuf object from which to construct an TransactionReceipt object.
   * @return The constructed TransactionReceipt object.
   */
  [[nodiscard]] static TransactionReceipt fromProtobuf(const proto::TransactionReceipt& proto);

  /**
   * Get the transaction status.
   *
   * @return The transaction status.
   */
  [[nodiscard]] inline Status getStatus() const { return mStatus; }

  /**
   * Get the ID of the account that was created as a result of the submitted transaction. Only valid is this
   * TransactionReceipt is for an AccountCreateTransaction.
   *
   * @return The ID of the created account. Uninitialized if the corresponding Transaction was not an
   * AccountCreateTransaction.
   */
  [[nodiscard]] inline std::optional<AccountId> getAccountId() const { return mAccountId; };

  /**
   * Get the ID of the file that was created as a result of the submitted transaction. Only valid is this
   * TransactionReceipt is for an FileCreateTransaction.
   *
   * @return The ID of the created file. Uninitialized if the corresponding Transaction was not an
   * FileCreateTransaction.
   */
  [[nodiscard]] inline std::optional<FileId> getFileId() const { return mFileId; };

  /**
   * Get the ID of the contract that was created as a result of the submitted transaction. Only valid is this
   * TransactionReceipt is for an ContractCreateTransaction.
   *
   * @return The ID of the created contract. Uninitialized if the corresponding Transaction was not an
   * ContractCreateTransaction.
   */
  [[nodiscard]] inline std::optional<ContractId> getContractId() const { return mContractId; };

  /**
   * Get the exchange rates in effect when the transaction reached consensus.
   *
   * @return The current and next exchange rates.
   */
  [[nodiscard]] inline std::optional<ExchangeRateSet> getExchangeRates() const { return mExchangeRates; }

private:
  /**
   * The consensus status of the transaction; is UNKNOWN if consensus has not been reached, or if the associated
   * transaction did not have a valid payer signature.
   */
  Status mStatus = Status::UNKNOWN;

  /**
   * In the receipt of a CryptoCreate, the ID of the newly created account.
   */
  std::optional<AccountId> mAccountId;

  /**
   * In the receipt of a FileCreate, the ID of the newly created file.
   */
  std::optional<FileId> mFileId;

  /**
   * In the receipt of a ContractCreate, the ID of the newly created contract.
   */
  std::optional<ContractId> mContractId;

  /**
   * The exchange rates in effect when the transaction reached consensus.
   */
  std::optional<ExchangeRateSet> mExchangeRates;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_RECEIPT_H_
