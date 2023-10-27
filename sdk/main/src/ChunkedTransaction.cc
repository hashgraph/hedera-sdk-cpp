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
#include "ChunkedTransaction.h"
#include "Client.h"
#include "FileAppendTransaction.h"
#include "TopicMessageSubmitTransaction.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/IllegalStateException.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"
#include "impl/openssl_utils/OpenSSLUtils.h"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <proto/transaction.pb.h>
#include <proto/transaction_contents.pb.h>

namespace Hedera
{
//-----
template<typename SdkRequestType>
struct ChunkedTransaction<SdkRequestType>::ChunkedTransactionImpl
{
  // The TransactionIds of all chunks that are to be sent as part of this ChunkedTransaction, minus the first chunk
  // (which is stored in Transaction<SdkRequestType>::mTransactionId).
  std::vector<TransactionId> mChunkedTransactionIds;

  // This ChunkedTransaction's data.
  std::vector<std::byte> mData;

  // The size of this ChunkedTransaction's chunks, in bytes.
  unsigned int mChunkSize = DEFAULT_CHUNK_SIZE;

  // The maximum number of chunks into which this ChunkedTransaction will get broken up.
  unsigned int mMaxChunks = DEFAULT_MAX_CHUNKS;

  // Should this ChunkedTransaction get a receipt for each submitted chunk?
  bool mShouldGetReceipt = false;

  // The current chunk attempting to be sent.
  unsigned int mCurrentChunk = 0U;
};

//-----
template<typename SdkRequestType>
TransactionResponse ChunkedTransaction<SdkRequestType>::execute(const Client& client)
{
  return executeAll(client).at(0);
}

//-----
template<typename SdkRequestType>
TransactionResponse ChunkedTransaction<SdkRequestType>::execute(const Client& client,
                                                                const std::chrono::system_clock::duration& timeout)
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
  const std::chrono::system_clock::duration& timeout)
{
  // Determine how many chunks are going to be required to send this whole ChunkedTransaction and make sure it's within
  // the set limit.
  const unsigned int requiredChunks = getNumberOfChunksRequired();
  if (requiredChunks > mImpl->mMaxChunks)
  {
    throw IllegalStateException("Transaction requires " + std::to_string(requiredChunks) + " but is only allotted " +
                                std::to_string(mImpl->mMaxChunks) + ". Try using setMaxChunks()");
  }

  // Container to hold responses.
  std::vector<TransactionResponse> responses;
  responses.reserve(requiredChunks);

  for (; mImpl->mCurrentChunk < requiredChunks; ++mImpl->mCurrentChunk)
  {
    responses.push_back(
      Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::execute(
        client, timeout));

    if (mImpl->mShouldGetReceipt)
    {
      const TransactionReceipt txReceipt = responses.back().getReceipt(client, timeout);
    }
  }

  // Reset current chunk.
  mImpl->mCurrentChunk = 0U;

  return responses;
}

//-----
template<typename SdkRequestType>
SdkRequestType& ChunkedTransaction<SdkRequestType>::addSignature(const std::shared_ptr<PublicKey>& publicKey,
                                                                 const std::vector<std::byte>& signature)
{
  if (mImpl->mData.size() > mImpl->mChunkSize)
  {
    throw IllegalStateException(
      "Cannot manually add a signature to a ChunkedTransaction with data length greater than " +
      std::to_string(mImpl->mChunkSize));
  }

  return Transaction<SdkRequestType>::addSignature(publicKey, signature);
}

//-----
template<typename SdkRequestType>
std::map<AccountId, std::map<std::shared_ptr<PublicKey>, std::vector<std::byte>>>
ChunkedTransaction<SdkRequestType>::getSignatures() const
{
  if (mImpl->mData.size() > mImpl->mChunkSize)
  {
    throw IllegalStateException("Cannot get signatures for a ChunkedTransaction with data length greater than " +
                                std::to_string(mImpl->mChunkSize) + ". Try calling getAllSignatures() instead.");
  }

  return Transaction<SdkRequestType>::getSignatures();
}

