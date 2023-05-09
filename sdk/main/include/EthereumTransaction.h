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
#ifndef HEDERA_SDK_CPP_ETHEREUM_TRANSACTION_H_
#define HEDERA_SDK_CPP_ETHEREUM_TRANSACTION_H_

#include "FileId.h"
#include "Hbar.h"
#include "Transaction.h"

#include <cstddef>
#include <optional>
#include <vector>

namespace proto
{
class EthereumTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * The raw Ethereum transaction (RLP encoded type 0, 1, and 2) will hold signed Ethereum transactions and execute them
 * as Hedera transactions in a prescribed manner.
 *
 * Transaction Signing Requirements:
 *  - The key of the transaction fee-paying account.
 */
class EthereumTransaction : public Transaction<EthereumTransaction>
{
public:
  EthereumTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent an EthereumTransaction transaction.
   */
  explicit EthereumTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Set the raw Ethereum transaction (RLP encoded type 0, 1, and 2). This is mutually exclusive with mCallDataFileId
   * and will reset the value of mCallDataFileId if it is set.
   *
   * @param ethereumData The raw Ethereum transaction.
   * @return A reference to this EthereumTransaction object with the newly-set ethereum data.
   * @throws IllegalStateException If this EthereumTransaction is frozen.
   */
  EthereumTransaction& setEthereumData(const std::vector<std::byte>& ethereumData);

  /**
   * Set the ID of the file that contains the call data. This is mutually exclusive with mEthereumData and will reset
   * the value of mEthereumData if it is set.
   *
   * @param fileId The ID of the file that contains the call data.
   * @return A reference to this EthereumTransaction object with the newly-set call data file ID.
   * @throws IllegalStateException If this EthereumTransaction is frozen.
   */
  EthereumTransaction& setCallDataFileId(const FileId& fileId);

  /**
   * Set the maximum amount that the payer of the Hedera transaction is willing to pay to complete this
   * EthereumTransaction.
   *
   * @param maxGasAllowance The maximum amount that the payer of the Hedera transaction is willing to pay to complete
   *                        this EthereumTransaction.
   * @return A reference to this EthereumTransaction object with the newly-set transfer maximum gas allowance.
   * @throws IllegalStateException If this EthereumTransaction is frozen.
   */
  EthereumTransaction& setMaxGasAllowance(const Hbar& maxGasAllowance);

  /**
   * Get the raw Ethereum transaction.
   *
   * @return The raw Ethereum transaction. Returns uninitialized if a value has not yet been set, or if a call data file
   *         ID has been set most recently.
   */
  [[nodiscard]] inline std::optional<std::vector<std::byte>> getEthereumData() const { return mEthereumData; }

  /**
   * Get the ID of the file that contains the call data.
   *
   * @return The ID of the file that contains the call data. Returns uninitialized if a value has not yet been set, or
   *         if ethereum data has been set most recently.
   */
  [[nodiscard]] inline std::optional<FileId> getCallDataFileId() const { return mCallDataFileId; }

  /**
   * Get the maximum amount that the payer of the Hedera transaction is willing to pay to complete this
   * EthereumTransaction.
   *
   * @return The maximum amount that the payer of the Hedera transaction is willing to pay to complete this
   *         EthereumTransaction.
   */
  [[nodiscard]] inline Hbar getMaxGasAllowance() const { return mMaxGasAllowance; }

private:
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this EthereumTransaction object.
   *
   * @param client The Client trying to construct this EthereumTransaction.
   * @param node   The Node to which this EthereumTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this EthereumTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                EthereumTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this EthereumTransaction to a Node.
   *
   * @param client   The Client submitting this EthereumTransaction.
   * @param deadline The deadline for submitting this EthereumTransaction.
   * @param node     Pointer to the Node to which this EthereumTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;

  /**
   * Build a EthereumTransactionBody protobuf object from this EthereumTransaction object.
   *
   * @return A pointer to a EthereumTransactionBody protobuf object filled with this EthereumTransaction object's data.
   */
  [[nodiscard]] proto::EthereumTransactionBody* build() const;

  /**
   * The raw Ethereum transaction (RLP encoded type 0, 1, and 2).
   */
  std::optional<std::vector<std::byte>> mEthereumData;

  /**
   * The ID of the file that contains the call data.
   *
   * For large transactions (for example contract creation) this should be used to set the FileId of an HFS file
   * containing the call data of the ethereum data. The data in the ethereum will be re-written with the call data
   * element as a zero-length string with the original contents in the referenced file at the time of execution. The
   * ethereum data will need to be "rehydrated" with the call data for signature validation to pass.
   */
  std::optional<FileId> mCallDataFileId;

  /**
   * The maximum amount that the payer of the Hedera transaction is willing to pay to complete the transaction.
   *
   * Ordinarily the account with the ECDSA alias corresponding to the public key that is extracted from the
   * ethereum data signature is responsible for fees that result from the execution of the transaction. If that amount
   * of authorized fees is not sufficient then the payer of the transaction can be charged, up to but not exceeding this
   * amount. If the ethereum data transaction authorized an amount that was insufficient then the payer will only be
   * charged the amount needed to make up the difference. If the gas price in the transaction was set to zero then the
   * payer will be assessed the entire fee.
   */
  Hbar mMaxGasAllowance = Hbar(0LL);
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ETHEREUM_TRANSACTION_H_
