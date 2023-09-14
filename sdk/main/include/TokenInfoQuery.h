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
#ifndef HEDERA_SDK_CPP_TOKEN_INFO_QUERY_H_
#define HEDERA_SDK_CPP_TOKEN_INFO_QUERY_H_

#include "Query.h"
#include "TokenId.h"

namespace Hedera
{
class TokenInfo;
class TransactionRecord;
}

namespace Hedera
{
/**
 * A query that gets information about a fungible or non-fungible token instance.
 */
class TokenInfoQuery : public Query<TokenInfoQuery, TokenInfo>
{
public:
  /**
   * Set the ID of the token of which to request the info.
   *
   * @param token The ID of the token of which to request the info.
   * @return A reference to this TokenInfoQuery object with the newly-set token ID.
   */
  TokenInfoQuery& setTokenId(const TokenId& tokenId);

  /**
   * Get the ID of the token of which this query is currently configured to get the info.
   *
   * @return The ID of the token for which this query is meant.
   */
  [[nodiscard]] inline TokenId getTokenId() const { return mTokenId; }

private:
  /**
   * Derived from Executable. Construct a TokenInfo object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct a TokenInfo object.
   * @return A TokenInfo object filled with the Response protobuf object's data
   */
  [[nodiscard]] TokenInfo mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit a Query protobuf object which contains this TokenInfoQuery's data to a Node.
   *
   * @param request  The Query protobuf object to submit.
   * @param node     The Node to which to submit the request.
   * @param deadline The deadline for submitting the request.
   * @param response Pointer to the ProtoResponseType object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const proto::Query& request,
                                           const std::shared_ptr<internal::Node>& node,
                                           const std::chrono::system_clock::time_point& deadline,
                                           proto::Response* response) const override;
  /**
   * Derived from Query. Build a Query protobuf object with this TokenInfoQuery's data, with the input QueryHeader
   * protobuf object.
   *
   * @param header A pointer to the QueryHeader protobuf object to add to the Query protobuf object.
   * @return The constructed Query protobuf object.
   */
  [[nodiscard]] proto::Query buildRequest(proto::QueryHeader* header) const override;

  /**
   * Derived from Query. Get the ResponseHeader protobuf object from the input Response protobuf object.
   *
   * @param response The Response protobuf object from which to get the ResponseHeader protobuf object.
   * @return The ResponseHeader protobuf object of the input Response protobuf object for this derived Query.
   */
  [[nodiscard]] proto::ResponseHeader mapResponseHeader(const proto::Response& response) const override;

  /**
   * The ID of the token of which this query should get the info.
   */
  TokenId mTokenId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_INFO_QUERY_H_
