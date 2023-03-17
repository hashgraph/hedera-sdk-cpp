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
#ifndef HEDERA_SDK_CPP_TRANSACTION_RECORD_H_
#define HEDERA_SDK_CPP_TRANSACTION_RECORD_H_

#include "AccountId.h"
#include "EvmAddress.h"
#include "Hbar.h"
#include "HbarTransfer.h"
#include "TokenNftTransfer.h"
#include "TokenTransfer.h"
#include "TransactionId.h"
#include "TransactionReceipt.h"

#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace proto
{
class TransactionRecord;
}

namespace Hedera
{
/**
 * The complete record for a transaction on Hedera that has reached consensus. This is not-free to request and is
 * available for 1 hour after a transaction reaches consensus. A TransactionReceipt can be thought of as a light-weight
 * record which is free to ask for if you just need what it contains. A receipt however lasts for only 180 seconds.
 */
class TransactionRecord
{
public:
  /**
   * Construct a TransactionRecord object from a TransactionRecord protobuf object.
   *
   * @param proto The TransactionRecord protobuf object from which to construct an TransactionRecord object.
   * @return The constructed TransactionRecord object.
   */
  [[nodiscard]] static TransactionRecord fromProtobuf(const proto::TransactionRecord& proto);

  /**
   * Get the receipt of the transaction with which this TransactionRecord is associated.
   *
   * @return The transaction receipt. Uninitialized if no receipt has been generated yet.
   */
  [[nodiscard]] inline std::optional<TransactionReceipt> getReceipt() const { return mReceipt; }

  /**
   * Get the hash of the transaction with which this TransactionRecord is associated.
   *
   * @return The transaction hash.
   */
  [[nodiscard]] inline std::string getTransactionHash() const { return mTransactionHash; }

  /**
   * Get the timestamp of when the transaction with which this TransactionRecord is associated reached consensus.
   *
   * @return The consensus timestamp. Uninitialized if the transaction has not yet reached consensus.
   */
  [[nodiscard]] inline std::optional<std::chrono::system_clock::time_point> getConsensusTimestamp() const
  {
    return mConsensusTimestamp;
  }

  /**
   * Get the ID of the transaction with which this TransactionRecord is associated.
   *
   * @return The transaction ID.
   */
  [[nodiscard]] inline std::optional<TransactionId> getTransactionId() const { return mTransactionID; }

  /**
   * Get the memo of the transaction with which this TransactionRecord is associated.
   *
   * @return The transaction memo.
   */
  [[nodiscard]] inline std::string getTransactionMemo() const { return mMemo; }

  /**
   * Get the fee that was paid to execute the transaction with which this TransactionRecord is associated.
   *
   * @return The transaction fee.
   */
  [[nodiscard]] inline uint64_t getTransactionFee() const { return mTransactionFee; }

  /**
   * Get the list of all crypto transfers that occurred during the execution of the transaction with which this
   * TransactionRecord is associated.
   *
   * @return A list of IDs for accounts that sent/received a crypto transfer, as well as the amount that was
   *         transferred.
   */
  [[nodiscard]] inline std::vector<HbarTransfer> getHbarTransferList() const { return mHbarTransferList; }

  /**
   * Get the list of all fungible token transfers that occurred during the execution of the transaction with which this
   * TransactionRecord is associated.
   *
   * @return A list of token IDs, account IDs, and amounts for each transfer that occurred.
   */
  [[nodiscard]] inline std::vector<TokenTransfer> getTokenTransferList() const { return mTokenTransferList; }

  /**
   * Get the list of all NFT transfers that occurred during the execution of the transaction with which this
   * TransactionRecord is associated.
   *
   * @return A list of NFT IDs and account IDs for each transfer that occurred.
   */
  [[nodiscard]] inline std::vector<TokenNftTransfer> getNftTransferList() const { return mNftTransferList; }

  /**
   * Get the EVM address of the account created by the transaction with which this TransactionRecord is associated.
   *
   * @return The EVM address of the account created by the transaction with which this TransactionRecord is associated.
   *         Uninitialized if no account was created by the transaction.
   */
  [[nodiscard]] inline std::optional<EvmAddress> getEvmAddress() const { return mEvmAddress; }

private:
  /**
   * The status (reach consensus, or failed, or is unknown) and the ID of any new account/file/instance created.
   */
  std::optional<TransactionReceipt> mReceipt;

  /**
   * The hash of the transaction that executed (not the hash of any transaction that failed for having a duplicate
   * transaction ID).
   */
  std::string mTransactionHash;

  /**
   * The consensus timestamp (or uninitialized if the transaction hasn't reached consensus yet).
   */
  std::optional<std::chrono::system_clock::time_point> mConsensusTimestamp;

  /**
   * The ID of the transaction this record represents.
   */
  std::optional<TransactionId> mTransactionID;

  /**
   * The memo that was submitted as part of the transaction (max 100 bytes).
   */
  std::string mMemo;

  /**
   * The actual transaction fee charged, not the original transaction fee value from TransactionBody.
   */
  uint64_t mTransactionFee;

  /**
   * All Hbar transfers as a result of this transaction, such as fees, or transfers performed by the transaction, or by
   * a smart contract it calls, or by the creation of threshold records that it triggers.
   */
  std::vector<HbarTransfer> mHbarTransferList;

  /**
   * All fungible token transfers as a result of this transaction.
   */
  std::vector<TokenTransfer> mTokenTransferList;

  /**
   * All NFT transfers as a result of this transaction.
   */
  std::vector<TokenNftTransfer> mNftTransferList;

  /**
   * The new default EVM address of the account created by transaction with which this TransactionRecord is
   * associated. This field is populated only when the EVM address is not specified in the related transaction body.
   */
  std::optional<EvmAddress> mEvmAddress;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_RECORD_H_
