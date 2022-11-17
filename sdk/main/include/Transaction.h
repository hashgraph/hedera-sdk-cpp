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
#ifndef HEDERA_SDK_CPP_TRANSACTION_H_
#define HEDERA_SDK_CPP_TRANSACTION_H_

#include "AccountId.h"
#include "Executable.h"
#include "Hbar.h"
#include "TransactionId.h"

#include <chrono>
#include <functional>
#include <memory>
#include <string>

namespace Hedera
{
namespace internal
{
class Node;
}

class Client;
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
 * Base class for all transactions that can be submitted to a Hedera network.
 *
 * @tparam SdkRequestType The SDK request type.
 */
template<typename SdkRequestType>
class Transaction
  : public Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>
{
public:
  ~Transaction() override = default;

  /**
   * Set the length of time that this Transaction will remain valid.
   *
   * @param duration The desired valid duration to set.
   * @return A reference to this derived Transaction object with the newly-set valid duration.
   */
  SdkRequestType& setValidTransactionDuration(const std::chrono::duration<int64_t>& duration);

  /**
   * Set the maximum transaction fee willing to be paid to execute this Transaction.
   *
   * @param fee The desired maximum transaction fee willing to be paid for this Transaction.
   * @return A reference to this derived Transaction object with the newly-set maximum transaction fee.
   */
  SdkRequestType& setMaxTransactionFee(const Hbar& fee);

  /**
   * Set a memo for this Transaction.
   *
   * @param memo The desired memo for this Transaction.
   * @return A reference to this derived Transaction object with the newly-set memo.
   */
  SdkRequestType& setTransactionMemo(const std::string& memo);

  /**
   * Set the ID for this Transaction.
   *
   * @param id The desired transaction ID to set.
   * @return A reference to this derived Transaction object with the newly-set transaction ID.
   */
  SdkRequestType& setTransactionId(const TransactionId& id);

  /**
   * Get the desired length of time for this Transaction to remain valid upon submission.
   *
   * @return The duration this Transaction will remain valid.
   */
  [[nodiscard]] inline std::chrono::duration<int64_t> getValidTransactionDuration() const
  {
    return mTransactionValidDuration;
  }

  /**
   * Get the maximum transaction fee willing to be paid to execute this Transaction.
   *
   * @return The maximum transaction fee willing to be paid.
   */
  [[nodiscard]] inline Hbar getMaxTransactionFee() const { return mMaxTransactionFee; }

  /**
   * Get the default maximum transaction fee for all Transactions.
   *
   * @return The default maximum transaction fee.
   */
  [[nodiscard]] inline Hbar getDefaultMaxTransactionFee() const { return mDefaultMaxTransactionFee; }

  /**
   * Get the memo for this Transaction.
   *
   * @return The memo for this Transaction.
   */
  [[nodiscard]] inline std::string getTransactionMemo() const { return mTransactionMemo; }

  /**
   * Get the ID of this Transaction.
   *
   * @return The ID of this Transaction.
   */
  [[nodiscard]] inline TransactionId getTransactionId() const { return mTransactionId; }

protected:
  /**
   * Prevent copying and moving to prevent slicing. Use the 'clone()' virtual method instead.
   */
  Transaction() = default;
  Transaction(const Transaction&) = default;
  Transaction& operator=(const Transaction&) = default;
  Transaction(Transaction&&) noexcept = default;
  Transaction& operator=(Transaction&&) noexcept = default;

  /**
   * Derived from Executable. Construct a TransactionResponse object from a TransactionResponse protobuf object.
   *
   * @param response The TransactionResponse protobuf object from which to construct a TransactionResponse object.
   * @return A TransactionResponse object filled with the TransactionResponse protobuf object's data.
   */
  [[nodiscard]] TransactionResponse mapResponse(const proto::TransactionResponse& response) const override;

  /**
   * Derived from Executable. Perform any needed actions for this Transaction when it is being submitted.
   *
   * Currently only sets the ID for this Transaction.
   *
   * @param client The Client being used to submit this Transaction.
   */
  void onExecute(const Client& client) override;

  /**
   * Derived from Executable. Perform any needed actions for this Transaction when a Node has been selected to which to
   * submit this Transaction.
   *
   * Currently only sets the node account ID for this Transaction.
   *
   * @param node The Node to which this Executable is being submitted.
   */
  void onSelectNode(const std::shared_ptr<internal::Node>& node) override;

  /**
   * Sign a TransactionBody protobuf object with a Client and put the signed bytes into a Transaction protobuf object.
   *
   * @param transaction The TransactionBody to sign.
   * @param client      The Client being used to sign the transaction.
   * @return A Transaction protobuf object containing the TransactionBody protobuf object signed by the Client.
   */
  [[nodiscard]] proto::Transaction signTransaction(const proto::TransactionBody& transaction,
                                                   const Client& client) const;

  /**
   * Create a TransactionBody protobuf object from this Transaction object's data.
   *
   * @param client The Client that will sign and submit this Transaction.
   * @return The created TransactionBody protobuf object.
   */
  [[nodiscard]] proto::TransactionBody generateTransactionBody(const Client& client) const;

private:
  /**
   * Helper function used to get the proper maximum transaction fee to pack into a protobuf TransactionBody. The order
   * of priority for maximum transaction fees goes:
   *  1. Manually-set maximum transaction fee for this transaction.
   *  2. Client-set default max transaction fee.
   *  3. Default maximum transaction fee.
   *
   * @param client The Client submitting this Transaction.
   * @return The proper maximum transaction fee to set for this Transaction.
   */
  [[nodiscard]] Hbar getMaxTransactionFee(const Client& client) const;

  /**
   * The default maximum transaction fee.
   */
  const Hbar mDefaultMaxTransactionFee = Hbar(2LL);

  /**
   * The valid transaction duration. Defaults to two minutes.
   */
  std::chrono::duration<int64_t> mTransactionValidDuration = std::chrono::minutes(2);

  /**
   * The account ID of the node sending this Transaction.
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

#endif // HEDERA_SDK_CPP_TRANSACTION_H_
