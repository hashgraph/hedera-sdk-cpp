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
#ifndef HEDERA_SDK_CPP_TRANSACTION_RECEIPT_H_
#define HEDERA_SDK_CPP_TRANSACTION_RECEIPT_H_

#include "AccountId.h"
#include "ContractId.h"
#include "ExchangeRates.h"
#include "FileId.h"
#include "ScheduleId.h"
#include "Status.h"
#include "TokenId.h"
#include "TopicId.h"
#include "TransactionId.h"

#include <cstddef>
#include <memory>
#include <optional>
#include <vector>

namespace proto
{
class TransactionGetReceiptResponse;
class TransactionReceipt;
}

namespace Hedera
{
/**
 * The summary of a transaction's result so far. If the transaction has not reached consensus, this result will be
 * necessarily incomplete.
 */
class TransactionReceipt
{
public:
  /**
   * Construct a TransactionReceipt object from a TransactionGetReceiptResponse protobuf object.
   *
   * @param proto         The TransactionGetReceiptResponse protobuf object from which to construct a TransactionReceipt
   *                      object.
   * @param transactionId The ID of the transaction to which the constructed TransactionReceipt will correspond.
   * @return The constructed TransactionReceipt object.
   */
  [[nodiscard]] static TransactionReceipt fromProtobuf(const proto::TransactionGetReceiptResponse& proto,
                                                       const TransactionId& transactionId);

  /**
   * Construct a TransactionReceipt object from a TransactionReceipt protobuf object.
   *
   * @param proto         The TransactionReceipt protobuf object from which to construct a TransactionReceipt object.
   * @param transactionId The ID of the transaction to which the constructed TransactionReceipt will correspond.
   * @return The constructed TransactionReceipt object.
   */
  [[nodiscard]] static TransactionReceipt fromProtobuf(const proto::TransactionReceipt& proto,
                                                       const TransactionId& transactionId = TransactionId());

  /**
   * Construct a TransactionReceipt object from a byte array.
   *
   * @param bytes The byte array from which to construct a TransactionReceipt object.
   * @return The constructed TransactionReceipt object.
   */
  [[nodiscard]] static TransactionReceipt fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a TransactionReceipt protobuf object from this TransactionReceipt object.
   *
   * @return A pointer to the created TransactionReceipt protobuf object.
   */
  [[nodiscard]] std::unique_ptr<proto::TransactionReceipt> toProtobuf() const;

  /**
   * Construct a representative byte array from this TransactionReceipt object.
   *
   * @return The byte array representing this TransactionReceipt object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Validate the status and throw if it is not a Status::SUCCESS.
   *
   * @throws ReceiptStatusException If the status is not a Status::SUCCESS.
   */
  void validateStatus() const;

  /**
   * The ID of the transaction to which this TransactionReceipt corresponds.
   */
  TransactionId mTransactionId;

  /**
   * The consensus status of the transaction; is UNKNOWN if consensus has not been reached, or if the associated
   * transaction did not have a valid payer signature.
   */
  Status mStatus = Status::UNKNOWN;

  /**
   * In the receipt of a CryptoCreate, the ID of the newly created account.
   */
  std::optional<AccountId> mAccountId;

  /**
   * In the receipt of a FileCreate, the ID of the newly created file.
   */
  std::optional<FileId> mFileId;

  /**
   * In the receipt of a ContractCreate, the ID of the newly created contract.
   */
  std::optional<ContractId> mContractId;

  /**
   * The exchange rates in effect when the transaction reached consensus.
   */
  ExchangeRates mExchangeRates;

  /**
   * In the receipt of a ConsensusCreateTopic, the ID of the newly created topic.
   */
  std::optional<TopicId> mTopicId;

  /**
   * In the receipt of a ConsensusSubmitMessage, the new sequence number of the topic that received the message.
   */
  std::optional<uint64_t> mTopicSequenceNumber;

