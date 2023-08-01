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
#ifndef HEDERA_SDK_CPP_TOPIC_INFO_QUERY_H_
#define HEDERA_SDK_CPP_TOPIC_INFO_QUERY_H_

#include "Query.h"
#include "TopicId.h"

namespace Hedera
{
class TopicInfo;
class TransactionRecord;
}

namespace Hedera
{
/**
 * A query that gets information about a topic instance.
 */
class TopicInfoQuery : public Query<TopicInfoQuery, TopicInfo>
{
public:
  /**
   * Set the ID of the topic of which to request the info.
   *
   * @param topic The ID of the topic of which to request the info.
   * @return A reference to this TopicInfoQuery object with the newly-set topic ID.
   */
  TopicInfoQuery& setTopicId(const TopicId& topicId);

  /**
   * Get the ID of the topic of which this query is currently configured to get the info.
   *
   * @return The ID of the topic for which this query is meant.
   */
  [[nodiscard]] inline TopicId getTopicId() const { return mTopicId; }

private:
  /**
   * Derived from Executable. Construct a Query protobuf object from this TopicInfoQuery object.
   *
   * @param client The Client trying to construct this TopicInfoQuery.
   * @param node   The Node to which this TopicInfoQuery will be sent.
   * @return A Query protobuf object filled with this TopicInfoQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct a TopicInfo object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct a TopicInfo object.
   * @return A TopicInfo object filled with the Response protobuf object's data.
   */
  [[nodiscard]] TopicInfo mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the status response code for a submitted TopicInfoQuery from a Response protobuf
   * object.
   *
   * @param response The Response protobuf object from which to grab the TopicInfoQuery status response code.
   * @return The TopicInfoQuery status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit this TopicInfoQuery to a Node.
   *
   * @param client   The Client submitting this TopicInfoQuery.
   * @param deadline The deadline for submitting this TopicInfoQuery.
   * @param node     Pointer to the Node to which this TopicInfoQuery should be submitted.
   * @param response Pointer to the Response protobuf object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::Response* response) const override;

  /**
   * The ID of the topic of which this query should get the info.
   */
  TopicId mTopicId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOPIC_INFO_QUERY_H_
