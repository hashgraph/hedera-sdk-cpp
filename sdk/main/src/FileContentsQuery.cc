/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2023 Hedera Hashgraph, LLC
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
#include "FileContentsQuery.h"
#include "impl/Node.h"
#include "impl/Utilities.h"

#include <proto/file_get_contents.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
FileContentsQuery& FileContentsQuery::setFileId(const FileId& fileId)
{
  mFileId = fileId;
  return *this;
}

//-----
FileContents FileContentsQuery::mapResponse(const proto::Response& response) const
{
  return internal::Utilities::stringToByteVector(response.filegetcontents().filecontents().contents());
}

//-----
grpc::Status FileContentsQuery::submitRequest(const proto::Query& request,
                                              const std::shared_ptr<internal::Node>& node,
                                              const std::chrono::system_clock::time_point& deadline,
                                              proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kFileGetContents, request, deadline, response);
}

//-----
proto::Query FileContentsQuery::buildRequest(proto::QueryHeader* header) const
{
  auto fileGetContentsQuery = std::make_unique<proto::FileGetContentsQuery>();
  fileGetContentsQuery->set_allocated_header(header);
  fileGetContentsQuery->set_allocated_fileid(mFileId.toProtobuf().release());

  proto::Query query;
  query.set_allocated_filegetcontents(fileGetContentsQuery.release());
  return query;
}

//-----
proto::ResponseHeader FileContentsQuery::mapResponseHeader(const proto::Response& response) const
{
  return response.filegetcontents().header();
}

} // namespace Hedera
