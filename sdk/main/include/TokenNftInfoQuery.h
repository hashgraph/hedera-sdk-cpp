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
#ifndef HEDERA_SDK_CPP_TOKEN_NFT_INFO_QUERY_H_
#define HEDERA_SDK_CPP_TOKEN_NFT_INFO_QUERY_H_

#include "NftId.h"
#include "Query.h"

namespace Hedera
{
class TokenNftInfo;
class TransactionRecord;
}

namespace Hedera
{
/**
 * A query that returns information about a non-fungible token (NFT). You request the info for an NFT by specifying the
 * NFT ID.
 *
 * Only when a spender is set on an explicit NFT ID of a token, the spender ID is returned for the respective NFT. If
 * mApproveTokenNftAllowanceAllSerials is used to approve all NFTs for a given token class and no NFT ID is specified,
 * the spender ID for all the serial numbers of that token will not be returned.
 */
class TokenNftInfoQuery : public Query<TokenNftInfoQuery, TokenNftInfo>
{
public:
  /**
   * Set the ID of the NFT of which to request the info.
   *
   * @param nft The ID of the NFT of which to request the info.
   * @return A reference to this TokenNftInfoQuery object with the newly-set NFT ID.
   */
  TokenNftInfoQuery& setNftId(const NftId& nft);

  /**
   * Get the ID of the NFT of which this query is currently configured to get the info.
   *
   * @return The ID of the NFT for which this query is meant.
   */
  [[nodiscard]] inline NftId getNftId() const { return mNftId; }

private:
  /**
   * Derived from Executable. Construct a Query protobuf object from this TokenNftInfoQuery object.
   *
   * @param client The Client trying to construct this TokenNftInfoQuery.
   * @param node   The Node to which this TokenNftInfoQuery will be sent.
   * @return A Query protobuf object filled with this TokenNftInfoQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct a TokenNftInfo object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct a TokenNftInfo object.
   * @return A TokenNftInfo object filled with the Response protobuf object's data.
   */
  [[nodiscard]] TokenNftInfo mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the status response code for a submitted TokenNftInfoQuery from a Response protobuf
   * object.
   *
   * @param response The Response protobuf object from which to grab the TokenNftInfoQuery status response code.
   * @return The TokenNftInfoQuery status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit this TokenNftInfoQuery to a Node.
   *
   * @param client   The Client submitting this TokenNftInfoQuery.
   * @param deadline The deadline for submitting this TokenNftInfoQuery.
   * @param node     Pointer to the Node to which this TokenNftInfoQuery should be submitted.
   * @param response Pointer to the Response protobuf object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::Response* response) const override;

  /**
   * The ID of the NFT of which this query should get the info.
   */
  NftId mNftId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_NFT_INFO_QUERY_H_
