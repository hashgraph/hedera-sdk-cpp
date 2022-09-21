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
#ifndef TRANSACTION_RECORD_H_
#define TRANSACTION_RECORD_H_

namespace proto
{
class TransactionRecord;
}

namespace Hedera
{
class TransactionRecord
{
public:
  static TransactionRecord fromProtobuf(const proto::TransactionRecord& proto)
  {
    return TransactionRecord();
  }
};

} // namespace Hedera

#endif // TRANSACTION_RECORD_H_