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
#ifndef HEDERA_SDK_CPP_CONTRACT_BYTE_CODE_QUERY_H_
#define HEDERA_SDK_CPP_CONTRACT_BYTE_CODE_QUERY_H_

#include "ContractId.h"
#include "Query.h"

#include <cstddef>
#include <vector>

namespace Hedera
{
/**
 * A query that returns the bytecode for a smart contract instance. Anyone can request the byte code of a smart contract
 * instance on the network. Queries do not change the state of the smart contract or require network consensus. The
 * information is returned from a single node processing the query.
 */
using ContractByteCode = std::vector<std::byte>;
class ContractByteCodeQuery : public Query<ContractByteCodeQuery, ContractByteCode>
{
public:
  /**
   * Set the ID of the contract of which to request the byte code.
   *
   * @param contractId The ID of the contract of which to request the byte code.
   * @return A reference to this ContractByteCodeQuery object with the newly-set contract ID.
   */
  ContractByteCodeQuery& setContractId(const ContractId& contractId);

  /**
   * Get the ID of the contract of which this query is currently configured to get the byte code.
   *
   * @return The ID of the contract for which this query is meant.
   */
  [[nodiscard]] inline ContractId getContractId() const { return mContractId; }

private:
  /**
   * Derived from Executable. Construct a Query protobuf object from this ContractByteCodeQuery object.
   *
   * @param client The Client trying to construct this ContractByteCodeQuery.
   * @param node   The Node to which this ContractByteCodeQuery will be sent.
   * @return A Query protobuf object filled with this ContractByteCodeQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct an ContractByteCode object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct an ContractByteCode object.
   * @return An ContractByteCode object filled with the Response protobuf object's data
   */
  [[nodiscard]] ContractByteCode mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the status response code for a submitted ContractByteCodeQuery from a Response
   * protobuf object.
   *
   * @param response The Response protobuf object from which to grab the ContractByteCodeQuery status response code.
   * @return The ContractByteCodeQuery status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit this ContractByteCodeQuery to a Node.
   *
   * @param client   The Client submitting this ContractByteCodeQuery.
   * @param deadline The deadline for submitting this ContractByteCodeQuery.
   * @param node     Pointer to the Node to which this ContractByteCodeQuery should be submitted.
   * @param response Pointer to the Response protobuf object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::Response* response) const override;

  /**
   * The ID of the contract of which this query should get the byte code.
   */
  ContractId mContractId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_BYTE_CODE_QUERY_H_
