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
#include "exceptions/IllegalStateException.h"
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
  // Determine how many chunks are going to be required to send this whole ChunkedTransaction and make sure it's within
  // the set limit.
  const auto requiredChunks =
    static_cast<unsigned int>(std::ceil(static_cast<double>(mData.size()) / static_cast<double>(mChunkSize)));
  if (requiredChunks > mMaxChunks)
  {
    throw IllegalStateException("Transaction requires " + std::to_string(requiredChunks) + " but is only allotted " +
                                std::to_string(mMaxChunks) + ". Try using setMaxChunks()");
  }

  // Container to hold responses.
  std::vector<TransactionResponse> responses;

  // The transaction chunks being sent should all have the same TransactionId besides their valid start time being a
  // little off. The first TransactionId should be saved so that it can be used to generate the IDs for the other
  // chunks.
  TransactionId firstTransactionId;

  // Move the data out to prevent an unnecessary copy of all the data.
  std::vector<std::byte> data = std::move(mData);
  mData.clear();
  mData.reserve(mChunkSize);

  for (int chunk = 0; chunk < requiredChunks; ++chunk)
  {
    // Create a copy of this entire transaction. This should carry over all manually-set items, if any.
    SdkRequestType tx = *static_cast<SdkRequestType*>(this);

    // Create the chunk and set it in the transaction.
    const unsigned int startingByteForChunk = mChunkSize * chunk;
    const auto start = data.cbegin() + startingByteForChunk;
    tx.mData = { start,
                 start + ((mChunkSize + startingByteForChunk > data.size()) ? data.size() - startingByteForChunk
                                                                            : mChunkSize) };

    // Adjust the TransactionId if this is not the first chunk.
    if (chunk != 0)
    {
      tx.setTransactionId(TransactionId::withValidStart(
        firstTransactionId.getAccountId(),
        firstTransactionId.getValidTransactionTime() +
          std::chrono::duration<int, std::chrono::system_clock::duration::period>(chunk)));
    }

    // Get the transaction ready to execute.
    tx.freezeWith(&client);

    // Grab the first transaction if this is the first transaction.
    if (chunk == 0)
    {
      firstTransactionId = tx.getTransactionId();
    }

    // Do any needed post-chunk processing on the transaction. This is required to happen after freezing so that
    // TopicMessageSubmitTransaction can read the transaction ID.
    tx.onChunk(firstTransactionId, chunk, requiredChunks);

    // Execute the chunk.
    responses.push_back(tx.wrappedExecute(client, timeout));

    // Wait for the transaction to fully complete, if configured
    if (mShouldGetReceipt)
    {
      const TransactionReceipt txReceipt = responses.back().getReceipt(client);
    }
  }

  // Move the data back into mData.
  mData = std::move(data);

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
//-----
template<typename SdkRequestType>
TransactionResponse ChunkedTransaction<SdkRequestType>::wrappedExecute(const Client& client,
                                                                       const std::chrono::duration<double>& timeout)
{
  return Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::execute(
    client, timeout);
}

/**
 * Explicit template instantiations.
 */
template class ChunkedTransaction<FileAppendTransaction>;
template class ChunkedTransaction<TopicMessageSubmitTransaction>;

} // namespace Hedera
