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
#ifndef HEDERA_SDK_CPP_CONTRACT_DELETE_TRANSACTION_H_
#define HEDERA_SDK_CPP_CONTRACT_DELETE_TRANSACTION_H_

#include "AccountId.h"
#include "ContractId.h"
#include "Transaction.h"

#include <optional>

namespace proto
{
class ContractDeleteTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * A transaction that deletes a smart contract from a Hedera network. Once a smart contract is marked deleted, you will
 * not be able to modify any of the contract's properties.
 *
 * If a smart contract did not have an admin key defined, you cannot delete the smart contract. You can verify the smart
 * contract was deleted by submitting a smart contract info query to the network. If a smart contract has an associated
 * Hbar balance, you will need to transfer the balance to another Hedera account.
 *
 * Transaction Signing Requirements:
 *  - If the admin key was defined for the smart contract it is required to sign the transaction.
 *  - The client operator's (fee payer account) private key is required to sign the transaction.
 */
class ContractDeleteTransaction : public Transaction<ContractDeleteTransaction>
{
public:
  ContractDeleteTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a ContractDeleteInstance transaction.
   */
  explicit ContractDeleteTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the contract to delete.
   *
   * @param contractId The ID of the contract to delete.
   * @return A reference to this ContractDeleteTransaction object with the newly-set contract ID.
   * @throws IllegalStateException If this ContractDeleteTransaction is frozen.
   */
  ContractDeleteTransaction& setContractId(const ContractId& contractId);

  /**
   * Set the ID of the account that will receive the deleted smart contract's remaining Hbars. This is mutually
   * exclusive with mTransferContractId, and will reset the value of mTransferAccountId if it is set.
   *
   * @param accountId The ID of the account that will receive the deleted smart contract's remaining Hbars.
   * @return A reference to this ContractDeleteTransaction object with the newly-set transfer account ID.
   * @throws IllegalStateException If this ContractDeleteTransaction is frozen.
   */
  ContractDeleteTransaction& setTransferAccountId(const AccountId& accountId);

  /**
   * Set the ID of the contract that will receive the deleted smart contract's remaining Hbars. This is mutually
   * exclusive with mTransferAccountId, and will reset the value of mTransferContractId if it is set.
   *
   * @param contractId The ID of the contract that will receive the deleted smart contract's remaining Hbars.
   * @return A reference to this ContractDeleteTransaction object with the newly-set transfer contract ID.
   * @throws IllegalStateException If this ContractDeleteTransaction is frozen.
   */
  ContractDeleteTransaction& setTransferContractId(const ContractId& contractId);

  /**
   * Get the ID of the contract to delete.
   *
   * @return The ID of the contract to delete.
   */
  [[nodiscard]] inline ContractId getContractId() const { return mContractId; }

  /**
   * Get the ID of the account that will receive the deleted smart contract's remaining Hbars.
   *
   * @return The ID of the account that will receive the deleted smart contract's remaining Hbars. Returns uninitialized
   *         if a value has not yet been set, or if a transfer contract ID has been set most recently.
   */
  [[nodiscard]] inline std::optional<AccountId> getTransferAccountId() const { return mTransferAccountId; }

  /**
   * Get the ID of the contract that will receive the deleted smart contract's remaining Hbars.
   *
   * @return The ID of the contract that will receive the deleted smart contract's remaining Hbars. Returns
   *         uninitialized if a value has not yet been set, or if a transfer account ID has been set most recently.
   */
  [[nodiscard]] inline std::optional<ContractId> getTransferContractId() const { return mTransferContractId; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this ContractDeleteTransaction object.
   *
   * @param client The Client trying to construct this ContractDeleteTransaction.
   * @param node   The Node to which this ContractDeleteTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this ContractDeleteTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                ContractDeleteTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this ContractDeleteTransaction to a Node.
   *
   * @param client   The Client submitting this ContractDeleteTransaction.
   * @param deadline The deadline for submitting this ContractDeleteTransaction.
   * @param node     Pointer to the Node to which this ContractDeleteTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the ContractDeleteTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a ContractDeleteTransactionBody protobuf object from this ContractDeleteTransaction object.
   *
   * @return A pointer to a ContractDeleteTransactionBody protobuf object filled with this ContractDeleteTransaction
   *         object's data.
   */
  [[nodiscard]] proto::ContractDeleteTransactionBody* build() const;

  /**
   * The ID of the contract to delete.
   */
  ContractId mContractId;

  /**
   * The ID of the account that will receive the deleted smart contract's remaining Hbars.
   */
  std::optional<AccountId> mTransferAccountId;

  /**
   * The ID of the contract that will receive the deleted smart contract's remaining Hbars.
   */
  std::optional<ContractId> mTransferContractId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_DELETE_TRANSACTION_H_
