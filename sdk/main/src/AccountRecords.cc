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
#include "AccountRecords.h"
#include "TransactionRecord.h"

#include <proto/crypto_get_account_records.pb.h>

namespace Hedera
{
//-----
AccountRecords AccountRecords::fromProtobuf(const proto::CryptoGetAccountRecordsResponse& proto)
{
  AccountRecords records;
  records.mAccountId = AccountId::fromProtobuf(proto.accountid());

  for (int i = 0; i < proto.records_size(); ++i)
  {
    records.mRecords.push_back(TransactionRecord::fromProtobuf(proto.records(i)));
  }

  return records;
}

} // namespace Hedera