//-----
template<typename SdkRequestType>
std::vector<std::map<AccountId, std::map<std::shared_ptr<PublicKey>, std::vector<std::byte>>>>
ChunkedTransaction<SdkRequestType>::getAllSignatures() const
{
  // Grab the signatures of the first chunk. If there are no signatures, there are no signers.
  std::vector<std::map<AccountId, std::map<std::shared_ptr<PublicKey>, std::vector<std::byte>>>> signatures;
  signatures.push_back(Transaction<SdkRequestType>::getSignatures());

  if (signatures.cbegin()->empty())
  {
    return {};
  }

  // All Transaction protobuf objects get built in Transaction<SdkRequestType>::getSignatures(), so the signatures just
  // need to be grabbed for each transaction chunk. The first transaction chunk is already in the signatures map, so
  // start at index 1.
  for (size_t i = 1; i < mImpl->mChunkedTransactionIds.size(); ++i)
  {
    signatures.push_back(Transaction<SdkRequestType>::getSignaturesInternal(i));
  }

  return signatures;
}

//-----
template<typename SdkRequestType>
std::vector<std::byte> ChunkedTransaction<SdkRequestType>::getTransactionHash() const
{
  if (!mImpl->mChunkedTransactionIds.empty())
  {
    throw IllegalStateException(
      "A single hash cannot be generated for this transaction, try calling 'getAllTransactionHashesPerNode()'");
  }

  return Transaction<SdkRequestType>::getTransactionHash();
}

//-----
template<typename SdkRequestType>
std::map<AccountId, std::vector<std::byte>> ChunkedTransaction<SdkRequestType>::getTransactionHashPerNode() const
{
  if (!mImpl->mChunkedTransactionIds.empty())
  {
    throw IllegalStateException(
      "A single hash cannot be generated for this transaction, try calling 'getAllTransactionHashesPerNode()'");
  }

  return Transaction<SdkRequestType>::getTransactionHashPerNode();
}

//-----
template<typename SdkRequestType>
std::vector<std::map<AccountId, std::vector<std::byte>>>
ChunkedTransaction<SdkRequestType>::getAllTransactionHashesPerNode() const
{
  if (!Transaction<SdkRequestType>::isFrozen())
  {
    throw IllegalStateException("Transaction must be frozen in order to calculate the hashes.");
  }

  // Build all the Transaction protobuf objects.
  Transaction<SdkRequestType>::buildAllTransactions();

  // Grab the node account IDs being used for this ChunkedTransaction.
  const std::vector<AccountId> nodeAccountIds =
    Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
      getNodeAccountIds();

  // Add one to the mChunkedTransactionIds size to include the first TransactionId stored in the Transaction base class.
  std::vector<std::map<AccountId, std::vector<std::byte>>> hashes;
  for (unsigned int i = 0; i < mImpl->mChunkedTransactionIds.size() + 1; ++i)
  {
    // For each node account ID, grab the correct Transaction protobuf object and generate the hash.
    std::map<AccountId, std::vector<std::byte>> hashMap;
    for (unsigned int j = 0; j < nodeAccountIds.size(); ++j)
    {
      hashMap.emplace(nodeAccountIds.at(j),
                      internal::OpenSSLUtils::computeSHA384(internal::Utilities::stringToByteVector(
                        Transaction<SdkRequestType>::getTransactionProtobufObject(
                          (i * static_cast<unsigned int>(nodeAccountIds.size())) + j)
                          .signedtransactionbytes())));
    }

    hashes.push_back(hashMap);
  }

  return hashes;
}

