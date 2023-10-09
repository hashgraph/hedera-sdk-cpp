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
#ifndef HEDERA_SDK_CPP_QUERY_H_
#define HEDERA_SDK_CPP_QUERY_H_

#include "Executable.h"

#include <chrono>
#include <memory>

namespace proto
{
class Query;
class QueryHeader;
class Response;
class ResponseHeader;
class Transaction;
}

namespace Hedera
{
class AccountId;
class Client;
class Hbar;
class TransactionId;
}

namespace Hedera
{
/**
 * Base class for all queries that can be submitted to Hedera.
 *
 * @tparam SdkRequestType  The SDK request type.
 * @tparam SdkResponseType The SDK response type.
 */
template<typename SdkRequestType, typename SdkResponseType>
class Query : public Executable<SdkRequestType, proto::Query, proto::Response, SdkResponseType>
{
public:
  /**
   * Set an amount to pay for this Query. The Client will submit exactly this amount and no remainder will be returned.
   *
   * @param amount The amount to pay for this Query.
   * @return A reference to this derived Query with the newly-set payment.
   */
  SdkRequestType& setQueryPayment(const Hbar& amount);

  /**
   * Set a maximum amount to pay for this Query. A Query without an explicit payment amount set will first query for the
   * cost of the Query and attach a payment for that amount from the operator account on the client to the node account
   * ID. Set to 0 to disable automatic implicit payments.
   *
   * @param maxAmount The maximum amount to pay for this Query.
   * @return A reference to this derived Query with the newly-set payment.
   */
  SdkRequestType& setMaxQueryPayment(const Hbar& maxAmount);

  /**
   * Get the expected cost of this Query.
   *
   * @param client The Client to use to fetch the cost.
   * @return The expected cost of this Query.
   */
  [[nodiscard]] Hbar getCost(const Client& client);

  /**
   * Get the expected cost of this Query with a specific timeout.
   *
   * @param client  The Client to use to fetch the cost.
   * @param timeout The timeout to use to get the cost.
   * @return The expected cost of this Query.
   */
  [[nodiscard]] Hbar getCost(const Client& client, const std::chrono::duration<double>& timeout);

protected:
  /**
   * Prevent public copying and moving to prevent slicing. Use the 'clone()' virtual method instead.
   */
  Query();
  ~Query();
  Query(const Query&);
  Query& operator=(const Query&);
  Query(Query&&) noexcept;
  Query& operator=(Query&&) noexcept;

  /**
   * Get the cost of the Query from the ResponseHeader protobuf object and set it in this Query, if this Query was
   * configured to get the cost.
   *
   * @param header The ResponseHeader protobuf object from which to get the cost.
   */
  void saveCostFromHeader(const proto::ResponseHeader& header) const;

  /**
   * Is this Query meant a cost query?
   *
   * @return \c TRUE if this Query is meant to get the cost, otherwise \c FALSE.
   */
  [[nodiscard]] bool isCostQuery() const;

private:
  /**
   * Build a Query protobuf object with this Query's data, with the input QueryHeader protobuf object.
   *
   * @param header A pointer to the QueryHeader protobuf object to add to the Query protobuf object.
   * @return The constructed Query protobuf object.
   */
  [[nodiscard]] virtual proto::Query buildRequest(proto::QueryHeader* header) const = 0;

  /**
   * Get the ResponseHeader protobuf object from the input Response protobuf object.
   *
   * @param response The Response protobuf object from which to get the ResponseHeader protobuf object.
   * @return The ResponseHeader protobuf object of the input Response protobuf object for this derived Query.
   */
  [[nodiscard]] virtual proto::ResponseHeader mapResponseHeader(const proto::Response& response) const = 0;

  /**
   * Verify that all the checksums in this Transaction are valid.
   *
   * @param client The Client that should be used to validate the checksums.
   * @throws BadEntityException This Transaction's checksums are not valid.
   */
  virtual void validateChecksums(const Client& client) const = 0;

  /**
   * Derived from Executable. Construct a Query protobuf object from this Query, based on the node account ID at the
   * given index.
   *
   * @param index The index of the node account ID that's associated with the Node being used to execute this Query.
   * @return A Query protobuf object filled with this Query's data, based on the node account ID at the given index.
   */
  [[nodiscard]] proto::Query makeRequest(unsigned int index) const override;

  /**
   * Derived from Executable. Get the status response code from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to grab the status response code.
   * @return The status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Perform any needed actions for this Query when it is being submitted.
   *
   * @param client The Client being used to submit this Query.
   */
  void onExecute(const Client& client) override;

  /**
   * Does this Query require payment? Default to \c TRUE, as most Queries do.
   *
   * @return \c TRUE if this Query requires payment, otherwise \c FALSE.
   */
  [[nodiscard]] inline virtual bool isPaymentRequired() const { return true; }

  /**
   * Implementation object used to hide implementation details and internal headers.
   */
  struct QueryImpl;
  std::unique_ptr<QueryImpl> mImpl;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_QUERY_H_
