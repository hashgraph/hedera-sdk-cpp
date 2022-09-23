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
#ifndef CONTRACT_EXECUTE_TRANSACTION_H_
#define CONTRACT_EXECUTE_TRANSACTION_H_

#include "ContractId.h"
#include "Hbar.h"
#include "Transaction.h"

#include "helper/InitType.h"

#include <string>
#include <unordered_map>

namespace Hedera
{
class AccountId;
class ContractFunctionParameters;
class TransactionId;
}

namespace proto
{
class ContractCallTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Call a function of the given smart contract instance, giving it
 * functionParameters as its inputs. The call can use at maximum the given
 * amount of gas – the paying account will not be charged for any unspent gas.
 *
 * If this function results in data being stored, an amount of gas is calculated
 * that reflects this storage burden.
 *
 * The amount of gas used, as well as other attributes of the transaction, e.g.
 * size, number of signatures to be verified, determine the fee for the
 * transaction – which is charged to the paying account.
 */
class ContractExecuteTransaction
  : public Transaction<ContractExecuteTransaction>
{
public:
  /**
   * Constructor.
   */
  ContractExecuteTransaction();

  /**
   * Construct from a map of transaction ID's to their corresponding account
   * ID's and protobuf transactions.
   *
   * @param transactions Map of transaction IDs to their corresponding account
   *                     ID's and protobuf transactions.
   */
  explicit ContractExecuteTransaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Construct from a protobuf transaction object.
   *
   * @param transaction The protobuf transaction object from which to construct
   *                    this transaction.
   */
  explicit ContractExecuteTransaction(
    const proto::TransactionBody& transaction);

  /**
   * Derived from Transaction. Validate the checksums.
   *
   * @param client The client with which to validate the checksums.
   */
  virtual void validateChecksums(const Client& client) const override;

  /**
   * Build a contract call transaction protobuf message based on the data in
   * this class.
   *
   * @return A contract call transaction protobuf message.
   */
  proto::ContractCallTransactionBody build() const;

  /**
   * Sets the contract instance to call.
   *
   * @param contractId The contract ID to be set.
   * @return Reference to this ContractExecuteTransaction object.
   */
  ContractExecuteTransaction& setContractId(const ContractId& contractId);

  /**
   * Sets the maximum amount of gas to use for the call.
   *
   * @param gas The gas to be set.
   * @return Reference to this ContractExecuteTransaction object.
   */
  ContractExecuteTransaction& setGas(const int64_t& gas);

  /**
   * Sets the number of Hbars sent with this function call.
   *
   * @param amount The amount of Hbars to be set.
   * @return Reference to this ContractExecuteTransaction object.
   */
  ContractExecuteTransaction& setPayableAmount(const Hbar& amount);

  /**
   * Sets the function parameters as their raw bytes.
   *
   * Use this instead of setFunction(std::string, ContractFunctionParameters) if
   * you have already pre-encoded a solidity function call.
   *
   * @param functionParameters The function parameters to be set.
   * @return Reference to this ContractExecuteTransaction object.
   */
  ContractExecuteTransaction& setFunctionParameters(
    const std::string& functionParameters);

  /**
   * Sets the function name to call.
   *
   * The function will be called with no parameters. Use
   * setFunction(std::string, ContractFunctionParameters) to call a function
   * with parameters.
   *
   * @param name The string to be set as the function name.
   * @return Reference to this ContractExecuteTransaction object.
   */
  ContractExecuteTransaction& setFunction(const std::string name);

  /**
   * Sets the function to call, and the parameters to pass to the function.
   *
   * @param name   The string to be set as the function name.
   * @param params The function parameters to be set.
   * @return Reference to this ContractExecuteTransaction object.
   */
  ContractExecuteTransaction& setFunction(
    const std::string& name,
    const ContractFunctionParameters& params);

  /**
   * Extract the contract ID.
   *
   * @return The contract ID.
   */
  inline InitType<ContractId> getContractId() const { return mContractId; }

  /**
   * Extract the gas.
   *
   * @return The gas.
   */
  inline int64_t getGas() const { return mGas; }

  /**
   * Extract the payable amount.
   *
   * @return The payable amount in Hbar.
   */
  inline Hbar getPayableAmount() const { return mPayableAmount; }

  /**
   * Extract the function parameters.
   *
   * @return The function parameters.
   */
  inline std::string getFunctionParameters() const
  {
    return mFunctionParameters;
  }

private:
  /**
   * Initialize from the transaction body.
   */
  void initFromTransactionBody();

  /**
   * The contract ID of the contract to call.
   */
  InitType<ContractId> mContractId;

  /**
   * The maximum amount of gas to use for the call.
   */
  int64_t mGas;

  /**
   * The number of tinybars sent (the function must be payable if this is
   * non-zero).
   */
  Hbar mPayableAmount;

  /**
   * Which function to call, and the parameters to pass to the function.
   */
  std::string mFunctionParameters;
};

} // namespace Hedera

#endif // CONTRACT_EXECUTE_TRANSACTION_H_