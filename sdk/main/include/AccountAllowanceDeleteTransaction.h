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
#ifndef HEDERA_SDK_CPP_ACCOUNT_ALLOWANCE_DELETE_TRANSACTION_H_
#define HEDERA_SDK_CPP_ACCOUNT_ALLOWANCE_DELETE_TRANSACTION_H_

#include "TokenNftAllowance.h"
#include "Transaction.h"

namespace proto
{
class CryptoDeleteAllowanceTransactionBody;
class TransactionBody;
}

namespace Hedera
{
class NftId;
}

namespace Hedera
{
/**
 * A transaction called by the token owner to delete allowances for NFTs only. In order to delete an existing Hbar or
 * fungible token allowance the AccountAllowanceApproveTransaction API should be used with an amount of 0.
 *
 * The total number of NFT serial number deletions contained within the transaction body cannot exceed 20.
 */
class AccountAllowanceDeleteTransaction : public Transaction<AccountAllowanceDeleteTransaction>
{
public:
  AccountAllowanceDeleteTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a CryptoDeleteAllowance transaction.
   */
  explicit AccountAllowanceDeleteTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Remove all NFT allowances from an account.
   *
   * @param nftId The ID of the NFT to remove as an allowance.
   * @param owner The ID of the account from which to remove the allowances.
   * @return A reference to this AccountAllowanceDeleteTransaction object with the added allowance removal.
   * @throws IllegalStateException If this AccountAllowanceDeleteTransaction is frozen.
   */
  AccountAllowanceDeleteTransaction& deleteAllTokenNftAllowances(const NftId& nftId, const AccountId& owner);

  /**
   * Get the list of a NFT allowance removals added to this AccountAllowanceDeleteTransaction.
   *
   * @return The list of a NFT allowance removals added to this AccountAllowanceDeleteTransaction.
   */
  [[nodiscard]] inline std::vector<TokenNftAllowance> getTokenNftAllowanceDeletions() const
  {
    return mNftAllowanceDeletions;
  }

private:
  friend class WrappedTransaction;
  
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this AccountAllowanceDeleteTransaction
   * object.
   *
   * @param client The Client trying to construct this AccountAllowanceDeleteTransaction.
   * @param node   The Node to which this AccountAllowanceDeleteTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this AccountAllowanceDeleteTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                AccountAllowanceApproveTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this AccountAllowanceDeleteTransaction to a Node.
   *
   * @param client   The Client submitting this AccountAllowanceDeleteTransaction.
   * @param deadline The deadline for submitting this AccountAllowanceDeleteTransaction.
   * @param node     Pointer to the Node to which this AccountAllowanceDeleteTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;
  /**
   * Derived from Transaction. Build and add the AccountAllowanceDeleteTransaction protobuf representation to the
   * Transaction protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Build a CryptoDeleteAllowanceTransactionBody protobuf object from this AccountAllowanceDeleteTransaction object.
   *
   * @return A pointer to a CryptoDeleteAllowanceTransactionBody protobuf object filled with this
   *         AccountAllowanceDeleteTransaction object's data.
   */
  [[nodiscard]] proto::CryptoDeleteAllowanceTransactionBody* build() const;

  /**
   * The list of NFT allowances to be deleted.
   */
  std::vector<TokenNftAllowance> mNftAllowanceDeletions;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ACCOUNT_ALLOWANCE_DELETE_TRANSACTION_H_
