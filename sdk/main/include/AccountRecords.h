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
#ifndef HEDERA_SDK_CPP_ACCOUNT_RECORDS_H_
#define HEDERA_SDK_CPP_ACCOUNT_RECORDS_H_

#include "AccountId.h"
#include "TransactionRecord.h"

#include <vector>

namespace proto
{
class CryptoGetAccountRecordsResponse;
}

namespace Hedera
{
/**
 * Response from a Hedera network when the client sends an AccountRecordsQuery.
 */
class AccountRecords
{
public:
  /**
   * Construct an AccountRecords object from a CryptoGetAccountRecordsResponse protobuf object.
   *
   * @param proto The CryptoGetAccountRecordsResponse protobuf object from which to construct an AccountRecords object.
   * @return The constructed AccountRecords object.
   */
  [[nodiscard]] static AccountRecords fromProtobuf(const proto::CryptoGetAccountRecordsResponse& proto);

  /**
   * Get the ID of the queried account.
   *
   * @return The ID of the queried account.
   */
  [[nodiscard]] inline AccountId getAccountId() const { return mAccountId; }

  /**
   * Get the TransactionRecords of the queried account.
   *
   * @return The TransactionRecords of the queried account.
   */
  [[nodiscard]] inline std::vector<TransactionRecord> getRecords() const { return mRecords; }

private:
  /**
   * The ID of the queried account.
   */
  AccountId mAccountId;

  /**
   * The list of TransactionRecords for the queried account.
   */
  std::vector<TransactionRecord> mRecords;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ACCOUNT_RECORDS_H_
