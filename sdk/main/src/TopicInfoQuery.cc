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
#include "TopicInfoQuery.h"
#include "Client.h"
#include "TopicInfo.h"
#include "TransactionRecord.h"
#include "TransferTransaction.h"
#include "impl/Node.h"

#include <proto/consensus_get_topic_info.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
TopicInfoQuery& TopicInfoQuery::setTopicId(const TopicId& topicId)
{
  mTopicId = topicId;
  return *this;
}

//-----
TopicInfo TopicInfoQuery::mapResponse(const proto::Response& response) const
{
  return TopicInfo::fromProtobuf(response.consensusgettopicinfo());
}

//-----
grpc::Status TopicInfoQuery::submitRequest(const proto::Query& request,
                                           const std::shared_ptr<internal::Node>& node,
                                           const std::chrono::system_clock::time_point& deadline,
                                           proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kConsensusGetTopicInfo, request, deadline, response);
}

//-----
proto::Query TopicInfoQuery::buildRequest(proto::QueryHeader* header) const
{
  auto consensusGetTopicInfoQuery = std::make_unique<proto::ConsensusGetTopicInfoQuery>();
  consensusGetTopicInfoQuery->set_allocated_header(header);
  consensusGetTopicInfoQuery->set_allocated_topicid(mTopicId.toProtobuf().release());

  proto::Query query;
  query.set_allocated_consensusgettopicinfo(consensusGetTopicInfoQuery.release());
  return query;
}

//-----
proto::ResponseHeader TopicInfoQuery::mapResponseHeader(const proto::Response& response) const
{
  Query<TopicInfoQuery, TopicInfo>::saveCostFromHeader(response.consensusgettopicinfo().header());
  return response.consensusgettopicinfo().header();
}

} // namespace Hedera
