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
#include "AccountRecordsQuery.h"

/**
 * Library includes
 */
#include "AccountId.h"
#include "AccountInfo.h"
#include "TransactionRecord.h"

/**
 * Protobuf includes
 */
#include "crypto_get_account_records.pb.h"
#include "query.pb.h"
#include "query_header.pb.h"
#include "response.pb.h"

namespace Hedera
{
//-----
AccountRecordsQuery::AccountRecordsQuery()
  : mAccountId()
{
}

//-----
void
AccountRecordsQuery::validateChecksums(const Client& client) const
{
  if (mAccountId.isValid())
  {
    mAccountId.getValue().validateChecksum(client);
  }
}

//-----
void
AccountRecordsQuery::onMakeRequest(proto::Query* query,
                                   proto::QueryHeader* header) const
{
  proto::CryptoGetAccountRecordsQuery* getRecordsQuery =
    query->mutable_cryptogetaccountrecords();

  if (mAccountId.isValid())
  {
    getRecordsQuery->set_allocated_accountid(
      mAccountId.getValue().toProtobuf());
  }

  getRecordsQuery->set_allocated_header(header);
}

//-----
proto::ResponseHeader
AccountRecordsQuery::mapResponseHeader(proto::Response* response) const
{
  return response->cryptogetaccountrecords().header();
}

//-----
proto::QueryHeader
AccountRecordsQuery::mapRequestHeader(const proto::Query& query) const
{
  return query.cryptogetaccountrecords().header();
}

//-----
std::vector<TransactionRecord>
AccountRecordsQuery::mapResponse(const proto::Response& response,
                                 const AccountId& accountId,
                                 const proto::Query& query) const
{
  std::vector<TransactionRecord> records;
  for (size_t i = 0; i < response.cryptogetaccountrecords().records_size(); ++i)
  {
    records.push_back(TransactionRecord::fromProtobuf(
      response.cryptogetaccountrecords().records(i)));
  }

  return records;
}

//-----
AccountRecordsQuery&
AccountRecordsQuery::setAccountId(const AccountId& accountId)
{
  mAccountId.setValue(accountId);
  return *this;
}

} // namespace Hedera
