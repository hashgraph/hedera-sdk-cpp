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
#include "ScheduleInfoQuery.h"
#include "ScheduleInfo.h"
#include "impl/Node.h"

#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>
#include <proto/schedule_get_info.pb.h>

namespace Hedera
{
//-----
ScheduleInfoQuery& ScheduleInfoQuery::setScheduleId(const ScheduleId& scheduleId)
{
  mScheduleId = scheduleId;
  return *this;
}

//-----
ScheduleInfo ScheduleInfoQuery::mapResponse(const proto::Response& response) const
{
  return ScheduleInfo::fromProtobuf(response.schedulegetinfo().scheduleinfo());
}

//-----
grpc::Status ScheduleInfoQuery::submitRequest(const proto::Query& request,
                                              const std::shared_ptr<internal::Node>& node,
                                              const std::chrono::system_clock::time_point& deadline,
                                              proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kScheduleGetInfo, request, deadline, response);
}

//-----
void ScheduleInfoQuery::validateChecksums(const Client& client) const
{
  mScheduleId.validateChecksum(client);
}

//-----
proto::Query ScheduleInfoQuery::buildRequest(proto::QueryHeader* header) const
{
  auto scheduleGetInfoQuery = std::make_unique<proto::ScheduleGetInfoQuery>();
  scheduleGetInfoQuery->set_allocated_header(header);
  scheduleGetInfoQuery->set_allocated_scheduleid(mScheduleId.toProtobuf().release());

  proto::Query query;
  query.set_allocated_schedulegetinfo(scheduleGetInfoQuery.release());
  return query;
}

//-----
proto::ResponseHeader ScheduleInfoQuery::mapResponseHeader(const proto::Response& response) const
{
  saveCostFromHeader(response.schedulegetinfo().header());
  return response.schedulegetinfo().header();
}

} // namespace Hedera
