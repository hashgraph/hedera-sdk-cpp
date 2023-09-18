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
#include "FileInfoQuery.h"
#include "FileInfo.h"
#include "impl/Node.h"

#include <proto/file_get_info.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
FileInfoQuery& FileInfoQuery::setFileId(const FileId& fileId)
{
  mFileId = fileId;
  return *this;
}

//-----
FileInfo FileInfoQuery::mapResponse(const proto::Response& response) const
{
  return FileInfo::fromProtobuf(response.filegetinfo().fileinfo());
}

//-----
grpc::Status FileInfoQuery::submitRequest(const proto::Query& request,
                                          const std::shared_ptr<internal::Node>& node,
                                          const std::chrono::system_clock::time_point& deadline,
                                          proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kFileGetInfo, request, deadline, response);
}

//-----
proto::Query FileInfoQuery::buildRequest(proto::QueryHeader* header) const
{
  auto fileGetInfoQuery = std::make_unique<proto::FileGetInfoQuery>();
  fileGetInfoQuery->set_allocated_header(header);
  fileGetInfoQuery->set_allocated_fileid(mFileId.toProtobuf().release());

  proto::Query query;
  query.set_allocated_filegetinfo(fileGetInfoQuery.release());
  return query;
}

//-----
proto::ResponseHeader FileInfoQuery::mapResponseHeader(const proto::Response& response) const
{
  saveCostFromHeader(response.filegetinfo().header());
  return response.filegetinfo().header();
}

} // namespace Hedera
