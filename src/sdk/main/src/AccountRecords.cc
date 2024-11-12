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
#include "AccountRecords.h"
#include "TransactionRecord.h"
#include "impl/Utilities.h"

#include <crypto_get_account_records.pb.h>
#include <nlohmann/json.hpp>

namespace Hiero
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

//-----
AccountRecords AccountRecords::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::CryptoGetAccountRecordsResponse proto;
  proto.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(proto);
}

//-----
std::unique_ptr<proto::CryptoGetAccountRecordsResponse> AccountRecords::toProtobuf() const
{
  auto proto = std::make_unique<proto::CryptoGetAccountRecordsResponse>();
  proto->set_allocated_accountid(mAccountId.toProtobuf().release());

  for (const TransactionRecord& record : mRecords)
  {
    *proto->add_records() = *record.toProtobuf();
  }

  return proto;
}

//-----
std::vector<std::byte> AccountRecords::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string AccountRecords::toString() const
{
  nlohmann::json json;
  json["mAccountId"] = mAccountId.toString();
  std::for_each(mRecords.cbegin(),
                mRecords.cend(),
                [&json](const TransactionRecord& record) { json["mRecords"].push_back(record.toString()); });
  return json.dump();
}

//-----
std::ostream& operator<<(std::ostream& os, const AccountRecords& records)
{
  os << records.toString();
  return os;
}

} // namespace Hiero
