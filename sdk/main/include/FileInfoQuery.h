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
#ifndef HEDERA_SDK_CPP_FILE_INFO_QUERY_H_
#define HEDERA_SDK_CPP_FILE_INFO_QUERY_H_

#include "FileId.h"
#include "Query.h"

namespace Hedera
{
class FileInfo;
class TransactionRecord;
}

namespace Hedera
{
/**
 * A query that returns the current state of a file. Queries do not change the state of the file or require network
 * consensus. The information is returned from a single node processing the query.
 */
class FileInfoQuery : public Query<FileInfoQuery, FileInfo>
{
public:
  /**
   * Set the ID of the file of which to request the info.
   *
   * @param fileId The ID of the desired file of which to request the info.
   * @return A reference to this FileInfoQuery object with the newly-set file ID.
   */
  FileInfoQuery& setFileId(const FileId& fileId);

  /**
   * Get the ID of the file of which this query is currently configured to get the info.
   *
   * @return The ID of the file for which this query is meant.
   */
  [[nodiscard]] inline FileId getfileId() const { return mFileId; }

private:
  /**
   * Derived from Executable. Construct a Query protobuf object from this FileInfoQuery object.
   *
   * @param client The Client trying to construct this FileInfoQuery.
   * @param node   The Node to which this FileInfoQuery will be sent.
   * @return A Query protobuf object filled with this FileInfoQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct a FileInfo object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct a FileInfo object.
   * @return A FileInfo object filled with the Response protobuf object's data.
   */
  [[nodiscard]] FileInfo mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the status response code for a submitted FileInfoQuery from a Response protobuf
   * object.
   *
   * @param response The Response protobuf object from which to grab the FileInfoQuery status response code.
   * @return The FileInfoQuery status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit this FileInfoQuery to a Node.
   *
   * @param client   The Client submitting this FileInfoQuery.
   * @param deadline The deadline for submitting this FileInfoQuery.
   * @param node     Pointer to the Node to which this FileInfoQuery should be submitted.
   * @param response Pointer to the Response protobuf object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::Response* response) const override;

  /**
   * The ID of the file of which this query should get the info.
   */
  FileId mFileId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_FILE_INFO_QUERY_H_
