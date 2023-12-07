/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_TOKEN_DELETE_PARTITION_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOKEN_DELETE_PARTITION_TRANSACTION_H_

#include "TokenId.h"
#include "Transaction.h"

namespace proto
{
class TokenDeletePartitionDefinitionTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Marks a token partition as deleted, though it will remain in the ledger. The operation must be signed by the
 * specified partition key of the parent Token. If the partition key is not set, the Transaction will result in
 * TOKEN_IS_IMMUTABlE. Once deleted update, freeze, unfreeze, grant kyc, revoke kyc and token transfer transactions will
 * resolve to TOKEN_WAS_DELETED.
 */
class TokenDeletePartitionTransaction : public Transaction<TokenDeletePartitionTransaction>
{
public:
  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a CreatePartition transaction.
   */
  explicit TokenDeletePartitionTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Construct from a map of TransactionIds to node account IDs and their respective Transaction protobuf objects.
   *
   * @param transactions The map of TransactionIds to node account IDs and their respective Transaction protobuf
   *                     objects.
   */
  explicit TokenDeletePartitionTransaction(
    const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions);

  /**
   * Set the ID of the partition to delete.
   *
   * @param tokenId The ID of the partition to delete.
   * @return A reference to this TokenDeletePartitionTransaction with the newly-set partition ID.
   */
  TokenDeletePartitionTransaction& setPartitionId(const TokenId& tokenId);

  /**
   * Get the ID of the partition to delete.
   *
   * @return The ID of the partition to delete.
   */
  [[nodiscard]] inline TokenId getPartitionId() const { return mPartitionId; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Submit a Transaction protobuf object which contains this TokenDeletePartitionTransaction's
   * data to a Node.
   *
   * @param request  The Transaction protobuf object to submit.
   * @param node     The Node to which to submit the request.
   * @param deadline The deadline for submitting the request.
   * @param response Pointer to the ProtoResponseType object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const proto::Transaction& request,
                                           const std::shared_ptr<internal::Node>& node,
                                           const std::chrono::system_clock::time_point& deadline,
                                           proto::TransactionResponse* response) const override;

  /**
   * Derived from Transaction. Verify that all the checksums in this TokenDeletePartitionTransaction are valid.
   *
   * @param client The Client that should be used to validate the checksums.
   * @throws BadEntityException This TokenDeletePartitionTransaction's checksums are not valid.
   */
  void validateChecksums(const Client& client) const override;

  /**
   * Derived from Transaction. Build and add the TokenDeletePartitionDefinitionTransactionBody protobuf representation
   * to the Transaction protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Initialize this TokenDeletePartitionTransaction from its source TransactionBody protobuf object.
   */
  void initFromSourceTransactionBody();

  /**
   * Build a TokenDeletePartitionDefinitionTransactionBody protobuf object from this TokenDeletePartitionTransaction
   * object.
   *
   * @return A pointer to a TokenDeletePartitionDefinitionTransactionBody protobuf object filled with this
   *         TokenDeletePartitionTransaction object's data.
   */
  [[nodiscard]] proto::TokenDeletePartitionDefinitionTransactionBody* build() const;

  /**
   * The ID of the partition to delete.
   */
  TokenId mPartitionId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_DELETE_PARTITION_TRANSACTION_H_
