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
#ifndef CONTRACT_CALL_QUERY_H_
#define CONTRACT_CALL_QUERY_H_

#include "AccountId.h"
#include "ContractFunctionParameters.h"
#include "ContractId.h"
#include "Hbar.h"
#include "Query.h"

#include "helper/InitType.h"

#include <string>

namespace Hedera
{
class Client;
class ContractFunctionResult;
}

namespace proto
{
class Query;
class QueryHeader;
class Response;
class ResponseHeader;
}

namespace Hedera
{
/**
 * Call a function of the given smart contract instance, giving it
 * functionParameters as its inputs. It will consume the entire given amount of
 * gas.
 *
 * This is performed locally on the particular node that the client is
 * communicating with. It cannot change the state of the contract instance (and
 * so, cannot spend anything from the instance's cryptocurrency account).
 *
 * It will not have a consensus timestamp. It cannot generate a record or a
 * receipt. The response will contain the output returned by the function call.
 * This is useful for calling getter functions, which purely read the state and
 * don't change it. It is faster and cheaper than a normal call, because it is
 * purely local to a single node.
 */
class ContractCallQuery
  : public Query<ContractFunctionResult, ContractCallQuery>
{
public:
  /**
   * Constructor.
   */
  ContractCallQuery();

  /**
   * Derived from Query. Validate the checksums.
   *
   * @param client The client with which to validate the checksums.
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
   * Derived from Query. Get the contract call header from the response.
   *
   * @param response The associated response to this query.
   * @return         The response header for the contract call query.
   */
  virtual proto::ResponseHeader mapResponseHeader(
    proto::Response* response) const override;

  /**
   * Derived from Query. Grab the contract byte code query header.
   *
   * @param query  The query of which to extract the header.
   * @return       The account info query header.
   */
  virtual proto::QueryHeader mapRequestHeader(
    const proto::Query& query) const override;

  /**
   * Derived from Query. Extract the contract byte code data from the response
   * object.
   *
   * @param response  The received response from Hedera.
   * @param accountId The account ID that made the request.
   * @param query     The original query.
   * @return          The contract byte code data.
   */
  virtual ContractFunctionResult mapResponse(
    const proto::Response& response,
    const AccountId& accountId,
    const proto::Query& query) const override;

  /**
   * Sets the contract instance to call, in the format used in transactions.
   *
   * @param contractId The contract ID to be set.
   * @return           Reference to this ContractCallQuery object.
   */
  ContractCallQuery& setContractId(const ContractId& contractId);

  /**
   * Sets the amount of gas to use for the call. All of the gas offered will be
   * charged for.
   *
   * @param gas The gas to be set.
   * @return    Reference to this ContractCallQuery object.
   */
  ContractCallQuery& setGas(const int64_t& gas);

  /**
   * Sets the function parameters as their raw bytes.
   *
   * Use this instead of setFunction(std::string,
   * ContractFunctionParameters) if you have already pre-encoded a solidity
   * function call.
   *
   * @param functionParameters The function parameters to be set.
   * @return                   Reference to this ContractCallQuery object.
   */
  ContractCallQuery& setFunctionParameters(
    const std::string& functionParameters);

  /**
   * Sets the function name to call.
   *
   * The function will be called with no parameters. Use
   * setFunction(std::string, ContractFunctionParameters) to call a function
   * with parameters.
   *
   * @param name The function name to be set.
   * @return     Reference to this ContractCallQuery object.
   */
  ContractCallQuery& setFunction(const std::string& name);

  /**
   * Sets the function to call, and the parameters to pass to the function.
   *
   * @param name   The function name to be set.
   * @param params The parameters to pass.
   * @return       Reference to this ContractCallQuery object.
   */
  ContractCallQuery& setFunction(const std::string& name,
                                 const ContractFunctionParameters& params);

  /**
   * Sets the account that is the "sender." If not present it is the account ID
   * from the transaction ID. Typically a different value than specified in the
   * transaction ID requires a valid signature over either the hedera
   * transaction or foreign transaction data.
   *
   * @param senderAccountId The "sender" account.
   * @return                Reference to this ContractCallQuery object.
   */
  ContractCallQuery& setSenderAccountId(const AccountId& senderAccountId);

  /**
   * Extract the contract ID.
   *
   * @return The contract ID. Can be invalid if not set.
   */
  inline InitType<ContractId> getContractId() { return mContractId; }

  /**
   * Extract the gas.
   *
   * @return The gas.
   */
  int64_t getGas() { return mGas; }

  /**
   * Extract the function parameters.
   *
   * @return The function parameters.
   */
  inline std::string getFunctionParameters() const
  {
    return mFunctionParameters;
  }

  /**
   * Extract the sender account ID.
   *
   * @return The sender account ID.
   */
  inline InitType<AccountId> getSenderAccountId() const
  {
    return mSenderAccountId;
  }

private:
  /**
   * The contract against which to make a static call.
   */
  InitType<ContractId> mContractId;

  /**
   * The amount of gas to use for the call. All of the gas offered will be used
   * and charged a corresponding fee.
   */
  int64_t mGas;

  /**
   * The function to call, and the parameters to pass to the function.
   */
  std::string mFunctionParameters;

  /**
   * The account that is the "sender." If not present it is the accountId from
   * the transactionId. Typically a different value than specified in the
   * transactionId requires a valid signature over either the hedera transaction
   * or foreign transaction data.
   */
  InitType<AccountId> mSenderAccountId;
};

} // namespace Hedera

#endif // CONTRACT_CALL_QUERY_H_