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
#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include "Executable.h"
#include "Hbar.h"

#include <chrono>
#include <memory>
#include <unordered_map>

namespace Hedera
{
class TransactionResponse;
}

namespace proto
{
class Transaction;
class TransactionBody;
class TransactionResponse;
}

namespace Hedera
{
/**
 * Base class for all transactions that can be submitted to Hedera.
 *
 * @tparam SdkRequestType  The SDK request type.
 */
template<typename SdkRequestType>
class Transaction
  : public Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>
{
protected:
  /**
   * Default destructor
   */
  virtual ~Transaction() = default;

  /**
   * Derived from Executable. Construct a transaction protobuf object from this transaction.
   *
   * @return The transaction protobuf object that contains this transaction information.
   */
  virtual proto::Transaction makeRequest() const = 0;

  /**
   * Derived from Executable. Create a response object from a protobuf response object.
   *
   * @param response The protobuf response object.
   * @return The response object with the response data.
   */
  virtual TransactionResponse mapResponse(const proto::Response& response) const = 0;

private:
  /**
   * The protobuf transaction body for this transaction.
   */
  std::shared_ptr<proto::TransactionBody> mSourceTransactionBody;

  /**
   * The default maximum transaction fee.
   */
  Hbar mDefaultMaxTransactionFee;

  /**
   * The default auto renew period.
   */
  std::chrono::duration<double> mDefaultAutoRenewPeriod;
};

} // namespace Hedera

#endif // TRANSACTION_H_