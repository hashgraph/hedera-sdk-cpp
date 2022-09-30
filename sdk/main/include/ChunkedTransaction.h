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
#ifndef CHUNKED_TRANSACTION_H_
#define CHUNKED_TRANSACTION_H_

#include "Transaction.h"

#include <unordered_map>

namespace Hedera
{
class AccountId;
class TransactionId;
}

namespace proto
{
class TransactionBody;
}

namespace Hedera
{
template<typename T>
class ChunkedTransaction : public Transaction<T>
{
public:
  void setChunkSize(int chunkSize) { return; }

protected:
  ChunkedTransaction() {}

  ChunkedTransaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  {
  }

  ChunkedTransaction(const proto::TransactionBody& transaction) {}

private:
  /**
   * The size of a chunk
   */
  int mChunkSize = 1024;

  /**
   * The maximum number of chunks this message will get broken up into when its
   * frozen.
   */
  int mMaxChunks = 20;
};

} // namespace Hedera

#endif // CHUNKED_TRANSACTION_H_