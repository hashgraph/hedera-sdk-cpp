/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#include "TopicMessage.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <proto/mirror/consensus_service.pb.h>

#include <utility>

namespace Hedera
{
//-----
TopicMessage::TopicMessage(const std::chrono::system_clock::time_point& lastConsensusTimestamp,
                           const std::vector<std::byte>& message,
                           const std::vector<std::byte>& lastRunningHash,
                           uint64_t lastSequenceNumber,
                           const std::vector<TopicMessageChunk>& chunks,
                           TransactionId transactionId)
  : mConsensusTimestamp(lastConsensusTimestamp)
  , mContents(message)
  , mRunningHash(lastRunningHash)
  , mSequenceNumber(lastSequenceNumber)
  , mChunks(chunks)
  , mTransactionId(std::move(transactionId))
{
}

//-----
TopicMessage TopicMessage::ofSingle(const com::hedera::mirror::api::proto::ConsensusTopicResponse& proto)
{
  return TopicMessage(internal::TimestampConverter::fromProtobuf(proto.consensustimestamp()),
                      internal::Utilities::stringToByteVector(proto.message()),
                      internal::Utilities::stringToByteVector(proto.runninghash()),
                      proto.sequencenumber(),
                      { TopicMessageChunk(proto) },
                      proto.has_chunkinfo() ? TransactionId::fromProtobuf(proto.chunkinfo().initialtransactionid())
                                            : TransactionId());
}

//-----
TopicMessage TopicMessage::ofMany(const std::vector<com::hedera::mirror::api::proto::ConsensusTopicResponse>& protos)
{
  // Place the responses in order from oldest to newest.
  std::vector<TopicMessageChunk> chunks(protos.size(), TopicMessageChunk());
  std::vector<std::vector<std::byte>> contents(protos.size(), std::vector<std::byte>());
  TransactionId transactionId;

  for (const auto& proto : protos)
  {
    if (proto.has_chunkinfo() && proto.chunkinfo().has_initialtransactionid())
    {
      transactionId = TransactionId::fromProtobuf(proto.chunkinfo().initialtransactionid());
    }

    const int32_t index = proto.chunkinfo().number() - 1;
    chunks[index] = TopicMessageChunk(proto);
    contents[index] = internal::Utilities::stringToByteVector(proto.message());
  }

  std::cout << "returning" << std::endl;
  return { chunks.back().mConsensusTimestamp,
           internal::Utilities::concatenateVectors(contents),
           chunks.back().mRunningHash,
           chunks.back().mSequenceNumber,
           chunks,
           transactionId };
}

} // namespace Hedera
