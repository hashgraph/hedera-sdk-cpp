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

#include "AccountId.h"
#include "HbarTransfer.h"
#include "NftId.h"
#include "TokenId.h"
#include "TokenNftTransfer.h"
#include "TokenTransfer.h"
#include "Transaction.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace proto
{
class CryptoTransferTransactionBody;
}

namespace Hedera
{
/**
 * Transfers cryptocurrency among two or more accounts by making the desired adjustments to their balances. Each
 * transfer list can specify up to 10 adjustments. Each negative amount is withdrawn from the corresponding account (a
 * sender), and each positive one is added to the corresponding account (a receiver). The amounts list must sum to zero.
 * Each amount is a number of tinybars (there are 100,000,000 tinybars in one hbar). If any sender account fails to
 * have sufficient hbars, then the entire transaction fails, and none of those transfers occur, though the transaction
 * fee is still charged. This transaction must be signed by the keys for all the sending accounts, and for any receiving
 * accounts that have mReceiverSigRequired == \c TRUE. The signatures are in the same order as the accounts, skipping
 * those accounts that don't need a signature.
 */
class TransferTransaction : public Transaction<TransferTransaction>
{
public:
  TransferTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a CryptoTransfer transaction.
   */
  explicit TransferTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Derived from Executable. Create a clone of this TransferTransaction.
   *
   * @return A pointer to the created clone.
   */
  [[nodiscard]] std::unique_ptr<Executable> clone() const override;

  /**
   * Add an Hbar transfer to be submitted as part of this TransferTransaction.
   *
   * @param accountId The ID of the account associated with this transfer.
   * @param amount    The amount of Hbar to transfer.
   * @return A reference to this TransferTransaction object with the newly-added Hbar transfer.
   * @throws IllegalStateException If This TransferTransaction is frozen.
   */
  TransferTransaction& addHbarTransfer(const AccountId& accountId, const Hbar& amount);

  /**
   * Add a token transfer to be submitted as part of this TransferTransaction.
   *
   * @param tokenId   The ID of the token associated with this transfer.
   * @param accountId The ID of the account associated with this transfer.
   * @param amount    The number of tokens to transfer.
   * @return A reference to this TransferTransaction object with the newly-added token transfer.
   * @throws IllegalStateException If This TransferTransaction is frozen.
   */
  TransferTransaction& addTokenTransfer(const TokenId& tokenId, const AccountId& accountId, const int64_t& amount);

  /**
   * Add an NFT transfer to be submitted as part of this TransferTransaction.
   *
   * @param nftId             The ID of the NFT associated with this transfer.
   * @param senderAccountId   The ID of the account sending the NFT.
   * @param receiverAccountId The ID of the receiving sending the NFT.
   * @return A reference to this TransferTransaction object with the newly-added NFT transfer.
   * @throws IllegalStateException If This TransferTransaction is frozen.
   */
  TransferTransaction& addNftTransfer(const NftId& nftId,
                                      const AccountId& senderAccountId,
                                      const AccountId& receiverAccountId);

  /**
   * Add a token transfer with decimals to be submitted as part of this TransferTransaction.
   *
   * @param tokenId   The ID of the token associated with this transfer.
   * @param accountId The ID of the account associated with this transfer.
   * @param amount    The number of tokens to transfer.
   * @param decimals  The number of decimals in the transfer amount.
   * @return A reference to this TransferTransaction object with the newly-added token transfer.
   * @throws IllegalStateException If This TransferTransaction is frozen.
   * @throws std::invalid_argument If decimals does not match previously set decimals for this token.
   */
  TransferTransaction& addTokenTransferWithDecimals(const TokenId& tokenId,
                                                    const AccountId& accountId,
                                                    const int64_t& amount,
                                                    uint32_t decimals);

  /**
   * Add an approved Hbar transfer to be submitted as part of this TransferTransaction.
   *
   * @param accountId The ID of the account associated with this transfer.
   * @param amount    The amount of Hbar to transfer.
   * @return A reference to this TransferTransaction object with the newly-added approved Hbar transfer.
   * @throws IllegalStateException If This TransferTransaction is frozen.
   */
  TransferTransaction& addApprovedHbarTransfer(const AccountId& accountId, const Hbar& amount);

