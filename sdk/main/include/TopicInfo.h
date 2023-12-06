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
#ifndef HEDERA_SDK_CPP_TOPIC_INFO_H_
#define HEDERA_SDK_CPP_TOPIC_INFO_H_

#include "AccountId.h"
#include "Key.h"
#include "LedgerId.h"
#include "TopicId.h"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace proto
{
class ConsensusGetTopicInfoResponse;
}

namespace Hedera
{
/**
 * Response from a Hedera network when the client sends an TopicInfoQuery.
 */
class TopicInfo
{
public:
  /**
   * Construct a TopicInfo object from a ConsensusGetTopicInfoResponse protobuf object.
   *
   * @param proto The ConsensusGetTopicInfoResponse protobuf object from which to construct a TopicInfo object.
   * @return The constructed TopicInfo object.
   */
  [[nodiscard]] static TopicInfo fromProtobuf(const proto::ConsensusGetTopicInfoResponse& proto);

  /**
   * Construct a TopicInfo object from a byte array.
   *
   * @param bytes The byte array from which to construct a TopicInfo object.
   * @return The constructed TopicInfo object.
   */
  [[nodiscard]] static TopicInfo fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a ConsensusGetTopicInfoResponse protobuf object from this TopicInfo object.
   *
   * @return A pointer to the created ConsensusGetTopicInfoResponse protobuf object.
   */
  [[nodiscard]] std::unique_ptr<proto::ConsensusGetTopicInfoResponse> toProtobuf() const;

  /**
   * Construct a representative byte array from this TopicInfo object.
   *
   * @return The byte array representing this TopicInfo object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Construct a string representation of this TopicInfo object.
   *
   * @return The string representation of this TopicInfo object.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Write this TopicInfo to an output stream.
   *
   * @param os   The output stream.
   * @param info The TopicInfo to print.
   * @return The output stream with this TopicInfo written to it.
   */
  friend std::ostream& operator<<(std::ostream& os, const TopicInfo& info);

  /**
   * The ID of the topic.
   */
  TopicId mTopicId;

  /**
   * The publicly visible memo for the new topic.
   */
  std::string mMemo;

  /**
   * The SHA384 running hash of [previousRunningHash, topicId, consensusTimestamp, sequenceNumber, message].
   */
  std::vector<std::byte> mRunningHash;

  /**
   * The sequence number (which starts at one for the first message) of messages on the topic.
   */
  uint64_t mSequenceNumber = 0ULL;

  /**
   * The timestamp at which the topic will expire.
   */
  std::chrono::system_clock::time_point mExpirationTime = std::chrono::system_clock::now();

  /**
   * The key used for access control to update or delete the topic. Nullptr if there is no admin key for the topic.
   */
  std::shared_ptr<Key> mAdminKey = nullptr;

  /**
   * The key that must sign to submit a message to the new topic (via a TopicMessageSubmitTransaction). Nullptr if there
   * is no submit key for the topic.
   */
  std::shared_ptr<Key> mSubmitKey = nullptr;

  /**
   * The amount of time by which to attempt to extend the topic's lifetime automatically at its expiration time.
   */
  std::optional<std::chrono::system_clock::duration> mAutoRenewPeriod;

  /**
   * The ID of the account that should be charged to extend the lifetime of the new topic at its expiration time.
   */
  std::optional<AccountId> mAutoRenewAccountId;

  /**
   * The ID of the ledger from which this response was returned.
   */
  LedgerId mLedgerId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOPIC_INFO_H_
