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
#include "ChunkedTransaction.h"
#include "Client.h"
#include "TransactionResponse.h"
#include "impl/Utilities.h"

#include <algorithm>

namespace Hedera
{
//-----
template<typename SdkRequestType>
TransactionResponse ChunkedTransaction<SdkRequestType>::execute(const Client& client)
{
  return executeAll(client).at(0);
}

//-----
template<typename SdkRequestType>
TransactionResponse ChunkedTransaction<SdkRequestType>::execute(const Client& client,
                                                                const std::chrono::duration<double>& timeout)
{
  return executeAll(client, timeout).at(0);
}

//-----
template<typename SdkRequestType>
std::vector<TransactionResponse> ChunkedTransaction<SdkRequestType>::executeAll(const Client& client)
{
  return executeAll(client, client.getRequestTimeout());
}

//-----
template<typename SdkRequestType>
std::vector<TransactionResponse> ChunkedTransaction<SdkRequestType>::executeAll(
  const Client& client,
  const std::chrono::duration<double>& timeout)
{
  std::vector<TransactionResponse> responses;
  std::vector<std::byte> allData = std::move(mData);

  for (long byte = 0; byte < allData.size(); byte += mChunkSize)
  {
    // Move the next chunk into mData (making sure not to move more bytes than are left)
    mData.insert(mData.cbegin(),
                 std::make_move_iterator(allData.cbegin() + byte),
                 std::make_move_iterator(
                   allData.cbegin() + byte +
                   ((mChunkSize + byte > allData.size()) ? mChunkSize + byte - allData.size() : mChunkSize)));

    // Execute this chunk
    responses.push_back(
      Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::execute(
        client, timeout));

    // Move the chunk back into fullData
    allData.insert(
      allData.cbegin() + byte, std::make_move_iterator(mData.cbegin()), std::make_move_iterator(mData.cend()));
  }

  // Move fullData back into mData
  mData = std::move(allData);

  return responses;
}

//-----
template<typename SdkRequestType>
SdkRequestType& ChunkedTransaction<SdkRequestType>::setData(const std::vector<std::byte>& data)
{
  Transaction<SdkRequestType>::requireNotFrozen();
  mData = data;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& ChunkedTransaction<SdkRequestType>::setData(std::string_view data)
{
  Transaction<SdkRequestType>::requireNotFrozen();
  mData = internal::Utilities::stringToByteVector(data);
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& ChunkedTransaction<SdkRequestType>::setMaxChunks(unsigned int chunks)
{
  Transaction<SdkRequestType>::requireNotFrozen();
  mMaxChunks = chunks;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& ChunkedTransaction<SdkRequestType>::setChunkSize(unsigned int size)
{
  Transaction<SdkRequestType>::requireNotFrozen();
  mChunkSize = size;
  return static_cast<SdkRequestType&>(*this);
}

} // namespace Hedera
