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
#ifndef FILE_CONTENTS_QUERY_H_
#define FILE_CONTENTS_QUERY_H_

#include "FileId.h"
#include "Query.h"

#include "helper/InitType.h"

#include <string>

namespace proto
{
class Query;
class QueryHeader;
class Response;
class ResponseHeader;
}

namespace Hedera
{
class Client;
}

namespace Hedera
{
/**
 * Get the contents of a file. The content field is empty (no bytes) if the file
 * is empty.
 */
class FileContentsQuery : public Query<std::string, FileContentsQuery>
{
public:
  /**
   * Constructor
   */
  FileContentsQuery();

  /**
   * Derived from Query. Validate the checksums of the account ID.
   *
   * @param client The client with which to validate the checksums
   */
  virtual void validateChecksums(const Client& client) const override;

  /**
   * Derived from Query. Fills query with this class's data and attaches the
   * header.
   *
   * @param query  The query object to fill out.
   * @param header The header for the query.
   */
  virtual void onMakeRequest(proto::Query* query,
                             proto::QueryHeader* header) const override;

  /**
   * Derived from Query. Get the file contents header from the response.
   *
   * @param response The associated response to this query.
   * @return The response header for the file content's query.
   */
  virtual proto::ResponseHeader mapResponseHeader(
    proto::Response* response) const override;

  /**
   * Derived from Query. Grab the file contents query header.
   *
   * @param query The query of which to extract the header.
   * @return The file contents query header.
   */
  virtual proto::QueryHeader mapRequestHeader(
    const proto::Query& query) const override;

  /**
   * Derived from Query. Extract the file contents from the response object.
   *
   * @param response  The received response from Hedera.
   * @param accountId The account ID that made the request.
   * @param query     The original query.
   * @return The file contents.
   */
  virtual std::string mapResponse(const proto::Response& response,
                                  const AccountId& accountId,
                                  const proto::Query& query) const override;

  /**
   * Sets the ID of the file whose contents are requested.
   *
   * @param fileId The file ID to be set.
   * @return Reference to this FileContentsQuery object.
   */
  FileContentsQuery& setFileId(const FileId& fileId);

  /**
   * Extract the file ID.
   *
   * @return The file ID.
   */
  inline InitType<FileId> getFileId() const { return mFileId; }

private:
  /**
   * The ID of the file whose contents are requested.
   */
  InitType<FileId> mFileId;
};

} // namespace Hedera

#endif // FILE_CONTENTS_QUERY_H_