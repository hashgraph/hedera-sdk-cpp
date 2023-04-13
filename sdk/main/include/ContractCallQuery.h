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
#ifndef HEDERA_SDK_CPP_CONTRACT_CALL_QUERY_H_
#define HEDERA_SDK_CPP_CONTRACT_CALL_QUERY_H_

#include "AccountId.h"
#include "ContractId.h"
#include "Query.h"

#include <cstddef>
#include <vector>

namespace Hedera
{
class ContractFunctionResult;
}

namespace Hedera
{
/**
 * A query that calls a function of the given smart contract instance, giving it function parameters as its inputs. This
 * is performed locally on the particular node that the client is communicating with. It cannot change the state of the
 * contract instance (and so, cannot spend anything from the instance's cryptocurrency account). It will not have a
 * consensus timestamp. It cannot generate a record or a receipt. The response will contain the output returned by the
 * function call. This is useful for calling getter functions, which purely read the state and don't change it. It is
 * faster and cheaper than a normal call because it is purely local to a single node.
 *
 * Unlike a contract execution transaction, the node will consume the entire amount of provided gas in determining the
 * fee for this query.
 */
class ContractCallQuery : public Query<ContractCallQuery, ContractFunctionResult>
{
public:
  /**
   * Set the ID of the contract from which to call a function.
   *
   * @param contractId The ID of the contract from which to call a function.
   * @return A reference to this ContractCallQuery object with the newly-set contract ID.
   */
  ContractCallQuery& setContractId(const ContractId& contractId);

  /**
   * Set the amount of gas to use for the call. All of the gas offered will be used and charged a corresponding fee.
   *
   * @param gas The amount of gas to use for the call.
   * @return A reference to this ContractCallQuery object with the newly-set gas.
   */
  ContractCallQuery& setGas(const uint64_t& gas);

  /**
   * Set the function parameters for the function call.
   *
   * @param functionParameters The function parameters for the function call.
   * @return A reference to this ContractCallQuery object with the newly-set function parameters.
   */
  ContractCallQuery& setFunctionParameters(const std::vector<std::byte>& functionParameters);

  /**
   * Set the ID of the account to be used as the "sender" for the function call.
   *
   * @param sender The ID of the account to be used as the "sender" for the function call.
   * @return A reference to this ContractCallQuery object with the newly-set sender account ID.
   */
  ContractCallQuery& setSenderAccountId(const AccountId& accountId);

  /**
   * Get the ID of the contract from which this query is currently configured to call a function.
   *
   * @return The ID of the contract from which this query is currently configured to call a function.
   */
  [[nodiscard]] inline ContractId getContractId() const { return mContractId; }

  /**
   * Get the amount of gas this query is currently configured to use for the function call.
   *
   * @return The amount of gas this query is currently configured to use for the function call.
   */
  [[nodiscard]] inline uint64_t getGas() const { return mGas; }

  /**
   * Get the function parameters this query is currently configured to use for the function call.
   *
   * @return The function parameters this query is currently configured to use for the function call.
   */
  [[nodiscard]] inline std::vector<std::byte> getFunctionParameters() const { return mFunctionParameters; }

  /**
   * Get the ID of the account this query is currently configured to use as the sender.
   *
   * @return The ID of the account this query is currently configured to use as the sender.
   */
  [[nodiscard]] inline AccountId getSenderAccountId() const { return mSenderAccountId; }

private:
  /**
   * Derived from Executable. Construct a Query protobuf object from this ContractCallQuery object.
   *
   * @param client The Client trying to construct this ContractCallQuery.
   * @param node   The Node to which this ContractCallQuery will be sent.
   * @return A Query protobuf object filled with this ContractCallQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct an ContractCallQuery object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct an ContractCallQuery object.
   * @return An ContractCallQuery object filled with the Response protobuf object's data
   */
  [[nodiscard]] ContractFunctionResult mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the status response code for a submitted ContractCallQuery from a Response
   * protobuf object.
   *
   * @param response The Response protobuf object from which to grab the ContractCallQuery status response code.
   * @return The ContractCallQuery status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit this ContractCallQuery to a Node.
   *
   * @param client   The Client submitting this ContractCallQuery.
   * @param deadline The deadline for submitting this ContractCallQuery.
   * @param node     Pointer to the Node to which this ContractCallQuery should be submitted.
   * @param response Pointer to the Response protobuf object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::Response* response) const override;

  /**
   * The ID of the contract from which this query should call a function.
   */
  ContractId mContractId;

  /**
   * The amount of gas this query should use for the function call.
   */
  uint64_t mGas = 0ULL;

  /**
   * The function parameters this query should use for the function call.
   */
  std::vector<std::byte> mFunctionParameters;

  /**
   * The ID of the account this query should use as the sender.
   */
  AccountId mSenderAccountId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_CALL_QUERY_H_