  /**
   * In the receipt of a ConsensusSubmitMessage, the new running hash of the topic that received the message.  This
   * 48-byte field is the output of a particular SHA-384 digest whose input data are determined by the value of the
   * mTopicRunningHashVersion below. The bytes of each uint64_t or uint32_t are to be in Big-Endian format.
   *
   * IF the mTopicRunningHashVersion is '0' or '1', then the input data to the SHA-384 digest are, in order:
   * ---
   * 1. The previous running hash of the topic (48 bytes)
   * 2. The topic's shard (8 bytes)
   * 3. The topic's realm (8 bytes)
   * 4. The topic's number (8 bytes)
   * 5. The number of seconds since the epoch before the ConsensusSubmitMessage reached
   *    consensus (8 bytes)
   * 6. The number of nanoseconds since 5. before the ConsensusSubmitMessage reached
   *    consensus (4 bytes)
   * 7. The topicSequenceNumber from above (8 bytes)
   * 8. The message bytes from the ConsensusSubmitMessage (variable).
   *
   * IF the mTopicRunningHashVersion is '2', then the input data to the SHA-384 digest are, in order:
   * ---
   * 1. The previous running hash of the topic (48 bytes)
   * 2. The topicRunningHashVersion below (8 bytes)
   * 3. The topic's shard (8 bytes)
   * 4. The topic's realm (8 bytes)
   * 5. The topic's number (8 bytes)
   * 6. The number of seconds since the epoch before the ConsensusSubmitMessage reached
   *    consensus (8 bytes)
   * 7. The number of nanoseconds since 6. before the ConsensusSubmitMessage reached
   *    consensus (4 bytes)
   * 8. The topicSequenceNumber from above (8 bytes)
   * 9. The output of the SHA-384 digest of the message bytes from the
   *    consensusSubmitMessage (48 bytes)
   *
   * Otherwise, IF the mTopicRunningHashVersion is '3', then the input data to the SHA-384 digest are, in order:
   * ---
   * 1.  The previous running hash of the topic (48 bytes)
   * 2.  The topicRunningHashVersion below (8 bytes)
   * 3.  The payer account's shard (8 bytes)
   * 4.  The payer account's realm (8 bytes)
   * 5.  The payer account's number (8 bytes)
   * 6.  The topic's shard (8 bytes)
   * 7.  The topic's realm (8 bytes)
   * 8.  The topic's number (8 bytes)
   * 9.  The number of seconds since the epoch before the ConsensusSubmitMessage reached consensus (8 bytes)
   * 10. The number of nanoseconds since 9. before the ConsensusSubmitMessage reached consensus (4 bytes)
   * 11. The topicSequenceNumber from above (8 bytes)
   * 12. The output of the SHA-384 digest of the message bytes from the consensusSubmitMessage (48 bytes)
   */
  std::optional<std::vector<std::byte>> mTopicRunningHash;

  /**
   * In the receipt of a ConsensusSubmitMessage, the version of the SHA-384 digest used to update the running hash.
   */
  std::optional<uint64_t> mTopicRunningHashVersion;

  /**
   * In the receipt of a TokenCreate, the ID of the newly created token.
   */
  std::optional<TokenId> mTokenId;

  /**
   * In the receipt of a TokenMint, TokenWipe, TokenBurn. For FUNGIBLE_COMMON tokens, this is the current total supply
   * of the token. For NON_FUNGIBLE_UNIQUE tokens, this is the total number of NFTs issued for a given token ID.
   */
  std::optional<uint64_t> mNewTotalSupply;

  /**
   * In the receipt of a ScheduleCreate, the ID of the newly created scheduled entity.
   */
  std::optional<ScheduleId> mScheduleId;

  /**
   * In the receipt of a ScheduleCreate or ScheduleSign that resolves to Status::SUCCESS, the TransactionId that should
   * be used to query for the receipt or record of the relevant schedule transaction.
   */
  std::optional<TransactionId> mScheduledTransactionId;

  /**
   * In the receipt of a TokenMint, for tokens of type NON_FUNGIBLE_COMMON, these are the serial numbers of the
   * newly-created NFTs.
   */
  std::vector<uint64_t> mSerialNumbers;

  /**
   * The receipts of processing all transactions with the given ID, in consensus time order.
   */
  std::vector<TransactionReceipt> mDuplicates;

  /**
   * The receipts (if any) of all child transactions spawned by the transaction with the given top-level id, in
   * consensus order. Always empty if the top-level status is UNKNOWN.
   */
  std::vector<TransactionReceipt> mChildren;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_RECEIPT_H_
