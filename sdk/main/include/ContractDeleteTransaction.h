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
#ifndef CONTRACT_DELETE_TRANSACTION_H_
#define CONTRACT_DELETE_TRANSACTION_H_

#include "AccountId.h"
#include "ContractId.h"
#include "Transaction.h"

#include "helper/InitType.h"

#include <unordered_map>

namespace Hedera
{
class TransactionId;
}

namespace proto
{
class ContractDeleteTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * At consensus, marks a contract as deleted and transfers its remaining Hbars,
 * if any, to a designated receiver. After a contract is deleted, it can no
 * longer be called.
 *
 * If the target contract is immutable (that is, was created without an admin
 * key), then this transaction resolves to MODIFYING_IMMUTABLE_CONTRACT.
 *
 * --- Signing Requirements ---
 * 1. The admin key of the target contract must sign.
 * 2. If the transfer account or contract has receiverSigRequired, its
 * associated key must also sign
 */
class ContractDeleteTransaction : public Transaction<ContractDeleteTransaction>
{
public:
  /**
   * Constructor.
   */
  ContractDeleteTransaction();

  /**
   * Construct from a map of transaction ID's to their corresponding account
   * ID's and protobuf transactions.
   *
   * @param transactions Map of transaction IDs to their corresponding account
   *                     ID's and protobuf transactions.
   */
  explicit ContractDeleteTransaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Construct from a protobuf transaction object.
   *
   * @param transaction The protobuf transaction object from which to construct
   *                    this transaction.
   */
  explicit ContractDeleteTransaction(const proto::TransactionBody& transaction);

  /**
   * Derived from Transaction. Validate the checksums.
   *
   * @param client The client with which to validate the checksums.
   */
  virtual void validateChecksums(const Client& client) const override;

  /**
   * Build a contract delete transaction protobuf message based on the data in
   * this class.
   *
   * @return A contract delete transaction protobuf message.
   */
  proto::ContractDeleteTransactionBody build() const;

  /**
   * Sets the contract ID which should be deleted.
   *
   * @param contractId The contract ID to be set.
   * @return Reference to this ContractDeleteTransaction object.
   */
  ContractDeleteTransaction& setContractId(const ContractId& contractId);

  /**
   * Sets the account ID which will receive all remaining Hbars.
   *
   * This is mutually exclusive with setTransferContractId(ContractId).
   *
   * @param transferAccountId The account ID to be set.
   * @return Reference to this ContractDeleteTransaction object.
   */
  ContractDeleteTransaction& setTransferAccountId(
    const AccountId& transferAccountId);

  /**
   * Sets the contract ID which will receive all remaining Hbars.
   *
   * This is mutually exclusive with setTransferAccountId(AccountId).
   *
   * @param transferContractId The contract ID to be set.
   * @return Reference to this ContractDeleteTransaction object.
   */
  ContractDeleteTransaction& setTransferContractId(
    const ContractId& transferContractId);

  /**
   * Extract the contract ID.
   *
   * @return The contract ID.
   */
  inline InitType<ContractId> getContractId() const { return mContractId; }

  /**
   * Extract the transfer account ID.
   *
   * @return The account ID of the account that will receive the remaining
   *         Hbars.
   */
  inline InitType<AccountId> getTransferAccountId() const
  {
    return mTransferAccountId;
  }

  /**
   * Extract the transfer contract ID.
   *
   * @return The contract ID that will receive the remaining Hbars.
   */
  inline InitType<ContractId> getTransferContractId() const
  {
    return mTransferContractId;
  }

private:
  /**
   * Initialize the transaction body.
   */
  void initFromTransactionBody();

  /**
   * The ID of the contract to be deleted.
   */
  InitType<ContractId> mContractId;

  /**
   * The ID of an account to receive any remaining Hbars from the deleted
   * contract. Mutually exclusive from mTransferContractId.
   */
  InitType<AccountId> mTransferAccountId;

  /**
   * The ID of a contract to receive any remaining Hbars from the deleted
   * contract. Mutually exclusive from mTransferAccountId.
   */
  InitType<ContractId> mTransferContractId;

  /**
   * If set to true, means this is a "synthetic" system transaction being used
   * to alert mirror nodes that the contract is being permanently removed from
   * the ledger.
   *
   * IMPORTANT: User transactions cannot set this field to true, as permanent
   * removal is always managed by the ledger itself. Any
   * ContractDeleteTransactionBody submitted to HAPI with permanent_removal=true
   * will be rejected with precheck status
   * PERMANENT_REMOVAL_REQUIRES_SYSTEM_INITIATION.
   */
  bool mPermanentRemoval;
};

} // namespace Hedera

#endif // CONTRACT_DELETE_TRANSACTION_H_