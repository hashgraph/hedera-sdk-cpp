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
/**
 * Get the balance of a Hedera crypto-currency account. This returns only the balance, so it is a smaller and faster
 * reply than an AccountInfoQuery.
 *
 * This query is free.
 */
class AccountBalanceQuery final : public Query<AccountBalanceQuery, AccountBalance>
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

private:
  /**
   * Derived from Executable. Construct a Query protobuf object from this AccountBalanceQuery object.
   *
   * @param client The Client trying to construct this AccountBalanceQuery. This is unused.
   * @param node   The Node to which this AccountBalanceQuery will be sent. This is unused.
   * @return A Query protobuf object filled with this AccountBalanceQuery object's data.
   */
  [[nodiscard]] proto::Query makeRequest(const Client& /*client*/,
                                         const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Construct an AccountBalance object from a Response protobuf object.
   *
   * @param response The Response protobuf object from which to construct a AccountBalance object.
   * @return An AccountBalance object filled with the Response protobuf object's data
   */
  [[nodiscard]] AccountBalance mapResponse(const proto::Response& response) const override;

  /**
   * Derived from Executable. Grab the status response code for a submitted AccountBalanceQuery from a Response protobuf
   * object.
   *
   * @param response The Response protobuf object from which to grab the AccountBalanceQuery status response code.
   * @return The AccountBalanceQuery status response code of the input Response protobuf object.
   */
  [[nodiscard]] Status mapResponseStatus(const proto::Response& response) const override;

  /**
   * Derived from Executable. Submit this AccountBalanceQuery to a Node.
   *
   * @param client   The Client submitting this AccountBalanceQuery.
   * @param deadline The deadline for submitting this AccountBalanceQuery.
   * @param node     Pointer to the Node to which this AccountBalanceQuery should be submitted.
   * @param response Pointer to the Response protobuf object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::Response* response) const override;

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
