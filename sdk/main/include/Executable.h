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
#ifndef EXECUTABLE_H
#define EXECUTABLE_H

#include "AccountId.h"

#include <chrono>
#include <vector>

namespace Hedera
{
class AccountBalance;
class AccountBalanceQuery;
class AccountCreateTransaction;
class Client;
class TransactionResponse;
}

namespace proto
{
class Query;
class Response;
class Transaction;
class TransactionResponse;
}

namespace Hedera
{
/**
 * Base class for queries and transactions that contains all the behavior for executing and submitting requests to a
 * Hedera network.
 *
 * @tparam SdkRequestType    The SDK request type.
 * @tparam ProtoRequestType  The protobuf request messate type.
 * @tparam ProtoResponseType The protobuf response message type.
 * @tparam SdkResponseType   The SDK response type.
 */
template<typename SdkRequestType, typename ProtoRequestType, typename ProtoResponseType, typename SdkResponseType>
class Executable
{
public:
  /**
   * Execute this executable.
   *
   * @param client The client with which this will be executed.
   * @return The result of execution.
   */
  SdkResponseType execute(const Client& client);

  /**
   * Execute this executable with an input timeout.
   *
   * @param client  The client with which this will be executed.
   * @param timeout The timeout for this execution.
   * @return The result of execution.
   */
  SdkResponseType execute(const Client& client, const std::chrono::duration<double>& duration);

  /**
   * Set the account IDs of the nodes to which this transaction will be submitted.
   *
   * @param nodeAccountIds The list of node account IDs to be set.
   * @return Reference to this Executable derived class.
   */
  SdkRequestType& setNodeAccountIds(const std::vector<AccountId>& nodeAccountIds);

  /**
   * Get the list of account IDs for nodes with which execution will be attempted.
   * @return The list of account IDs.
   */
  inline std::vector<AccountId> getNodeAccountIds() const { return mNodeAccountIds; }

protected:
  /**
   * Default destructor.
   */
  virtual ~Executable() = default;

  /**
   * Construct this request and place it in a protobuf object.
   *
   * @return The protobuf object that contains this request.
   */
  virtual ProtoRequestType makeRequest() const = 0;

  /**
   * Construct the response from a protobuf response object.
   *
   * @param response The protobuf response object.
   * @return The response object with the response data.
   */
  virtual SdkResponseType mapResponse(const ProtoResponseType& response) const = 0;

private:
  /**
   * The list of account IDs of nodes on which to attempt execution.
   */
  std::vector<AccountId> mNodeAccountIds;

  /**
   * The maximum number of attempts to send a request.
   */
  const int mMaxNumOfAttempts = 10;
};

/**
 * Explicit template instantiation
 */
template class Executable<AccountBalanceQuery, proto::Query, proto::Response, AccountBalance>;
template class Executable<AccountCreateTransaction,
                          proto::Transaction,
                          proto::TransactionResponse,
                          TransactionResponse>;

} // namespace Hedera

#endif // EXECUTABLE_H
