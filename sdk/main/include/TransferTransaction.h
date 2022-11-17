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
#ifndef HEDERA_SDK_CPP_TRANSFER_TRANSACTION_H_
#define HEDERA_SDK_CPP_TRANSFER_TRANSACTION_H_

#include "Transaction.h"
#include "Transfer.h"

#include <memory>
#include <vector>

namespace Hedera
{
namespace internal
{
class Node;
}
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
/**
 * Transfers cryptocurrency among two or more accounts by making the desired adjustments to their balances. Each
 * transfer list can specify up to 10 adjustments. Each negative amount is withdrawn from the corresponding account (a
 * sender), and each positive one is added to the corresponding account (a receiver). The amounts list must sum to zero.
 * Each amount is a number of tinybars (there are 100,000,000 tinybars in one hbar).  If any sender account fails to
 * have sufficient hbars, then the entire transaction fails, and none of those transfers occur, though the transaction
 * fee is still charged. This transaction must be signed by the keys for all the sending accounts, and for any receiving
 * accounts that have mReceiverSigRequired == \c TRUE. The signatures are in the same order as the accounts, skipping
 * those accounts that don't need a signature.
 */
class TransferTransaction : public Transaction<TransferTransaction>
{
public:
  ~TransferTransaction() override = default;

  /**
   * Derived from Executable. Create a clone of this TransferTransaction.
   *
   * @return A pointer to the created clone.
   */
  [[nodiscard]] std::unique_ptr<Executable> clone() const override;

  /**
   * Add an approved Hbar transfer to be submitted as part of this TransferTransaction.
   *
   * @param accountId The ID of the account associated with this transfer.
   * @param amount    The amount to transfer.
   * @return A reference to this TransferTransaction object with the newly-added approved transfer.
   */
  TransferTransaction& addApprovedHbarTransfer(const std::shared_ptr<AccountId>&, const Hbar& amount);

  /**
   * Add an unapproved Hbar transfer to be submitted as part of this TransferTransaction.
   *
   * @param accountId The ID of the account associated with this transfer.
   * @param amount    The amount to transfer.
   * @return A reference to this TransferTransaction object with the newly-added unapproved transfer.
   */
  TransferTransaction& addUnapprovedHbarTransfer(const std::shared_ptr<AccountId>& accountId, const Hbar& amount);

  /**
   * Get the list of Hbar transfers that have been added to this TransferTransaction.
   *
   * @return The list of Hbar transfers.
   */
  [[nodiscard]] inline std::vector<Transfer> getHbarTransfers() const { return mHbarTransfers; }

protected:
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TransferTransaction object.
   *
   * @param client The Client trying to construct this TransferTransaction.
   * @param node   The Node on which this TransferTransaction will be sent.
   * @return A Transaction protobuf object filled with this TransferTransaction object's data.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Get the gRPC method to call to send this TransferTransaction.
   *
   * @param node The Node to which this TransferTransaction is being sent and from which to retrieve the gRPC method.
   * @return The gRPC method to call to execute this TransferTransaction.
   */
  [[nodiscard]] std::function<
    grpc::Status(grpc::ClientContext*, const proto::Transaction&, proto::TransactionResponse*)>
  getGrpcMethod(const std::shared_ptr<internal::Node>& node) const override;

private:
  /**
   * Allow queries that are not free to create Transaction protobuf objects from TransferTransactions.
   */
  friend class TransactionRecordQuery;

  /**
   * Build a CryptoTransferTransactionBody protobuf object from this TransferTransaction object.
   *
   * @return A pointer to a CryptoTransferTransactionBody protobuf object filled with this TransferTransaction object's
   *         data.
   */
  [[nodiscard]] proto::CryptoTransferTransactionBody* build() const;

  /**
   * Add an Hbar transfer to the Hbar transfers list.
   *
   * @param transfer The Hbar transfer to add.
   */
  void addHbarTransfer(const Transfer& transfer);

  /**
   * The desired Hbar balance adjustments.
   */
  std::vector<Transfer> mHbarTransfers;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSFER_TRANSACTION_H_
