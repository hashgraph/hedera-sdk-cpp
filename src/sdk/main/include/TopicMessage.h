/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#ifndef HEDERA_SDK_CPP_TOPIC_MESSAGE_H_
#define HEDERA_SDK_CPP_TOPIC_MESSAGE_H_

#include "TopicMessageChunk.h"
#include "TransactionId.h"

#include <chrono>
#include <cstddef>
#include <vector>

namespace com::hedera::mirror::api::proto
{
class ConsensusTopicResponse;
}

namespace Hedera
{
/**
 * The message of a topic.
 */
class TopicMessage
{
public:
  TopicMessage() = default;

  /**
   * Construct with the consensus timestamp of the last message received, the topic message, the last running hash, the
   * last sequence number, the chunks that make up the message, and the transaction ID.
   *
   * @param lastConsensusTimestamp The consensus timestamp of the last message received.
   * @param message                The topic message.
   * @param lastRunningHash        The last running hash of the topic.
   * @param lastSequenceNumber     The last sequence number of the topic.
   * @param chunks                 The chunks that make up the message.
   * @param transactionId          The ID of the transaction.
   */
  TopicMessage(const std::chrono::system_clock::time_point& lastConsensusTimestamp,
               const std::vector<std::byte>& message,
               const std::vector<std::byte>& lastRunningHash,
               uint64_t lastSequenceNumber,
               const std::vector<TopicMessageChunk>& chunks,
               TransactionId transactionId);

  /**
   * Create a TopicMessage from a single ConsensusTopicResponse protobuf object.
   *
   * @param proto The ConsensusTopicResponse protobuf object from which to create a TopicMessage.
   * @return The constructed TopicMessage object.
   */
  [[nodiscard]] static TopicMessage ofSingle(const com::hedera::mirror::api::proto::ConsensusTopicResponse& proto);

  /**
   * Create a TopicMessage from numerous ConsensusTopicResponse protobuf objects.
   *
   * @param protos The ConsensusTopicResponse protobuf objects from which to create a TopicMessage.
   * @return The constructed TopicMessage object.
   */
  [[nodiscard]] static TopicMessage ofMany(
    const std::vector<com::hedera::mirror::api::proto::ConsensusTopicResponse>& protos);

  /**
   * The consensus timestamp of the full TopicMessage.
   */
  std::chrono::system_clock::time_point mConsensusTimestamp;

  /**
   * The content of this TopicMessage.
   */
  std::vector<std::byte> mContents;

  /**
   * The running hash of the topic that received the message.
   */
  std::vector<std::byte> mRunningHash;

  /**
   * The sequence number of this TopicMessage.
   */
  uint64_t mSequenceNumber = 0ULL;

  /**
   * This TopicMessage's chunks, if it was divided into chunks.
   */
  std::vector<TopicMessageChunk> mChunks;

  /**
   * The ID of the corresponding transaction.
   */
  TransactionId mTransactionId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOPIC_MESSAGE_H_
