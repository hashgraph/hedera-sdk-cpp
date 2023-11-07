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
#include "TopicInfo.h"
#include "impl/DurationConverter.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <proto/consensus_get_topic_info.pb.h>

namespace Hedera
{
//-----
TopicInfo TopicInfo::fromProtobuf(const proto::ConsensusGetTopicInfoResponse& proto)
{
  TopicInfo topicInfo;

  if (proto.has_topicid())
  {
    topicInfo.mTopicId = TopicId::fromProtobuf(proto.topicid());
  }

  const proto::ConsensusTopicInfo& consensusTopicInfo = proto.topicinfo();

  topicInfo.mMemo = consensusTopicInfo.memo();
  topicInfo.mRunningHash = internal::Utilities::stringToByteVector(consensusTopicInfo.runninghash());
  topicInfo.mSequenceNumber = consensusTopicInfo.sequencenumber();

  if (consensusTopicInfo.has_expirationtime())
  {
    topicInfo.mExpirationTime = internal::TimestampConverter::fromProtobuf(consensusTopicInfo.expirationtime());
  }

  if (consensusTopicInfo.has_adminkey())
  {
    topicInfo.mAdminKey = Key::fromProtobuf(consensusTopicInfo.adminkey());
  }

  if (consensusTopicInfo.has_submitkey())
  {
    topicInfo.mSubmitKey = Key::fromProtobuf(consensusTopicInfo.submitkey());
  }

  if (consensusTopicInfo.has_autorenewperiod())
  {
    topicInfo.mAutoRenewPeriod = internal::DurationConverter::fromProtobuf(consensusTopicInfo.autorenewperiod());
  }

  if (consensusTopicInfo.has_autorenewaccount())
  {
    topicInfo.mAutoRenewAccountId = AccountId::fromProtobuf(consensusTopicInfo.autorenewaccount());
  }

  topicInfo.mLedgerId = LedgerId(internal::Utilities::stringToByteVector(consensusTopicInfo.ledger_id()));

  return topicInfo;
}

//-----
TopicInfo TopicInfo::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::ConsensusGetTopicInfoResponse proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
std::unique_ptr<proto::ConsensusGetTopicInfoResponse> TopicInfo::toProtobuf() const
{
  auto protoTopicInfo = std::make_unique<proto::ConsensusGetTopicInfoResponse>();
  protoTopicInfo->set_allocated_topicid(mTopicId.toProtobuf().release());
  protoTopicInfo->mutable_topicinfo()->set_memo(mMemo);
  protoTopicInfo->mutable_topicinfo()->set_runninghash(internal::Utilities::byteVectorToString(mRunningHash));
  protoTopicInfo->mutable_topicinfo()->set_sequencenumber(mSequenceNumber);
  protoTopicInfo->mutable_topicinfo()->set_allocated_expirationtime(
    internal::TimestampConverter::toProtobuf(mExpirationTime));

  if (mAdminKey)
  {
    protoTopicInfo->mutable_topicinfo()->set_allocated_adminkey(mAdminKey->toProtobufKey().release());
  }

  if (mSubmitKey)
  {
    protoTopicInfo->mutable_topicinfo()->set_allocated_submitkey(mSubmitKey->toProtobufKey().release());
  }

  if (mAutoRenewPeriod.has_value())
  {
    protoTopicInfo->mutable_topicinfo()->set_allocated_autorenewperiod(
      internal::DurationConverter::toProtobuf(mAutoRenewPeriod.value()));
  }

  if (mAutoRenewAccountId.has_value())
  {
    protoTopicInfo->mutable_topicinfo()->set_allocated_autorenewaccount(mAutoRenewAccountId->toProtobuf().release());
  }

  protoTopicInfo->mutable_topicinfo()->set_ledger_id(internal::Utilities::byteVectorToString(mLedgerId.toBytes()));

  return protoTopicInfo;
}

//-----
std::vector<std::byte> TopicInfo::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

} // namespace Hedera
