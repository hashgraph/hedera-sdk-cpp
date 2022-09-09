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

/**
 * Protobuf includes
 */
#include "transaction_body.pb.h"

/**
 * STL includes
 */
#include <unordered_map>

/**
 * Hedera forward declarations
 */
namespace Hedera
{
class AccountId;
class TransactionId;
} // namespace Hedera

namespace Hedera
{
/**
 * Base class for all transactions that may be built and submitted to Hedera.
 *
 * @param <T> The type of the transaction. Used to enable chaining.
 */
template<typename T>
class Transaction
{
protected:
  Transaction() {}

  Transaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  {
  }

  Transaction(const proto::TransactionBody& transaction) {}

  void requireNotFrozen() {}

  proto::TransactionBody mSourceTransactionBody;
};

} // namespace Hedera

#endif // TRANSACTION_H_