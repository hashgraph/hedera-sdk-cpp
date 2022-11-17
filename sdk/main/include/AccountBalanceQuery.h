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
#ifndef HEDERA_SDK_CPP_ACCOUNT_BALANCE_QUERY_H_
#define HEDERA_SDK_CPP_ACCOUNT_BALANCE_QUERY_H_

#include "AccountId.h"
#include "ContractId.h"
#include "Query.h"

#include <optional>

namespace Hedera
{
namespace internal
{
class Node;
}
class AccountBalance;
class Client;
}

namespace proto
{
class Query;
class Response;
}

namespace grpc
{
class ClientContext;
class Status;
}

namespace Hedera
{
/**
 * Get the balance of a Hedera crypto-currency account. This returns only the balance, so it is a smaller and faster
 * reply than an AccountInfoQuery.
 *
 * This query is free.
 */
class AccountBalanceQuery : public Query<AccountBalanceQuery, AccountBalance>
{
public:
  ~AccountBalanceQuery() override = default;

  /**
   * Derived from Executable. Create a clone of this AccountBalanceQuery.
   *
   * @return A pointer to the created clone.
   */
  [[nodiscard]] std::unique_ptr<Executable> clone() const override;

  /**
   * Set the ID of the account of which to request the balance. This is mutually exclusive with setContractId() and will
   * clear the contract ID if one is already set.
   *
   * @param accountId The ID of the desired account of which to request the balance.
   * @return A reference to this AccountBalanceQuery object with the newly-set account ID.
   */
  AccountBalanceQuery& setAccountId(const AccountId& accountId);

  /**
   * Set the ID of the contract of which to request the balance. This is mutually exclusive with setAccountId() and will
   * clear the account ID if one is already set.
   *
   * @param contractId The ID of the desired contract of which to request the balance.
   * @return A reference to this AccountBalanceQuery object with the newly-set contract ID.
   */
  AccountBalanceQuery& setContractId(const ContractId& contractId);

  /**
   * Get the ID of the account of which this query is currently configured to get the balance.
   *
   * @return The ID of the account for which this query is meant. Returns uninitialized if a value has not yet been set,
   *         or if a contract ID has been set most recently.
   */
  [[nodiscard]] inline std::optional<AccountId> getAccountId() { return mAccountId; }

  /**
   * Get the ID of the contract of which this query is currently configured to get the balance.
   *
   * @return The ID of the contract for which this query is meant. Returns uninitialized if a value has not yet been
   *         set, or if an account ID has been set most recently.
   */
  [[nodiscard]] inline std::optional<ContractId> getContractId() { return mContractId; }

protected:
  /**
   * Derived from Executable. Construct a Query protobuf object from this AccountBalanceQuery object.
   *
   * @param client The Client trying to construct this AccountBalanceQuery.
   * @param node   The Node on which this AccountBalanceQuery will be sent.
   * @return A Query protobuf object filled with this AccountBalanceQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& client,
                                         const std::shared_ptr<internal::Node>& node) const override;

  /**
   * Derived from Executable. Construct an AccountBalance object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct an AccountBalance object.
   * @return An AccountBalance object filled with the Response protobuf object's data
   */
  [[nodiscard]] AccountBalance mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Get the gRPC method to call to send this AccountBalanceQuery.
   *
   * @param node The Node to which this AccountBalanceQuery is being sent and from which to retrieve the gRPC method.
   * @return The gRPC method to call to execute this AccountBalanceQuery.
   */
  [[nodiscard]] std::function<grpc::Status(grpc::ClientContext*, const proto::Query&, proto::Response*)> getGrpcMethod(
    const std::shared_ptr<internal::Node>& node) const override;

private:
  /**
   * The ID of the account of which this query should get the balance.
   */
  std::optional<AccountId> mAccountId;

  /**
   * The ID of the contract of which this query should get the balance.
   */
  std::optional<ContractId> mContractId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ACCOUNT_BALANCE_QUERY_H_
