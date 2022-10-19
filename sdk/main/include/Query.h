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
#ifndef QUERY_H_
#define QUERY_H_

#include "Executable.h"

namespace Hedera
{
class AccountBalance;
class AccountBalanceQuery;
class AccountId;
class Client;
class TransactionReceipt;
class TransactionReceiptQuery;
}

namespace proto
{
class Query;
class Response;
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
   * Does this query require a payment? Nearly all queries require payment,
   * so default to returning true. Queries that don't require payment can
   * overwrite this to return false.
   *
   * @return \c TRUE if this query requires a payment, otherwise \c FALSE.
   */
  inline virtual bool isPaymentRequired() const { return true; }

protected:
  /**
   * Default destructor.
   */
  virtual ~Query() = default;
};

/**
 * Explicit template instantiation
 */
template class Query<AccountBalanceQuery, AccountBalance>;
template class Query<TransactionReceiptQuery, TransactionReceipt>;

} // namespace Hedera

#endif // QUERY_H_