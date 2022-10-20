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
#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include "Executable.h"
#include "Hbar.h"
#include "TransactionId.h"

#include <chrono>
#include <memory>
#include <string>

namespace Hedera
{
class AccountCreateTransaction;
class TransactionResponse;
}

namespace proto
{
class Transaction;
class TransactionBody;
class TransactionResponse;
}

namespace Hedera
{
/**
 * Base class for all transactions that can be submitted to Hedera.
 *
 * @tparam SdkRequestType The SDK request type.
 */
template<typename SdkRequestType>
class Transaction
  : public Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>
{
public:
  /**
   * Set the valid transaction duration.
   *
   * @param duration The duration to set.
   * @return Reference to this derived Transaction object.
   */
  SdkRequestType& setValidTransactionDuration(const std::chrono::duration<double>& duration);

  /**
   * Set the max transaction fee.
   *
   * @param fee The max transaction fee to set.
   * @return Reference to this derived Transaction object.
   */
  SdkRequestType& setMaxTransactionFee(const Hbar& fee);

  /**
   * Set the transaction memo.
   *
   * @param memo The transaction memo to set.
   * @return Reference to this derived Transaction object.
   */
  SdkRequestType& setTransactionMemo(const std::string& memo);

  /**
   * Set the transaction ID.
   *
   * @param id The transaction ID to set.
   * @return Reference to this derived Transaction object.
   */
  SdkRequestType& setTransactionId(const TransactionId& id);

  /**
   * Extract the valid transaction duration.
   *
   * @return The valid transaction duration.
   */
  inline std::chrono::duration<double> getValidTransactionDuration() const { return mTransactionValidDuration; }

  /**
   * Extract the max transaction fee.
   *
   * @return The max transaction fee.
   */
  inline Hbar getMaxTransactionFee() const { return mMaxTransactionFee; }

  /**
   * Extract the default max transaction fee.
   *
   * @return The default max transaction fee.
   */
  inline Hbar getDefaultMaxTransactionFee() const { return Hbar(2LL); }

  /**
   * Extract the transaction memo.
   *
   * @return The transaction memo.
   */
  inline std::string getTransactionMemo() const { return mTransactionMemo; }

  /**
   * Extract the transaction ID.
   *
   * @return The transaction ID.
   */
  inline TransactionId getTransactionId() const { return mTransactionId; }

protected:
  /**
   * Default destructor
   */
  virtual ~Transaction() = default;

  /**
   * Derived from Executable. Create a transaction response object from a protobuf transaction response object.
   *
   * @param response The protobuf transaction response object.
   * @return The transaction response object with the response data.
   */
  TransactionResponse mapResponse(const proto::TransactionResponse& response) const override;

  /**
   * Sign a protobuf TransactionBody with a Client and put the signed bytes into a protobuf Transaction.
   *
   * @param transaction The TransactionBody to sign.
   * @param client      The Client being used to sign the transaction.
   * @return A protobuf Transaction containing the TransactionBody signed by the Client.
   */
  proto::Transaction signTransaction(const proto::TransactionBody& transaction, const Client& client) const;

private:
  /**
   * The valid transaction duration. Defaults to two minutes.
   */
  std::chrono::duration<double> mTransactionValidDuration = std::chrono::minutes(2);

  /**
   * The maximum transaction fee.
   */
  Hbar mMaxTransactionFee = Hbar(2LL);

  /**
   * The transaction memo.
   */
  std::string mTransactionMemo;

  /**
   * The transaction ID.
   */
  TransactionId mTransactionId;
};

} // namespace Hedera

#endif // TRANSACTION_H_