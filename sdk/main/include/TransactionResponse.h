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
#ifndef HEDERA_SDK_CPP_TRANSACTION_RESPONSE_H_
#define HEDERA_SDK_CPP_TRANSACTION_RESPONSE_H_

#include "TransactionId.h"

#include <chrono>
#include <cstdint>

namespace Hedera
{
template<typename SdkRequestType>
class Transaction;

class Client;
class TransactionReceipt;
class TransactionRecord;
}

namespace proto
{
class TransactionResponse;
}

namespace Hedera
{
/**
 * When the client sends the node a transaction of any kind, the node replies with this, which simply says that the
 * transaction passed the pre-check (so the node will submit it to the network) or it failed (so it won't). To learn the
 * consensus result, the client should later obtain a receipt (free), or can buy a more detailed record (not free).
 */
class TransactionResponse
{
public:
  /**
   * Construct a TransactionResponse object from a TransactionResponse protobuf object.
   *
   * @param proto The TransactionResponse protobuf object from which to construct a TransactionResponse object.
   * @return The constructed TransactionResponse object.
   */
  [[nodiscard]] static TransactionResponse fromProtobuf(const proto::TransactionResponse& proto);

  /**
   * Get a TransactionReceipt for this TransactionResponse's corresponding Transaction.
   *
   * @param client The Client to use to submit a TransactionReceiptQuery.
   * @return A TransactionReceipt object containing data about this TransactionResponse's corresponding Transaction.
   * @throws MaxAttemptsExceededException If the TransactionReceiptQuery attempts to execute past the number of
   *                                      allowable attempts.
   * @throws PrecheckStatusException      If this TransactionReceiptQuery fails its pre-check.
   * @throws ReceiptStatusException       If the retrieved TransactionReceipt is not successful, and this
   *                                      TransactionResponse is configured to throw.
   * @throws UninitializedException       If the input Client has not yet been initialized.
   */
  [[nodiscard]] TransactionReceipt getReceipt(const Client& client) const;

  /**
   * Get a TransactionReceipt for the Transaction to which this TransactionResponse is responding.
   *
   * @param client  The Client to use to submit a TransactionReceiptQuery.
   * @param timeout The desired timeout for the execution of the TransactionReceiptQuery.
   * @return A TransactionReceipt object containing data about this TransactionResponse's corresponding Transaction.
   * @throws MaxAttemptsExceededException If the TransactionReceiptQuery attempts to execute past the number of
   *                                      allowable attempts.
   * @throws PrecheckStatusException      If this TransactionReceiptQuery fails its pre-check.
   * @throws ReceiptStatusException       If the retrieved TransactionReceipt is not successful, and this
   *                                      TransactionResponse is configured to throw.
   * @throws UninitializedException       If the input Client has not yet been initialized.
   */
  [[nodiscard]] TransactionReceipt getReceipt(const Client& client, const std::chrono::duration<double>& timeout) const;

  /**
   * Get a TransactionRecord for the Transaction to which this TransactionResponse is responding.
   *
   * @param client The Client to use to submit a TransactionRecordQuery.
   * @return A TransactionRecord object containing data about this TransactionResponse's corresponding Transaction.
   * @throws MaxAttemptsExceededException If the TransactionRecordQuery attempts to execute past the number of
   *                                      allowable attempts.
   * @throws PrecheckStatusException      If this TransactionRecordQuery fails its pre-check.
   * @throws UninitializedException       If the input Client has not yet been initialized.
   */
  [[nodiscard]] TransactionRecord getRecord(const Client& client) const;

  /**
   * Get a TransactionRecord for the Transaction to which this TransactionResponse is responding.
   *
   * @param client  The Client to use to submit a TransactionRecordQuery.
   * @param timeout The desired timeout for the execution of the TransactionRecordQuery.
   * @return A TransactionRecord object containing data about this TransactionResponse's corresponding Transaction.
   * @throws MaxAttemptsExceededException If the TransactionRecordQuery attempts to execute past the number of
   *                                      allowable attempts.
   * @throws PrecheckStatusException      If this TransactionRecordQuery fails its pre-check.
   * @throws UninitializedException       If the input Client has not yet been initialized.
   */
  [[nodiscard]] TransactionRecord getRecord(const Client& client, const std::chrono::duration<double>& timeout) const;

  /**
   * Set this TransactionResponse's TransactionReceipt validation policy.
   *
   * @param validate \c TRUE if this TransactionResponse should throw if it retrieves a TransactionReceipt that does not
   *                 have a successful status (i.e. Status::SUCCESS), otherwise \c FALSE.
   */
  TransactionResponse& setValidateStatus(bool validate);

  /**
   * Get the cost to execute this TransactionResponse's corresponding Transaction if the Transaction's max transaction
   * fee wasn't enough, unspecified otherwise.
   *
   * @return The cost to execute this TransactionResponse's corresponding Transaction.
   */
  [[nodiscard]] inline uint64_t getCost() const { return mCost; }

  /**
   * Determine if this TransactionResponse's corresponding Transaction pre-checks were a success.
   *
   * @return \c TRUE if this TransactionResponse's corresponding Transaction pre-checks were a success, otherwise \c
   *         FALSE.
   */
  [[nodiscard]] inline bool getValidateStatus() const { return mValidateStatus; }

  /**
   * Get the ID of this TransactionResponse's corresponding Transaction.
   *
   * @return The ID of the Transaction.
   */
  [[nodiscard]] inline TransactionId getTransactionId() const { return mTransactionId; }

private:
  /**
   * Allow Transactions to adjust this TransactionResponse's mTransactionId.
   */
  template<typename SdkRequestType>
  friend class Transaction;

  /**
   * The cost to execute this TransactionResponse's corresponding Transaction.
   */
  uint64_t mCost = 0ULL;

  /**
   * Did this TransactionResponse's corresponding Transaction have a successful pre-check?
   */
  bool mValidateStatus = false;

  /**
   * The ID of this TransactionResponse's corresponding Transaction.
   */
  TransactionId mTransactionId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_RESPONSE_H_
