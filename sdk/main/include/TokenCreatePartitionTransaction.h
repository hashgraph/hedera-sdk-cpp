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
#ifndef HEDERA_SDK_CPP_TOKEN_CREATE_PARTITION_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOKEN_CREATE_PARTITION_TRANSACTION_H_

#include "TokenId.h"
#include "Transaction.h"

#include <string>

namespace proto
{
class TokenCreatePartitionDefinitionTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Create a new partition type on a token. After the entity is created, the TokenID for it is in the receipt.
 */
class TokenCreatePartitionTransaction : public Transaction<TokenCreatePartitionTransaction>
{
public:
  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a CreatePartition transaction.
   */
  explicit TokenCreatePartitionTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Construct from a map of TransactionIds to node account IDs and their respective Transaction protobuf objects.
   *
   * @param transactions The map of TransactionIds to node account IDs and their respective Transaction protobuf
   *                     objects.
   */
  explicit TokenCreatePartitionTransaction(
    const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions);

  /**
   * Set the ID of the parent token from which to create the partition.
   *
   * @param tokenId The ID of the parent token from which to create the partition.
   * @return A reference to this TokenCreatePartitionTransaction with the newly-set parent token ID.
   */
  TokenCreatePartitionTransaction& setParentTokenId(const TokenId& tokenId);

  /**
   * Set the name of the partition.
   *
   * @param name The name of the partition.
   * @return A reference to this TokenCreatePartitionTransaction with the newly-set name.
   */
  TokenCreatePartitionTransaction& setPartitionName(std::string_view name);

  /**
   * Set the memo of the partition.
   *
   * @param memo The memo of the partition.
   * @return A reference to this TokenCreatePartitionTransaction with the newly-set memo.
   */
  TokenCreatePartitionTransaction& setPartitionMemo(std::string_view memo);

  /**
   * Get the ID of the parent token.
   *
   * @return The ID of the parent token.
   */
  [[nodiscard]] inline TokenId getParentTokenId() const { return mParentTokenId; }

  /**
   * Get the name of the partition.
   *
   * @return The name of the partition.
   */
  [[nodiscard]] inline std::string getPartitionName() const { return mPartitionName; }

  /**
   * Get the memo of the partition.
   *
   * @return The memo of the partition.
   */
  [[nodiscard]] inline std::string getPartitionMemo() const { return mPartitionMemo; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Submit a Transaction protobuf object which contains this TokenCreatePartitionTransaction's
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
   * Derived from Transaction. Verify that all the checksums in this TokenCreatePartitionTransaction are valid.
   *
   * @param client The Client that should be used to validate the checksums.
   * @throws BadEntityException This TokenCreatePartitionTransaction's checksums are not valid.
   */
  void validateChecksums(const Client& client) const override;

  /**
   * Derived from Transaction. Build and add the TokenCreatePartitionTransaction protobuf representation to the
   * Transaction protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Initialize this TokenCreatePartitionTransaction from its source TransactionBody protobuf object.
   */
  void initFromSourceTransactionBody();

  /**
   * Build a TokenCreatePartitionDefinitionTransactionBody protobuf object from this TokenCreatePartitionTransaction
   * object.
   *
   * @return A pointer to a TokenCreatePartitionDefinitionTransactionBody protobuf object filled with this
   *         TokenCreatePartitionTransaction object's data.
   */
  [[nodiscard]] proto::TokenCreatePartitionDefinitionTransactionBody* build() const;

  /**
   * The ID of the parent token from which to create the partition.
   */
  TokenId mParentTokenId;

  /**
   * The name of the partition.
   */
  std::string mPartitionName;

  /**
   * The memo of the partition.
   */
  std::string mPartitionMemo;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_CREATE_PARTITION_TRANSACTION_H_
