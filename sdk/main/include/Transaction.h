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
#include <functional>
#include <memory>
#include <string>

namespace Hedera
{
class AccountCreateTransaction;
class Channel;
class TransactionResponse;
}

namespace proto
{
class Transaction;
class TransactionBody;
class TransactionResponse;
}

namespace grpc
{
class ClientContext;
class Status;
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
  inline Hbar getDefaultMaxTransactionFee() const { return mDefaultMaxTransactionFee; }

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
   * Derived from Executable. Perform any operations needed when this transaction is being executed.
   *
   * @param client The Client executing this transaction.
   */
  void onExecute(const Client& client) override;

  /**
   * Derived from Executable. Perform any needed actions for this Transaction when a Node has been selected to which to
   * send this Transaction.
   *
   * @param node The Node to which this Executable is being sent.
   */
  void onSelectNode(const std::shared_ptr<Node>& node) override;

  /**
   * Sign a protobuf TransactionBody with a Client and put the signed bytes into a protobuf Transaction.
   *
   * @param transaction The TransactionBody to sign.
   * @param client      The Client being used to sign the transaction.
   * @return A protobuf Transaction containing the TransactionBody signed by the Client.
   */
  proto::Transaction signTransaction(const proto::TransactionBody& transaction, const Client& client) const;

  /**
   * Generate a protobuf TransactionBody with this transaction's data.
   *
   * @param client The Client that is generating this transaction.
   * @return A protobuf TransactionBody with this transaction's data.
   */
  proto::TransactionBody generateTransactionBody(const Client& client) const;

private:
  /**
   * Helper function used to get the proper maximum transaction fee to pack into a protobuf TransactionBody. The order
   * of priority for maximum transaction fees goes:
   *  1. Manually-set maximum transaction fee for this transaction.
   *  2. Client-set default max transaction fee.
   *  3. Default maximum transaction fee.
   *
   * @param client The Client submitting this transaction.
   * @return The proper maximum transaction fee to set for the protobuf TransactionBody.
   */
  [[nodiscard]] Hbar getMaxTransactionFee(const Client& client) const;

  /**
   * The default maximum transaction fee.
   */
  const Hbar mDefaultMaxTransactionFee = Hbar(2LL);

  /**
   * The valid transaction duration. Defaults to two minutes.
   */
  std::chrono::duration<double> mTransactionValidDuration = std::chrono::minutes(2);

  /**
   * The account ID of the node sending this transaction.
   */
  std::shared_ptr<AccountId> mNodeAccountId;

  /**
   * The maximum transaction fee.
   */
  Hbar mMaxTransactionFee = mDefaultMaxTransactionFee;

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