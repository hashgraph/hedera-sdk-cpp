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
#ifndef HEDERA_SDK_CPP_CONTRACT_EXECUTE_TRANSACTION_H_
#define HEDERA_SDK_CPP_CONTRACT_EXECUTE_TRANSACTION_H_

#include "ContractFunctionParameters.h"
#include "ContractId.h"
#include "Hbar.h"
#include "Transaction.h"

#include <cstddef>
#include <string_view>
#include <vector>

namespace proto
{
class ContractCallTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * The transaction calls a function of the given smart contract instance, giving it functionParameters as its input. The
 * call can use at maximum the given amount of gas – the paying account will not be charged for any unspent gas. If this
 * function results in data being stored, an amount of gas is calculated that reflects this storage burden. The amount
 * of gas used, as well as other attributes of the transaction, e.g. size, and number of signatures to be verified,
 * determine the fee for the transaction – which is charged to the paying account.
 *
 * Transaction Signing Requirements:
 *  - The key of the transaction fee-paying account.
 */
class ContractExecuteTransaction : public Transaction<ContractExecuteTransaction>
{
public:
  ContractExecuteTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a ContractCall transaction.
   */
  explicit ContractExecuteTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the ID of the contract to call.
   *
   * @param fileId The ID of the contract to call.
   * @return A reference to this ContractExecuteTransaction object with the newly-set contract ID.
   * @throws IllegalStateException If this ContractExecuteTransaction is frozen.
   */
  ContractExecuteTransaction& setContractId(const ContractId& contractId);

  /**
   * Set the maximum amount of gas to use for the function call.
   *
   * @param gas The maximum amount of gas to use for the function call.
   * @return A reference to this ContractExecuteTransaction object with the newly-set amount of gas.
   * @throws IllegalStateException If this ContractExecuteTransaction is frozen.
   */
  ContractExecuteTransaction& setGas(const uint64_t& gas);

  /**
   * Set the amount to pay for the function call.
   *
   * @param amount The amount to pay for the function call.
   * @return A reference to this ContractExecuteTransaction object with the newly-set payable amount.
   * @throws IllegalStateException If this ContractExecuteTransaction is frozen.
   */
  ContractExecuteTransaction& setPayableAmount(const Hbar& amount);

  /**
   * Set the function parameters for the function call.
   *
   * @param parameters The parameters to pass to function call.
   * @return A reference to this ContractExecuteTransaction object with the newly-set function parameters.
   * @throws IllegalStateException If this ContractExecuteTransaction is frozen.
   */
  ContractExecuteTransaction& setFunctionParameters(const std::vector<std::byte>& parameters);

  /**
   * Set the function to call, optionally with its parameters.
   *
   * @param name       The name of the function to call.
   * @param parameters The function parameters to pass to the function call.
   * @return A reference to this ContractExecuteTransaction object with the newly-set function name and parameters.
   * @throws IllegalStateException If this ContractExecuteTransaction is frozen.
   */
  ContractExecuteTransaction& setFunction(std::string_view name,
                                          const ContractFunctionParameters& parameters = ContractFunctionParameters());

  /**
   * Get the ID of the contract to call.
   *
   * @return The ID of the contract to call
   */
  [[nodiscard]] inline ContractId getContractId() const { return mContractId; }

  /**
   * Get the maximum amount of gas to use for the function call.
   *
   * @return The maximum amount of gas to use for the function call.
   */
  [[nodiscard]] inline uint64_t getGas() const { return mGas; }

  /**
   * Get the amount to pay for the function call.
   *
   * @return The amount to pay for the function call.
   */
  [[nodiscard]] inline Hbar getPayableAmount() const { return mPayableAmount; }

  /**
   * Get the function parameters for the function call.
   *
   * @return The function parameters for the function call.
   */
  [[nodiscard]] inline std::vector<std::byte> getFunctionParameters() const { return mFunctionParameters; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this ContractExecuteTransaction object.
   *
   * @param client The Client trying to construct this ContractExecuteTransaction.
   * @param node   The Node to which this ContractExecuteTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this ContractExecuteTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                ContractExecuteTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this ContractExecuteTransaction to a Node.
   *
   * @param client   The Client submitting this ContractExecuteTransaction.
   * @param deadline The deadline for submitting this ContractExecuteTransaction.
   * @param node     Pointer to the Node to which this ContractExecuteTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the ContractExecuteTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a ContractCallTransactionBody protobuf object from this ContractExecuteTransaction object.
   *
   * @return A pointer to a ContractCallTransactionBody protobuf object filled with this ContractExecuteTransaction
   *         object's data.
   */
  [[nodiscard]] proto::ContractCallTransactionBody* build() const;

  /**
   * The ID of the contract to call.
   */
  ContractId mContractId;

  /**
   * The maximum amount of gas to use for the function call.
   */
  uint64_t mGas = 0ULL;

  /**
   * The amount to pay for the function call
   */
  Hbar mPayableAmount = Hbar(0LL);

  /**
   * The function parameters for the function call.
   */
  std::vector<std::byte> mFunctionParameters;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_EXECUTE_TRANSACTION_H_
