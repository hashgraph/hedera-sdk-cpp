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
 * @tparam SdkRequestType  The SDK request type.
 */
template<typename SdkRequestType>
class Transaction
  : public Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>
{
protected:
  /**
   * Default destructor
   */
  virtual ~Transaction() = default;

  /**
   * Derived from Executable. Construct a transaction protobuf object from this transaction.
   *
   * @return The transaction protobuf object that contains this transaction information.
   */
  virtual proto::Transaction makeRequest() const = 0;

  /**
   * Derived from Executable. Create a response object from a protobuf response object.
   *
   * @param response The protobuf response object.
   * @return The response object with the response data.
   */
  virtual TransactionResponse mapResponse(const proto::Response& response) const = 0;

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
  inline std::chrono::duration<double> getValidTransactionDuration() const { mTransactionValidDuration; }

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
  inline Hbar getDefaultMaxTransactionFee() const { return DEFAULT_MAX_TRANSACTION_FEE; }

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

private:
  /**
   * The default valid transaction duration.
   */
  const static std::chrono::duration<double> DEFAULT_VALID_TRANSACTION_DURATION = std::chrono::minutes(2);

  /**
   * The default maximum transaction fee.
   */
  const static Hbar DEFAULT_MAX_TRANSACTION_FEE = Hbar::from(2LL);

  /**
   * The protobuf transaction body for this transaction.
   */
  std::shared_ptr<proto::TransactionBody> mSourceTransactionBody;

  /**
   * The valid transaction duration. Defaults to two minutes.
   */
  std::chrono::duration<double> mTransactionValidDuration = DEFAULT_VALID_TRANSACTION_DURATION;

  /**
   * The maximum transaction fee.
   */
  Hbar mMaxTransactionFee = DEFAULT_MAX_TRANSACTION_FEE;

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