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
#ifndef HEDERA_SDK_CPP_SCHEDULE_INFO_QUERY_H_
#define HEDERA_SDK_CPP_SCHEDULE_INFO_QUERY_H_

#include "Query.h"
#include "ScheduleId.h"

namespace Hedera
{
class ScheduleInfo;
class TransactionRecord;
}

namespace Hedera
{
/**
 * A query that returns information about the current state of a schedule transaction on a Hedera network.
 */
class ScheduleInfoQuery : public Query<ScheduleInfoQuery, ScheduleInfo>
{
public:
  /**
   * Set the ID of the schedule of which to request the info.
   *
   * @param schedule The ID of the schedule of which to request the info.
   * @return A reference to this ScheduleInfoQuery object with the newly-set schedule ID.
   */
  ScheduleInfoQuery& setScheduleId(const ScheduleId& scheduleId);

  /**
   * Get the ID of the schedule of which this query is currently configured to get the info.
   *
   * @return The ID of the schedule for which this query is meant.
   */
  [[nodiscard]] inline ScheduleId getScheduleId() const { return mScheduleId; }

private:
  /**
   * Derived from Executable. Construct a Query protobuf object from this ScheduleInfoQuery object.
   *
   * @param client The Client trying to construct this ScheduleInfoQuery.
   * @param node   The Node to which this ScheduleInfoQuery will be sent.
   * @return A Query protobuf object filled with this ScheduleInfoQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct a ScheduleInfo object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct a ScheduleInfo object.
   * @return A ScheduleInfo object filled with the Response protobuf object's data.
   */
  [[nodiscard]] ScheduleInfo mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the status response code for a submitted ScheduleInfoQuery from a Response protobuf
   * object.
   *
   * @param response The Response protobuf object from which to grab the ScheduleInfoQuery status response code.
   * @return The ScheduleInfoQuery status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit this ScheduleInfoQuery to a Node.
   *
   * @param client   The Client submitting this ScheduleInfoQuery.
   * @param deadline The deadline for submitting this ScheduleInfoQuery.
   * @param node     Pointer to the Node to which this ScheduleInfoQuery should be submitted.
   * @param response Pointer to the Response protobuf object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::Response* response) const override;

  /**
   * The ID of the schedule of which this query should get the info.
   */
  ScheduleId mScheduleId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_SCHEDULE_INFO_QUERY_H_
