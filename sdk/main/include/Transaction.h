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
#include <utility>
#include <variant>
#include <vector>

namespace Hedera
{
class AccountAllowanceApproveTransaction;
class AccountAllowanceDeleteTransaction;
class AccountCreateTransaction;
class AccountDeleteTransaction;
class AccountUpdateTransaction;
class ContractCreateTransaction;
class ContractDeleteTransaction;
class ContractExecuteTransaction;
class EthereumTransaction;
class FileCreateTransaction;
class FileDeleteTransaction;
class PrivateKey;
class TransactionResponse;
class TransferTransaction;
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
 * Base class for all transactions that can be submitted to a Hedera network.
 *
 * @tparam SdkRequestType The SDK request type.
 */
template<typename SdkRequestType>
class Transaction
  : public Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>
{
public:
  /**
   * Construct a Transaction derived class from a byte array. The bytes can be a protobuf encoded TransactionBody,
   * Transaction, or SignedTransaction. Since C++ return types must be known at compile time and the type of Transaction
   * to create may not be known at compile time, a std::variant is used to encompass all possible Transactions. Usage of
   * this return type would look like the following:
   *
   * std::vector<std::byte> bytes;
   *                                                              The Transaction type here doesn't matter and is an
   *                                     vvvvvvvvvvvvvvvvvvvvvvvv unfortunate, ugly byproduct of this approach.
   * auto [index, variant] = Transaction<AccountCreateTransaction>::fromBytes(bytes);
   *
   * switch (index)
   * {
   *    case 0:
   *    {
   *        AccountCreateTransaction tx = std::get<0>(variant);
   *        ** do stuff with tx here **
   *        break;
   *    }
   *    case 1:
   *    {
   *        TransferTransaction tx = std::get<1>(variant);
   *        ** do stuff with tx here **
   *        break;
   *    }
   *    ...
   * }
   *
   * @param bytes The bytes from which to construct a Transaction.
   * @return A pair which contains an index into the variant, as well as the created Transaction.
   * @throws std::invalid_argument If unable to construct a Transaction from the input bytes.
   */
  [[nodiscard]] static std::pair<int,
                                 std::variant<AccountCreateTransaction,
                                              TransferTransaction,
                                              AccountUpdateTransaction,
                                              AccountDeleteTransaction,
                                              AccountAllowanceApproveTransaction,
                                              AccountAllowanceDeleteTransaction,
                                              ContractCreateTransaction,
                                              ContractDeleteTransaction,
                                              FileCreateTransaction,
                                              FileDeleteTransaction,
                                              ContractExecuteTransaction,
                                              EthereumTransaction>>
  fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Sign this Transaction with the given PrivateKey. Signing a Transaction with a key that has already been used to
   * sign will be ignored.
   *
   * @param key The PrivateKey with which to sign this Transaction.
   * @return A reference to this derived Transaction object with the signature.
   * @throws IllegalStateException If this Transaction object is not frozen.
   */
  SdkRequestType& sign(const PrivateKey* key);

  /**
   * Sign this Transaction with a given PublicKey and a signing callback. Signing a Transaction with a key that has
   * already been used to sign will be ignored.
   *
   * @param key    The PublicKey with which to sign this Transaction.
   * @param signer The callback function to use to sign this Transaction.
   * @return A reference to this derived Transaction object with the signature.
   * @throws IllegalStateException If this Transaction object is not frozen.
   */
  SdkRequestType& signWith(const std::shared_ptr<PublicKey>& key,
                           const std::function<std::vector<std::byte>(const std::vector<std::byte>&)>& signer);

  /**
   * Freeze this transaction with a Client. The Client's operator will be used to generate a transaction ID, and the
   * client's network will be used to generate a list of node account IDs.
   *
   * @param client The Client with which to freeze this Transaction.
   * @return A reference to this derived Transaction object, now frozen.
   * @throws UninitializedException If Client operator has not been initialized.
   */
  SdkRequestType& freezeWith(const Client& client);

  /**
   * Set the length of time that this Transaction will remain valid.
   *
   * @param duration The desired length of time to keep this Transaction valid.
   * @return A reference to this derived Transaction object with the newly-set valid duration.
   */
  SdkRequestType& setValidTransactionDuration(const std::chrono::duration<double>& duration);

  /**
   * Set the maximum transaction fee willing to be paid to execute this Transaction.
   *
   * @param fee The desired maximum transaction fee willing to be paid to execute this Transaction.
   * @return A reference to this derived Transaction object with the newly-set maximum transaction fee.
   */
  SdkRequestType& setMaxTransactionFee(const Hbar& fee);

  /**
   * Set the memo for this Transaction.
   *
   * @param memo The desired memo for this Transaction.
   * @return A reference to this derived Transaction object with the newly-set memo.
   */
  SdkRequestType& setTransactionMemo(const std::string& memo);

  /**
   * Set the ID for this Transaction.
   *
   * @param id The desired transaction ID for this Transaction.
   * @return A reference to this derived Transaction object with the newly-set transaction ID.
   */
  SdkRequestType& setTransactionId(const TransactionId& id);

  /**
   * Set the transaction ID regeneration policy for this Transaction.
   *
   * @param regenerate \c TRUE if it is desired for this Transaction to regenerate a transaction ID upon receiving a
   *                   TRANSACTION_EXPIRED response from the network after submission, otherwise \c FALSE.
   * @return A reference to this derived Transaction object with the newly-set transaction ID regeneration policy.
   */
  SdkRequestType& setRegenerateTransactionIdPolicy(bool regenerate);

  /**
   * Get the desired length of time for this Transaction to remain valid upon submission.
   *
   * @return The length of time this Transaction will remain valid.
   */
  [[nodiscard]] inline std::chrono::duration<double> getValidTransactionDuration() const
  {
    return mTransactionValidDuration;
  }

  /**
   * Get the desired maximum transaction fee willing to be paid to execute this Transaction.
   *
   * @return The desired maximum transaction fee willing to be paid.
   */
  [[nodiscard]] inline std::optional<Hbar> getMaxTransactionFee() const { return mMaxTransactionFee; }

  /**
   * Get the memo for this Transaction.
   *
   * @return The memo for this Transaction.
   */
  [[nodiscard]] inline std::string getTransactionMemo() const { return mTransactionMemo; }

  /**
   * Get the desired ID for this Transaction.
   *
   * @return The desired ID for this Transaction.
   */
  [[nodiscard]] inline TransactionId getTransactionId() const { return mTransactionId; }

  /**
   * Get the desired transaction ID regeneration policy of this Transaction.
   *
   * @return \c TRUE if this Transaction should regenerate its transaction ID upon receipt of a TRANSACTION_EXPIRED
   *         response from the network, otherwise \c FALSE.
   */
  [[nodiscard]] inline std::optional<bool> getRegenerateTransactionIdPolicy() const
  {
    return mTransactionIdRegenerationPolicy;
  }

protected:
  /**
   * Prevent public copying and moving to prevent slicing. Use the 'clone()' virtual method instead.
   */
  Transaction() = default;
  Transaction(const Transaction&) = default;
  Transaction& operator=(const Transaction&) = default;
  Transaction(Transaction&&) noexcept = default;
  Transaction& operator=(Transaction&&) noexcept = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   */
  explicit Transaction(const proto::TransactionBody& transactionBody);

  /**
   * Derived from Executable. Perform any needed actions for this Transaction when a Node has been selected to which to
   * submit this Transaction.
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
   * @throws UninitializedException If the input client has no operator with which to sign this Transaction.
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

  /**
   * Check and make sure this Transaction isn't frozen.
   *
   * @throws IllegalStateException If this Transaction is frozen.
   */
  void requireNotFrozen() const;

private:
  /**
   * Derived from Executable. Construct a TransactionResponse object from a TransactionResponse protobuf object.
   *
   * @param response The TransactionResponse protobuf object from which to construct a TransactionResponse object.
   * @return A TransactionResponse object filled with the TransactionResponse protobuf object's data.
   */
  [[nodiscard]] TransactionResponse mapResponse(const proto::TransactionResponse& response) const override;

  /**
   * Derived from Executable. Get the status response code for a submitted Transaction from a TransactionResponse
   * protobuf object.
   *
   * @param response The TransactionResponse protobuf object from which to grab the Transaction status response code.
   * @return The Transaction status response code of the input TransactionResponse protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::TransactionResponse& response) const override;

  /**
   * Derived from Executable. Determine the ExecutionStatus of this Transaction after being submitted.
   *
   * @param status   The response status from the network.
   * @param client   The Client that submitted this Transaction.
   * @param response The TransactionResponse protobuf object received from the network in response to submitting this
   *                 Transaction.
   * @return The status of the submitted Transaction.
   */
  [[nodiscard]]
  typename Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
    ExecutionStatus
    determineStatus(Status status,
                    const Client& client,
                    [[maybe_unused]] const proto::TransactionResponse& response) override;

  /**
   * Derived from Executable. Perform any needed actions for this Transaction when it is being submitted.
   *
   * @param client The Client being used to submit this Transaction.
   * @throws UninitializedException If the client doesn't have an AccountId from which to generate a TransactionId.
   */
  void onExecute(const Client& client) override;

  /**
   * Helper function used to get the proper maximum transaction fee to pack into a protobuf TransactionBody. The order
   * of priority for maximum transaction fees goes:
   *  1. Manually-set maximum transaction fee for this Transaction.
   *  2. Client-set default max transaction fee.
   *  3. Default maximum transaction fee.
   *
   * @param client The Client submitting this Transaction.
   * @return The proper maximum transaction fee to set for this Transaction.
   */
  [[nodiscard]] Hbar getMaxTransactionFee(const Client& client) const;

  /**
   * Container of PublicKey and signer function pairs to use to sign this Transaction.
   */
  std::vector<
    std::pair<std::shared_ptr<PublicKey>, std::function<std::vector<std::byte>(const std::vector<std::byte>&)>>>
    mSignatures;

  /**
   * Is this Transaction frozen? \c TRUE if yes, otherwise \c FALSE.
   */
  bool mIsFrozen = false;

  /**
   * The length of time this Transaction will remain valid.
   */
  std::chrono::duration<double> mTransactionValidDuration = std::chrono::minutes(2);

  /**
   * The account ID of the Node sending this Transaction.
   */
  AccountId mNodeAccountId;

  /**
   * The maximum transaction fee willing to be paid to execute this Transaction.
   */
  std::optional<Hbar> mMaxTransactionFee;

  /**
   * The memo to be associated with this Transaction.
   */
  std::string mTransactionMemo;

  /**
   * The ID of this Transaction.
   */
  TransactionId mTransactionId;

  /**
   * Should this Transaction regenerate its TransactionId upon a TRANSACTION_EXPIRED response from the network? If not
   * set, this Transaction will use the Client's set transaction ID regeneration policy. If that's not set, the default
   * behavior is to regenerate the transaction ID.
   */
  std::optional<bool> mTransactionIdRegenerationPolicy;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_H_