//-----
template<typename SdkRequestType>
SdkRequestType& ChunkedTransaction<SdkRequestType>::setMaxChunks(unsigned int chunks)
{
  Transaction<SdkRequestType>::requireNotFrozen();
  mImpl->mMaxChunks = chunks;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& ChunkedTransaction<SdkRequestType>::setChunkSize(unsigned int size)
{
  Transaction<SdkRequestType>::requireNotFrozen();
  mImpl->mChunkSize = size;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
unsigned int ChunkedTransaction<SdkRequestType>::getMaxChunks() const
{
  return mImpl->mMaxChunks;
}

//-----
template<typename SdkRequestType>
unsigned int ChunkedTransaction<SdkRequestType>::getChunkSize() const
{
  return mImpl->mChunkSize;
}

//-----
template<typename SdkRequestType>
ChunkedTransaction<SdkRequestType>::ChunkedTransaction()
  : Transaction<SdkRequestType>()
  , mImpl(std::make_unique<ChunkedTransactionImpl>())
{
}

//-----
template<typename SdkRequestType>
ChunkedTransaction<SdkRequestType>::~ChunkedTransaction() = default;

//-----
template<typename SdkRequestType>
ChunkedTransaction<SdkRequestType>::ChunkedTransaction(const ChunkedTransaction& other)
  : Transaction<SdkRequestType>(other)
  , mImpl(std::make_unique<ChunkedTransactionImpl>(*other.mImpl))
{
}

//-----
template<typename SdkRequestType>
ChunkedTransaction<SdkRequestType>& ChunkedTransaction<SdkRequestType>::operator=(const ChunkedTransaction& other)
{
  if (this != &other)
  {
    Transaction<SdkRequestType>::operator=(other);
    mImpl = std::make_unique<ChunkedTransactionImpl>(*other.mImpl);
  }

  return *this;
}

//-----
template<typename SdkRequestType>
ChunkedTransaction<SdkRequestType>::ChunkedTransaction(ChunkedTransaction&& other) noexcept
  : Transaction<SdkRequestType>(std::move(other))
  , mImpl(std::move(other.mImpl)) // NOLINT
{
}

//-----
template<typename SdkRequestType>
ChunkedTransaction<SdkRequestType>& ChunkedTransaction<SdkRequestType>::operator=(ChunkedTransaction&& other) noexcept
{
  if (this != &other)
  {
    Transaction<SdkRequestType>::operator=(std::move(other));
    mImpl = std::move(other.mImpl); // NOLINT
  }

  return *this;
}

//-----
template<typename SdkRequestType>
ChunkedTransaction<SdkRequestType>::ChunkedTransaction(const proto::TransactionBody& txBody)
  : Transaction<SdkRequestType>(txBody)
  , mImpl(std::make_unique<ChunkedTransactionImpl>())
{
}

//-----
template<typename SdkRequestType>
ChunkedTransaction<SdkRequestType>::ChunkedTransaction(
  const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions)
  : Transaction<SdkRequestType>(transactions)
  , mImpl(std::make_unique<ChunkedTransactionImpl>())
{
  // Make sure there's more than one chunk.
  if (transactions.size() <= 1)
  {
    return;
  }

  // Go through each additional TransactionId and store its information.
  auto iter = transactions.cbegin();
  std::advance(iter, 1);

  for (; iter != transactions.cend(); ++iter)
  {
    // Save the TransactionId.
    mImpl->mChunkedTransactionIds.push_back(iter->first);

    // Add each Transaction protobuf object to this ChunkedTransaction's Transaction protobuf object list. The node
    // account IDs should be in the same order as each other for each TransactionId.
    for (const auto& [accountId, transaction] : iter->second)
    {
      Transaction<SdkRequestType>::addTransaction(transaction);
    }
  }
}

//-----
template<typename SdkRequestType>
SdkRequestType& ChunkedTransaction<SdkRequestType>::setData(const std::vector<std::byte>& data)
{
  mImpl->mData = data;
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
std::vector<std::byte> ChunkedTransaction<SdkRequestType>::getData() const
{
  return mImpl->mData;
}

//-----
template<typename SdkRequestType>
std::vector<std::byte> ChunkedTransaction<SdkRequestType>::getDataForChunk(unsigned int chunk) const
{
  const unsigned int startingByteForChunk = mImpl->mChunkSize * chunk;
  const auto start = mImpl->mData.cbegin() + startingByteForChunk;
  return { start,
           start + ((mImpl->mChunkSize + startingByteForChunk > mImpl->mData.size())
                      ? mImpl->mData.size() - startingByteForChunk
                      : mImpl->mChunkSize) };
}

//-----
template<typename SdkRequestType>
void ChunkedTransaction<SdkRequestType>::setShouldGetReceipt(bool retrieveReceipt)
{
  mImpl->mShouldGetReceipt = retrieveReceipt;
}

//-----
template<typename SdkRequestType>
bool ChunkedTransaction<SdkRequestType>::getShouldGetReceipt() const
{
  return mImpl->mShouldGetReceipt;
}

//-----
template<typename SdkRequestType>
proto::Transaction ChunkedTransaction<SdkRequestType>::makeRequest(unsigned int attempt) const
{
  // Adjust the index to account for the current chunk.
  return Transaction<SdkRequestType>::makeRequest(
    static_cast<unsigned int>(
      mImpl->mCurrentChunk *
      Executable<SdkRequestType, proto::Transaction, proto::TransactionResponse, TransactionResponse>::
        getNodeAccountIds()
          .size()) +
    attempt);
}

//-----
template<typename SdkRequestType>
void ChunkedTransaction<SdkRequestType>::generateSignedTransactions(const Client* client)
{
  // Update this Transaction's source TransactionBody protobuf object.
  Transaction<SdkRequestType>::updateSourceTransactionBody(client);
  proto::TransactionBody sourceTransactionBody = Transaction<SdkRequestType>::getSourceTransactionBody();

  const unsigned int requiredChunks = getNumberOfChunksRequired();
  for (int i = 0; i < requiredChunks; ++i)
  {
    // Generate a new TransactionId if this isn't the first chunk. Add one to the smallest system clock duration to make
    // cascading transaction IDs.
    if (i > 0)
    {
      mImpl->mChunkedTransactionIds.push_back(TransactionId::withValidStart(
        AccountId::fromProtobuf(sourceTransactionBody.transactionid().accountid()),
        internal::TimestampConverter::fromProtobuf(sourceTransactionBody.transactionid().transactionvalidstart()) +
          std::chrono::system_clock::duration(1)));
      sourceTransactionBody.set_allocated_transactionid(mImpl->mChunkedTransactionIds.back().toProtobuf().release());
    }

    // Generate the chunk and add it to sourceTransactionBody.
    addToChunk(i, requiredChunks, sourceTransactionBody);

    // Create a SignedTransaction protobuf object for each node account ID and add it to this ChunkedTransaction's
    // SignedTransaction protobuf object list.
    Transaction<SdkRequestType>::addSignedTransactionForEachNode(sourceTransactionBody);
  }
}

//-----
template<typename SdkRequestType>
void ChunkedTransaction<SdkRequestType>::clearTransactions()
{
  Transaction<SdkRequestType>::clearTransactions();
  mImpl->mChunkedTransactionIds.clear();
}

//-----
template<typename SdkRequestType>
TransactionId ChunkedTransaction<SdkRequestType>::getCurrentTransactionId() const
{
  return mImpl->mCurrentChunk == 0 ? Transaction<SdkRequestType>::getCurrentTransactionId()
                                   : mImpl->mChunkedTransactionIds.at(mImpl->mCurrentChunk - 1);
}

//-----
template<typename SdkRequestType>
unsigned int ChunkedTransaction<SdkRequestType>::getNumberOfChunksRequired() const
{
  return static_cast<unsigned int>(
    std::ceil(static_cast<double>(mImpl->mData.size()) / static_cast<double>(mImpl->mChunkSize)));
}

/**
 * Explicit template instantiations.
 */
template class ChunkedTransaction<FileAppendTransaction>;
template class ChunkedTransaction<TopicMessageSubmitTransaction>;

} // namespace Hedera
