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
#ifndef HEDERA_SDK_CPP_RECEIPT_STATUS_EXCEPTION_H_
#define HEDERA_SDK_CPP_RECEIPT_STATUS_EXCEPTION_H_

#include "Status.h"
#include "TransactionId.h"

#include <exception>
#include <string>
#include <string_view>

namespace Hedera
{
/**
 * Exception that is thrown when a TransactionReceipt contains a non-successful status (i.e. not Status::SUCCESS) and it
 * is configured to do so.
 */
class ReceiptStatusException : public std::exception
{
public:
  /**
   * Construct with the ID of the transaction that failed and its status.
   *
   * @param transactionId The ID of the transaction that failed.
   * @param status        The status of the transaction.
   */
  explicit ReceiptStatusException(const TransactionId& transactionId, Status status)
    : mTransactionId(transactionId)
    , mStatus(status)
    , mError("receipt for transaction " + mTransactionId.toString() + " raised status " + gStatusToString.at(mStatus))
  {
  }

  /**
   * Get the descriptor message for this error.
   *
   * @return The descriptor message for this error.
   */
  [[nodiscard]] const char* what() const noexcept override { return mError.c_str(); };

  /**
   * The ID of the transaction that failed.
   */
  TransactionId mTransactionId;

  /**
   * The status of the failed transaction.
   */
  Status mStatus = Status::OK;

  /**
   * Descriptive error message.
   */
  std::string mError;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_RECEIPT_STATUS_EXCEPTION_H_
