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
#ifndef HEDERA_SDK_CPP_CHUNKED_TRANSACTION_H_
#define HEDERA_SDK_CPP_CHUNKED_TRANSACTION_H_

#include "Defaults.h"
#include "Transaction.h"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

namespace Hedera
{
class Client;
class TransactionResponse;
}

namespace Hedera
{
/**
 * Base class for all transactions that require being divided into chunks.
 *
 * @tparam SdkRequestType The SDK request type.
 */
template<typename SdkRequestType>
class ChunkedTransaction : public Transaction<SdkRequestType>
{
public:
  /**
   * Derived from Executable. Execute this ChunkedTransaction. This should NOT be used if multiple chunks are being sent
   * (i.e. data.size() > mMaxChunkSize); opt to use `executeAll()` instead.
   *
   * @param client The Client to use to submit this ChunkedTransaction.
   * @return The TransactionResponse object sent from the Hedera network that contains the result of the first chunked
   *         request.
   * @throws MaxAttemptsExceededException If this Executable attempts to execute past the number of allowable attempts.
   * @throws PrecheckStatusException      If this Executable fails its pre-check.
   * @throws UninitializedException       If the input Client has not yet been initialized.
   */
  TransactionResponse execute(const Client& client) override;

  /**
   * Derived from Executable. Execute this ChunkedTransaction with a specific timeout. This should NOT be used if
   * multiple chunks are being sent (i.e. data.size() > mMaxChunkSize); opt to use `executeAll()` instead.
   *
   * @param client The Client to use to submit this ChunkedTransaction.
   * @param timeout The desired timeout for the execution of this ChunkedTransaction.
   * @return The TransactionResponse object sent from the Hedera network that contains the result of the first chunked
   *         request.
   * @throws MaxAttemptsExceededException If this Executable attempts to execute past the number of allowable attempts.
   * @throws PrecheckStatusException      If this Executable fails its pre-check.
   * @throws UninitializedException       If the input Client has not yet been initialized.
   */
  TransactionResponse execute(const Client& client, const std::chrono::duration<double>& timeout) override;

  /**
   * Execute all chunks of this ChunkedTransaction.
   *
   * @param client The Client to use to submit this ChunkedTransaction.
   * @return The list of TransactionResponse objects sent from the Hedera network that contains the result of the
   *         requests.
   * @throws MaxAttemptsExceededException If this Executable attempts to execute past the number of allowable attempts.
   * @throws PrecheckStatusException      If this Executable fails its pre-check.
   * @throws UninitializedException       If the input Client has not yet been initialized.
   */
  std::vector<TransactionResponse> executeAll(const Client& client);

  /**
   * Execute all chunks of this ChunkedTransaction with a specific timeout.
   *
   * @param client The Client to use to submit this ChunkedTransaction.
   * @param timeout The desired timeout for the execution of this ChunkedTransaction.
   * @return The list of TransactionResponse objects sent from the Hedera network that contains the result of the
   *         requests.
   * @throws MaxAttemptsExceededException If this Executable attempts to execute past the number of allowable attempts.
   * @throws PrecheckStatusException      If this Executable fails its pre-check.
   * @throws UninitializedException       If the input Client has not yet been initialized.
   */
  std::vector<TransactionResponse> executeAll(const Client& client, const std::chrono::duration<double>& timeout);

  /**
   * Set the maximum number of chunks for this ChunkedTransaction.
   *
   * @param chunks The maximum number of chunks for this ChunkedTransaction.
   * @return A reference to this derived ChunkedTransaction object with the newly-set chunk maximum.
   */
  SdkRequestType& setMaxChunks(unsigned int chunks);

  /**
   * Set the size of each chunk, in bytes, for this ChunkedTransaction.
   *
   * @param size The size of each chunk, in bytes, for this ChunkedTransaction.
   * @return A reference to this derived ChunkedTransaction object with the newly-set chunk size.
   */
  SdkRequestType& setChunkSize(unsigned int size);

  /**
   * Get the maximum number of chunks for this ChunkedTransaction.
   *
   * @return The maximum number of chunks for this ChunkedTransaction.
   */
  [[nodiscard]] inline unsigned int getMaxChunks() const { return mMaxChunks; }

  /**
   * Get the size of each chunk, in bytes, for this ChunkedTransaction.
   *
   * @return The size of each chunk, in bytes, for this ChunkedTransaction.
   */
  [[nodiscard]] inline unsigned int getChunkSize() const { return mChunkSize; }

protected:
  /**
   * Create a chunk of this ChunkedTransaction to send.
   *
   * @param data  The whole entirety of data to be sent across all chunks.
   * @param chunk The chunk number to create.
   * @param total The total number of chunks to create.
   */
  virtual void createChunk(const std::vector<std::byte>& data, int32_t chunk, [[maybe_unused]] int32_t total);

  /**
   * Set the data for this ChunkedTransaction.
   *
   * @param data The data for this ChunkedTransaction.
   * @return A reference to this derived ChunkedTransaction object with the newly-set data.
   */
  SdkRequestType& setData(const std::vector<std::byte>& data);
  SdkRequestType& setData(std::string_view data);

  /**
   * Get the data for this ChunkedTransaction.
   *
   * @return The data for this ChunkedTransaction.
   */
  [[nodiscard]] inline std::vector<std::byte> getData() const { return mData; }

  /**
   * Set the receipt retrieval policy for this ChunkedTransaction.
   *
   * @param retrieveReceipt \c TRUE if this ChunkedTransaction should retrieve a receipt after each submitted chunk,
   *                        otherwise \c FALSE.
   */
  void setShouldGetReceipt(bool retrieveReceipt);

  /**
   * Get the receipt retrieval policy for this ChunkedTransaction.
   *
   * @param retrieveReceipt \c TRUE if this ChunkedTransaction should retrieve a receipt after each submitted chunk,
   *                        otherwise \c FALSE.
   */
  [[nodiscard]] inline bool getShouldGetReceipt() const { return mShouldGetReceipt; }

private:
  /**
   * This ChunkedTransaction's data.
   */
  std::vector<std::byte> mData;

  /**
   * The size of this ChunkedTransaction's chunks, in bytes.
   */
  unsigned int mChunkSize = 1024U;

  /**
   * The maximum number of chunks into which this ChunkedTransaction will get broken up.
   */
  unsigned int mMaxChunks = DEFAULT_MAX_CHUNKS;

  /**
   * Should this ChunkedTransaction get a receipt for each submitted chunk?
   */
  bool mShouldGetReceipt = false;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CHUNKED_TRANSACTION_H_
