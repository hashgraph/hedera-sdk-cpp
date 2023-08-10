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
#include "FileAppendTransaction.h"
#include "TopicMessageSubmitTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "impl/Utilities.h"

#include <algorithm>
#include <iostream>

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
  mData.clear();
  mData.reserve(mChunkSize);

  const auto totalChunks = static_cast<unsigned int>(std::ceil(static_cast<double>(allData.size()) / mChunkSize));

  // Generate the transaction ID for the first chunk before looping.
  Transaction<SdkRequestType>::setTransactionId(TransactionId::generate(client.getOperatorAccountId().value()));

  for (int chunk = 0; chunk < totalChunks; ++chunk)
  {
    // Create the chunk.
    createChunk(allData, chunk, totalChunks);

    // Execute this chunk
    responses.push_back(
      Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::execute(
        client, timeout));

    // Wait for the transaction to fully complete, if configured
    if (mShouldGetReceipt)
    {
      const TransactionReceipt txReceipt = responses.back().getReceipt(client);
    }

    // Generate a new transaction ID with the same account ID (to prevent DUPLICATE_TRANSACTION errors)
    Transaction<SdkRequestType>::setTransactionId(
      TransactionId::generate(Transaction<SdkRequestType>::getTransactionId().getAccountId()));
  }

  // Move fullData back into mData
  mData = std::move(allData);

  return responses;
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

//-----
template<typename SdkRequestType>
void ChunkedTransaction<SdkRequestType>::createChunk(const std::vector<std::byte>& data, int32_t chunk, int32_t total)
{
  const unsigned int startingByteForChunk = mChunkSize * chunk;
  const auto start = data.cbegin() + startingByteForChunk;

  mData = {
    start, start + ((mChunkSize + startingByteForChunk > data.size()) ? data.size() - startingByteForChunk : mChunkSize)
  };
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
  return setData(internal::Utilities::stringToByteVector(data));
}

//-----
template<typename SdkRequestType>
void ChunkedTransaction<SdkRequestType>::setShouldGetReceipt(bool retrieveReceipt)
{
  mShouldGetReceipt = retrieveReceipt;
}

/**
 * Explicit template instantiations.
 */
template class ChunkedTransaction<FileAppendTransaction>;
template class ChunkedTransaction<TopicMessageSubmitTransaction>;

} // namespace Hedera
