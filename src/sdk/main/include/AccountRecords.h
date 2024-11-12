/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#ifndef HIERO_SDK_CPP_ACCOUNT_RECORDS_H_
#define HIERO_SDK_CPP_ACCOUNT_RECORDS_H_

#include "AccountId.h"
#include "TransactionRecord.h"

#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace proto
{
class CryptoGetAccountRecordsResponse;
}

namespace Hiero
{
/**
 * Response from a Hiero network when the client sends an AccountRecordsQuery.
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
   * Construct an AccountRecords object from a byte array.
   *
   * @param bytes The byte array from which to construct an AccountRecords object.
   * @return The constructed AccountRecords object.
   */
  [[nodiscard]] static AccountRecords fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a CryptoGetAccountRecordsResponse protobuf object from this AccountRecords object.
   *
   * @return A pointer to the created CryptoGetAccountRecordsResponse protobuf object.
   */
  [[nodiscard]] std::unique_ptr<proto::CryptoGetAccountRecordsResponse> toProtobuf() const;

  /**
   * Construct a representative byte array from this AccountRecords object.
   *
   * @return The byte array representing this AccountRecords object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * Construct a string representation of this AccountRecords object.
   *
   * @return The string representation of this AccountRecords object.
   */
  [[nodiscard]] std::string toString() const;

  /**
   * Write this AccountRecords to an output stream.
   *
   * @param os      The output stream.
   * @param records The AccountRecords to print.
   * @return The output stream with this AccountRecords written to it.
   */
  friend std::ostream& operator<<(std::ostream& os, const AccountRecords& records);

  /**
   * The ID of the queried account.
   */
  AccountId mAccountId;

  /**
   * The list of TransactionRecords for the queried account.
   */
  std::vector<TransactionRecord> mRecords;
};

} // namespace Hiero

#endif // HIERO_SDK_CPP_ACCOUNT_RECORDS_H_
