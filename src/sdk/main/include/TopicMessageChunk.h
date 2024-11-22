/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#ifndef HIERO_SDK_CPP_TOPIC_MESSAGE_CHUNK_H_
#define HIERO_SDK_CPP_TOPIC_MESSAGE_CHUNK_H_

#include "TransactionId.h"

#include <chrono>
#include <cstddef>
#include <vector>

namespace com::hedera::mirror::api::proto
{
class ConsensusTopicResponse;
}

namespace Hiero
{
/**
 * A chunk of a topic message.
 */
class TopicMessageChunk
{
public:
  TopicMessageChunk() = default;

  /**
   * Construct from a ConsensusTopicResponse protobuf object.
   *
   * @param proto The ConsensusTopicResponse protobuf object.
   */
  explicit TopicMessageChunk(const com::hedera::mirror::api::proto::ConsensusTopicResponse& proto);

  /**
   * The consensus timestamp of this topic message chunk.
   */
  std::chrono::system_clock::time_point mConsensusTimestamp;

  /**
   * The size of this chunk's content.
   */
  uint64_t mContentSize = 0ULL;

  /**
   * The running hash of this chunk.
   */
  std::vector<std::byte> mRunningHash;

  /**
   * The sequence number of this chunk.
   */
  uint64_t mSequenceNumber = 0ULL;
};

} // namespace Hiero

#endif // HIERO_SDK_CPP_TOPIC_MESSAGE_CHUNK_H_
