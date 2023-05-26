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
#ifndef HEDERA_SDK_CPP_FILE_CONTENTS_QUERY_H_
#define HEDERA_SDK_CPP_FILE_CONTENTS_QUERY_H_

#include "FileId.h"
#include "Query.h"

#include <cstddef>
#include <optional>
#include <vector>

namespace Hedera
{
/**
 * A query to get the contents of a file. Queries do not change the state of the file or require network consensus. The
 * information is returned from a single node processing the query.
 *
 * Query Signing Requirements:
 *  - The client operator private key is required to sign the query request.
 */
using FileContents = std::vector<std::byte>;
class FileContentsQuery : public Query<FileContentsQuery, FileContents>
{
public:
  /**
   * Set the ID of the file of which to request the contents.
   *
   * @param fileId The ID of the file of which to request the contents.
   * @return A reference to this FileContentsQuery object with the newly-set file ID.
   */
  FileContentsQuery& setFileId(const FileId& fileId);

  /**
   * Get the ID of the file of which this query is currently configured to get the contents.
   *
   * @return The ID of the file for which this query is meant.
   */
  [[nodiscard]] inline FileId getFileId() const { return mFileId; }

private:
  /**
   * Derived from Executable. Construct a Query protobuf object from this FileContentsQuery object.
   *
   * @param client The Client trying to construct this FileContentsQuery.
   * @param node   The Node to which this FileContentsQuery will be sent.
   * @return A Query protobuf object filled with this FileContentsQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct a FileContents object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct a FileContents object.
   * @return A FileContents object filled with the Response protobuf object's data.
   */
  [[nodiscard]] FileContents mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the status response code for a submitted FileContentsQuery from a Response
   * protobuf object.
   *
   * @param response The Response protobuf object from which to grab the FileContentsQuery status response code.
   * @return The FileContentsQuery status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit this FileContentsQuery to a Node.
   *
   * @param client   The Client submitting this FileContentsQuery.
   * @param deadline The deadline for submitting this FileContentsQuery.
   * @param node     Pointer to the Node to which this FileContentsQuery should be submitted.
   * @param response Pointer to the Response protobuf object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::Response* response) const override;

  /**
   * The ID of the file of which this query should get the contents.
   */
  FileId mFileId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_FILE_CONTENTS_QUERY_H_
