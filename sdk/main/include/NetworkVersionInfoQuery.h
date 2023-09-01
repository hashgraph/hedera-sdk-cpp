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
#ifndef HEDERA_SDK_CPP_NETWORK_VERSION_INFO_QUERY_H_
#define HEDERA_SDK_CPP_NETWORK_VERSION_INFO_QUERY_H_

#include "Query.h"

namespace Hedera
{
class NetworkVersionInfo;
}

namespace Hedera
{
/**
 * Get the deployed versions of Hedera Services and the HAPI proto in semantic version format.
 */
class NetworkVersionInfoQuery : public Query<NetworkVersionInfoQuery, NetworkVersionInfo>
{
private:
  /**
   * Derived from Executable. Construct a Query protobuf object from this NetworkVersionInfoQuery object.
   *
   * @param client The Client trying to construct this NetworkVersionInfoQuery.
   * @param node   The Node to which this NetworkVersionInfoQuery will be sent.
   * @return A Query protobuf object filled with this NetworkVersionInfoQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct a NetworkVersionInfo object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct a NetworkVersionInfo object.
   * @return A NetworkVersionInfo object filled with the Response protobuf object's data.
   */
  [[nodiscard]] NetworkVersionInfo mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the status response code for a submitted NetworkVersionInfoQuery from a Response
   * protobuf object.
   *
   * @param response The Response protobuf object from which to grab the NetworkVersionInfoQuery status response code.
   * @return The NetworkVersionInfoQuery status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit this NetworkVersionInfoQuery to a Node.
   *
   * @param client   The Client submitting this NetworkVersionInfoQuery.
   * @param deadline The deadline for submitting this NetworkVersionInfoQuery.
   * @param node     Pointer to the Node to which this NetworkVersionInfoQuery should be submitted.
   * @param response Pointer to the Response protobuf object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::Response* response) const override;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_NETWORK_VERSION_INFO_QUERY_H_
