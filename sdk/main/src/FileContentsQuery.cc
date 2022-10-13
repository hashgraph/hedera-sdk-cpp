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
#include "FileContentsQuery.h"

#include <proto/file_get_contents.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>
#include <proto/response_header.pb.h>

namespace Hedera
{
//-----
FileContentsQuery::FileContentsQuery()
  : Query()
  , mFileId()
{
}

//-----
void
FileContentsQuery::validateChecksums(const Client& client) const
{
  if (mFileId.isValid())
  {
    mFileId.getValue().validateChecksum(client);
  }
}

//-----
void
FileContentsQuery::onMakeRequest(proto::Query* query,
                                 proto::QueryHeader* header) const
{
  proto::FileGetContentsQuery* getContentsQuery =
    query->mutable_filegetcontents();

  if (mFileId.isValid())
  {
    getContentsQuery->set_allocated_fileid(mFileId.getValue().toProtobuf());
  }

  getContentsQuery->set_allocated_header(header);
}

//-----
proto::ResponseHeader
FileContentsQuery::mapResponseHeader(proto::Response* response) const
{
  return response->filegetcontents().header();
}

//-----
proto::QueryHeader
FileContentsQuery::mapRequestHeader(const proto::Query& query) const
{
  return query.filegetcontents().header();
}

//-----
std::string
FileContentsQuery::mapResponse(const proto::Response& response,
                               const AccountId& accountId,
                               const proto::Query& query) const
{
  return response.filegetcontents().filecontents().contents();
}

//----
FileContentsQuery&
FileContentsQuery::setFileId(const FileId& fileId)
{
  mFileId.setValue(fileId);
  return *this;
}

} // namespace Hedera