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
#ifndef TRANSFER_TRANSACTION_H_
#define TRANSFER_TRANSACTION_H_

#include "Transaction.h"
#include "Transfer.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace Hedera
{
class TransactionRecordQuery;
class TransactionResponse;
}

namespace proto
{
class CryptoTransferTransactionBody;
class Response;
class Transaction;
}

namespace Hedera
{
class TransferTransaction : public Transaction<TransferTransaction>
{
public:
  /**
   * Default destructor.
   */
  ~TransferTransaction() override = default;

  /**
   * Add an approved Hbar transfer to this transaction.
   *
   * @param accountId The account ID with which to transfer.
   * @param amount    The amount to transfer.
   * @return Reference to this TransferTransaction object.
   */
  TransferTransaction& addApprovedHbarTransfer(const AccountId& accountId, const Hbar& amount);

  /**
   * Add an unapproved Hbar transfer to this transaction.
   *
   * @param accountId The account ID with which to transfer.
   * @param amount    The amount to transfer.
   * @return Reference to this TransferTransaction object.
   */
  TransferTransaction& addUnapprovedHbarTransfer(const AccountId& accountId, const Hbar& amount);

  /**
   * Extract the list of Hbar transfers.
   *
   * @return The list of Hbar transfers.
   */
  [[nodiscard]] inline std::vector<Transfer> getHbarTransfers() const { return mHbarTransfers; }

protected:
  /**
   * Derived from Executable. Construct a Transaction protobuf from this TransferTransaction.
   *
   * @param client The Client submitting this TransferTransaction.
   * @return A Transaction protobuf that contains this TransferTransaction's data.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client, const std::shared_ptr<Node>&) const override;

  /**
   * Derived from Executable. Get the gRPC method to call to transfer between accounts.
   *
   * @param node The Node from which to retrieve the function.
   * @return The gRPC method to call to execute this TransferTransaction.
   */
  [[nodiscard]] std::function<
    grpc::Status(grpc::ClientContext*, const proto::Transaction&, proto::TransactionResponse*)>
  getGrpcMethod(const std::shared_ptr<Node>& node) const override;

private:
  /**
   * Allow queries that are not free to create TransferTransactions.
   */
  friend class TransactionRecordQuery;

  /**
   * Build this TransferTransaction into a protobuf CryptoCreateTransactionBody.
   *
   * @return Pointer to a protobuf CryptoCreateTransactionBody.
   */
  [[nodiscard]] proto::CryptoTransferTransactionBody* build() const;

  /**
   * Add an Hbar transfer to the Hbar transfers list.
   *
   * @param transfer Hbar transfer to add.
   */
  void addHbarTransfer(const Transfer& transfer);

  /**
   * The list of transfers to be sent as a part of this transaction.
   */
  std::vector<Transfer> mHbarTransfers;
};

} // namespace Hedera

#endif // TRANSFER_TRANSACTION_H_
