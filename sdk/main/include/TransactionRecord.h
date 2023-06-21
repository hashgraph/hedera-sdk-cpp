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
#include "AssessedCustomFee.h"
#include "ContractFunctionResult.h"
#include "EvmAddress.h"
#include "Hbar.h"
#include "HbarTransfer.h"
#include "TokenAssociation.h"
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
   * The result of the executed smart contract function or the result of the executed smart contract constructor.
   */
  std::optional<ContractFunctionResult> mContractFunctionResult;

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
   * All custom fees that were assessed during a CryptoTransfer, and must be paid if the transaction status resolved to
   * SUCCESS.
   */
  std::vector<AssessedCustomFee> mAssessedCustomFees;

  /**
   * All token associations implicitly created while handling this transaction.
   */
  std::vector<TokenAssociation> mTokenAssociations;

  /**
   * The new default EVM address of the account created by transaction with which this TransactionRecord is
   * associated. This field is populated only when the EVM address is not specified in the related transaction body.
   */
  std::optional<EvmAddress> mEvmAddress;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_RECORD_H_