  /**
   * Add an approved token transfer to be submitted as part of this TransferTransaction.
   *
   * @param tokenId   The ID of the token associated with this transfer.
   * @param accountId The ID of the account associated with this transfer.
   * @param amount    The number of tokens to transfer.
   * @return A reference to this TransferTransaction object with the newly-added approved token transfer.
   * @throws IllegalStateException If This TransferTransaction is frozen.
   */
  TransferTransaction& addApprovedTokenTransfer(const TokenId& tokenId,
                                                const AccountId& accountId,
                                                const int64_t& amount);

  /**
   * Add an approved NFT transfer to be submitted as part of this TransferTransaction.
   *
   * @param nftId             The ID of the NFT associated with this transfer.
   * @param senderAccountId   The ID of the account sending the NFT.
   * @param receiverAccountId The ID of the receiving sending the NFT.
   * @return A reference to this TransferTransaction object with the newly-added approved NFT transfer.
   * @throws IllegalStateException If This TransferTransaction is frozen.
   */
  TransferTransaction& addApprovedNftTransfer(const NftId& nftId,
                                              const AccountId& senderAccountId,
                                              const AccountId& receiverAccountId);

  /**
   * Add an approved token transfer with decimals to be submitted as part of this TransferTransaction.
   *
   * @param tokenId   The ID of the token associated with this transfer.
   * @param accountId The ID of the account associated with this transfer.
   * @param amount    The number of tokens to transfer.
   * @param decimals  The number of decimals in the transfer amount.
   * @return A reference to this TransferTransaction object with the newly-added approved token transfer.
   * @throws IllegalStateException If This TransferTransaction is frozen.
   * @throws std::invalid_argument If decimals does not match previously set decimals for this token.
   */
  TransferTransaction& addApprovedTokenTransferWithDecimals(const TokenId& tokenId,
                                                            const AccountId& accountId,
                                                            const int64_t& amount,
                                                            uint32_t decimals);

  /**
   * Get all Hbar transfers that have been added to this TransferTransaction.
   *
   * @return The map of Hbar transfers.
   */
  [[nodiscard]] std::unordered_map<AccountId, Hbar> getHbarTransfers() const;

  /**
   * Get all token transfers that have been added to this TransferTransaction.
   *
   * @return The map of token transfers.
   */
  [[nodiscard]] std::unordered_map<TokenId, std::unordered_map<AccountId, int64_t>> getTokenTransfers() const;

  /**
   * Get all NFT transfers that have been added to this TransferTransaction.
   *
   * @return The map of NFT transfers.
   */
  [[nodiscard]] std::unordered_map<TokenId, std::vector<TokenNftTransfer>> getNftTransfers() const;

  /**
   * Get the expected decimals for token transfers that have been added to this TransferTransaction.
   *
   * @return The map of expected decimals.
   */
  [[nodiscard]] std::unordered_map<TokenId, uint32_t> getTokenIdDecimals() const;

private:
  /**
   * Allow queries that are not free to create Transaction protobuf objects from TransferTransactions.
   */
  friend class TransactionRecordQuery;

  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TransferTransaction object.
   *
   * @param client The Client trying to construct this TransferTransaction.
   * @param node   The Node to which this TransferTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TransferTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this TransferTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TransferTransaction to a Node.
   *
   * @param client   The Client submitting this TransferTransaction.
   * @param deadline The deadline for submitting this TransferTransaction.
   * @param node     Pointer to the Node to which this TransferTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;

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
  void doHbarTransfer(const HbarTransfer& transfer);

  /**
   * Add a token transfer to the token transfers list.
   *
   * @param transfer The token transfer to add.
   */
  void doTokenTransfer(const TokenTransfer& transfer);

  /**
   * Add an NFT transfer to the NFT transfers list.
   *
   * @param transfer The NFT transfer to add.
   */
  void doNftTransfer(const TokenNftTransfer& transfer);

  /**
   * The desired Hbar balance adjustments.
   */
  std::vector<HbarTransfer> mHbarTransfers;

  /**
   * The desired token adjustments.
   */
  std::vector<TokenTransfer> mTokenTransfers;

  /**
   * The desired NFT adjustments.
   */
  std::vector<TokenNftTransfer> mNftTransfers;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSFER_TRANSACTION_H_
