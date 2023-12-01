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
#include "TransactionResponse.h"
#include "Client.h"
#include "TransactionReceipt.h"
#include "TransactionReceiptQuery.h"
#include "TransactionRecord.h"
#include "TransactionRecordQuery.h"
#include "impl/HexConverter.h"

#include <nlohmann/json.hpp>

namespace Hedera
{
//-----
TransactionResponse::TransactionResponse(AccountId nodeId, TransactionId transactionId, std::vector<std::byte> hash)
  : mNodeId(std::move(nodeId))
  , mTransactionHash(std::move(hash))
  , mTransactionId(std::move(transactionId))
{
}

//-----
TransactionReceipt TransactionResponse::getReceipt(const Client& client) const
{
  return getReceipt(client, client.getRequestTimeout());
}

//-----
TransactionReceipt TransactionResponse::getReceipt(const Client& client,
                                                   const std::chrono::system_clock::duration& timeout) const
{
  TransactionReceipt txReceipt = getReceiptQuery().execute(client, timeout);

  if (mValidateStatus)
  {
    txReceipt.validateStatus();
  }

  return txReceipt;
}

//-----
TransactionReceiptQuery TransactionResponse::getReceiptQuery() const
{
  return TransactionReceiptQuery().setTransactionId(mTransactionId).setNodeAccountIds({ mNodeId });
}

//-----
std::future<TransactionReceipt> TransactionResponse::getReceiptAsync(const Client& client) const
{
  return getReceiptAsync(client, client.getRequestTimeout());
}

//-----
std::future<TransactionReceipt> TransactionResponse::getReceiptAsync(
  const Client& client,
  const std::chrono::system_clock::duration& timeout) const
{
  return std::async(std::launch::async, [this, &client, &timeout]() { return this->getReceipt(client, timeout); });
}

//-----
void TransactionResponse::getReceiptAsync(
  const Client& client,
  const std::function<void(const TransactionReceipt&, const std::exception&)>& callback) const
{
  return getReceiptAsync(client, client.getRequestTimeout(), callback);
}

//-----
void TransactionResponse::getReceiptAsync(
  const Client& client,
  const std::chrono::system_clock::duration& timeout,
  const std::function<void(const TransactionReceipt&, const std::exception&)>& callback) const
{
  std::future<TransactionReceipt> future =
    std::async(std::launch::async, [this, &client, &timeout]() { return this->getReceipt(client, timeout); });

  try
  {
    callback(future.get(), std::exception());
  }
  catch (const std::exception& exception)
  {
    callback(TransactionReceipt(), exception);
  }
}

//-----
void TransactionResponse::getReceiptAsync(const Client& client,
                                          const std::function<void(const TransactionReceipt&)>& responseCallback,
                                          const std::function<void(const std::exception&)>& exceptionCallback) const
{
  return getReceiptAsync(client, client.getRequestTimeout(), responseCallback, exceptionCallback);
}

//-----
void TransactionResponse::getReceiptAsync(const Client& client,
                                          const std::chrono::system_clock::duration& timeout,
                                          const std::function<void(const TransactionReceipt&)>& responseCallback,
                                          const std::function<void(const std::exception&)>& exceptionCallback) const
{
  std::future<TransactionReceipt> future =
    std::async(std::launch::async, [this, &client, &timeout]() { return this->getReceipt(client, timeout); });

  try
  {
    responseCallback(future.get());
  }
  catch (const std::exception& exception)
  {
    exceptionCallback(exception);
  }
}

//-----
TransactionRecord TransactionResponse::getRecord(const Client& client) const
{
  return getRecord(client, client.getRequestTimeout());
}

//-----
TransactionRecord TransactionResponse::getRecord(const Client& client,
                                                 const std::chrono::system_clock::duration& timeout) const
{
  return TransactionRecordQuery().setTransactionId(mTransactionId).execute(client, timeout);
}

//-----
TransactionRecordQuery TransactionResponse::getRecordQuery() const
{
  return TransactionRecordQuery().setTransactionId(mTransactionId).setNodeAccountIds({ mNodeId });
}

//-----
std::future<TransactionRecord> TransactionResponse::getRecordAsync(const Client& client) const
{
  return getRecordAsync(client, client.getRequestTimeout());
}

//-----
std::future<TransactionRecord> TransactionResponse::getRecordAsync(
  const Client& client,
  const std::chrono::system_clock::duration& timeout) const
{
  return std::async(std::launch::async, [this, &client, &timeout]() { return this->getRecord(client, timeout); });
}

//-----
void TransactionResponse::getRecordAsync(
  const Client& client,
  const std::function<void(const TransactionRecord&, const std::exception&)>& callback) const
{
  return getRecordAsync(client, client.getRequestTimeout(), callback);
}

//-----
void TransactionResponse::getRecordAsync(
  const Client& client,
  const std::chrono::system_clock::duration& timeout,
  const std::function<void(const TransactionRecord&, const std::exception&)>& callback) const
{
  std::future<TransactionRecord> future =
    std::async(std::launch::async, [this, &client, &timeout]() { return this->getRecord(client, timeout); });

  try
  {
    callback(future.get(), std::exception());
  }
  catch (const std::exception& exception)
  {
    callback(TransactionRecord(), exception);
  }
}

//-----
void TransactionResponse::getRecordAsync(const Client& client,
                                         const std::function<void(const TransactionRecord&)>& responseCallback,
                                         const std::function<void(const std::exception&)>& exceptionCallback) const
{
  return getRecordAsync(client, client.getRequestTimeout(), responseCallback, exceptionCallback);
}

//-----
void TransactionResponse::getRecordAsync(const Client& client,
                                         const std::chrono::system_clock::duration& timeout,
                                         const std::function<void(const TransactionRecord&)>& responseCallback,
                                         const std::function<void(const std::exception&)>& exceptionCallback) const
{
  std::future<TransactionRecord> future =
    std::async(std::launch::async, [this, &client, &timeout]() { return this->getRecord(client, timeout); });

  try
  {
    responseCallback(future.get());
  }
  catch (const std::exception& exception)
  {
    exceptionCallback(exception);
  }
}

//-----
std::string TransactionResponse::toString() const
{
  nlohmann::json json;
  json["mNodeId"] = mNodeId.toString();
  json["mTransactionHash"] = internal::HexConverter::bytesToHex(mTransactionHash);
  json["mTransactionId"] = mTransactionId.toString();
  return json.dump();
}

//-----
std::ostream& operator<<(std::ostream& os, const TransactionResponse& response)
{
  os << response.toString();
  return os;
}

//-----
TransactionResponse& TransactionResponse::setValidateStatus(bool validate)
{
  mValidateStatus = validate;
  return *this;
}

} // namespace Hedera
