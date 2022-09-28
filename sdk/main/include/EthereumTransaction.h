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
#ifndef ETHEREUM_TRANSACTION_H_
#define ETHEREUM_TRANSACTION_H_

#include "FileId.h"
#include "Hbar.h"
#include "Transaction.h"

#include "helper/InitType.h"

#include "ethereum_transaction.pb.h"

#include <string>
#include <unordered_map>

namespace Hedera
{
class AccountId;
class Client;
class TransactionId;
}

namespace proto
{
class TransactionBody;
}

namespace Hedera
{
class EthereumTransaction : public Transaction<EthereumTransaction>
{
public:
  /**
   * Constructor.
   */
  EthereumTransaction();

  /**
   * Construct from a map of transaction ID's to their corresponding account
   * ID's and protobuf transactions.
   *
   * @param transactions Map of transaction IDs to their corresponding account
   *                     ID's and protobuf transactions.
   */
  explicit EthereumTransaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Construct from a protobuf transaction object.
   *
   * @param transaction The protobuf transaction object from which to construct
   *                    this transaction.
   */
  explicit EthereumTransaction(const proto::TransactionBody& transaction);

  /**
   * Derived from Transaction. Validate the checksums.
   *
   * @param client The client with which to validate the checksums.
   */
  virtual void validateChecksums(const Client& client) const override;

  /**
   * Build an ethereum transaction protobuf message based on the data in
   * this class.
   *
   * @return An ethereum transaction protobuf message.
   */
  proto::EthereumTransactionBody build() const;

  /**
   * Sets the raw Ethereum transaction (RLP encoded type 0, 1, and 2). Complete
   * unless the callDataFileId is set.
   *
   * @param ethereumData The raw ethereum transaction bytes.
   * @return Reference to this EthereumTransaction object.
   */
  EthereumTransaction& setEthereumData(const std::string& ethereumData);

  /**
   * For large transactions (for example contract create) this should be used to
   * set the FileId of an HFS file containing the callData of the ethereumData.
   * The data in the ethereumData will be re-written with the callData element
   * as a zero length string with the original contents in the referenced file
   * at time of execution. The ethereumData will need to be "rehydrated" with
   * the callData for signature validation to pass.
   *
   * @param fileId File ID of an HFS file containing the callData.
   * @return Reference to this EthereumTransaction object.
   */
  EthereumTransaction& setCallDataFileId(const FileId& fileId);

  /**
   * Sets the maximum amount that the payer of the hedera transaction
   * is willing to pay to complete the transaction.
   *
   * Ordinarily the account with the ECDSA alias corresponding to the public
   * key that is extracted from the ethereum_data signature is responsible for
   * fees that result from the execution of the transaction. If that amount of
   * authorized fees is not sufficient then the payer of the transaction can be
   * charged, up to but not exceeding this amount. If the ethereum_data
   * transaction authorized an amount that was insufficient then the payer will
   * only be charged the amount needed to make up the difference. If the gas
   * price in the transaction was set to zero then the payer will be assessed
   * the entire fee.
   *
   * @param maxGasAllowanceHbar The maximum gas allowance.
   * @return Reference to this EthereumTransaction object.
   */
  EthereumTransaction& setMaxGasAllowanceHbar(const Hbar& maxGasAllowance);

  /**
   * Extract the raw ethereum transaction.
   *
   * @return The raw ethereum transaction.
   */
  inline std::string getEthereumData() const { return mEthereumData; }

  /**
   * Extract the file ID of the call data.
   *
   * @return The file ID of the call data.
   */
  inline InitType<FileId> getCallDataFileId() const { return mCallData; }

  /**
   * Gets the maximum amount that the payer of the hedera transaction
   * is willing to pay to complete the transaction.
   *
   * @return The max gas allowance.
   */
  inline Hbar getMaxGasAllowanceHbar() const { return mMaxGasAllowance; }

private:
  /**
   * Initialize from the transaction body.
   */
  void initFromTransactionBody();

  /**
   * The raw Ethereum transaction (RLP encoded type 0, 1, and 2). Complete
   * unless the callData field is set.
   */
  std::string mEthereumData;

  /**
   * For large transactions (for example contract create) this is the callData
   * of the ethereumData. The data in the ethereumData will be re-written with
   * the callData element as a zero length string with the original contents in
   * the referenced file at time of execution. The ethereumData will need to be
   * "rehydrated" with the callData for signature validation to pass.
   */
  InitType<FileId> mCallData;

  /**
   * The maximum amount that the payer of the hedera transaction is willing to
   * pay to complete the transaction.
   *
   * Ordinarily the account with the ECDSA alias corresponding to the public
   * key that is extracted from the ethereum_data signature is responsible for
   * fees that result from the execution of the transaction. If that amount of
   * authorized fees is not sufficient then the payer of the transaction can be
   * charged, up to but not exceeding this amount. If the ethereum_data
   * transaction authorized an amount that was insufficient then the payer will
   * only be charged the amount needed to make up the difference. If the gas
   * price in the transaction was set to zero then the payer will be assessed
   * the entire fee.
   */
  Hbar mMaxGasAllowance;
};

} // namespace Hedera

#endif // ETHEREUM_TRANSACTION_H_