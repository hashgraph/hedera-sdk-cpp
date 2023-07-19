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
#ifndef HEDERA_SDK_CPP_TRANSACTION_RECEIPT_H_
#define HEDERA_SDK_CPP_TRANSACTION_RECEIPT_H_

#include "AccountId.h"
#include "ContractId.h"
#include "ExchangeRateSet.h"
#include "FileId.h"
#include "Status.h"
#include "TokenId.h"
#include "TopicId.h"

#include <optional>
#include <vector>

namespace proto
{
class TransactionReceipt;
}

namespace Hedera
{
/**
 * The summary of a transaction's result so far. If the transaction has not reached consensus, this result will be
 * necessarily incomplete.
 */
class TransactionReceipt
{
public:
  /**
   * Construct a TransactionReceipt object from a TransactionReceipt protobuf object.
   *
   * @param proto The TransactionReceipt protobuf object from which to construct an TransactionReceipt object.
   * @return The constructed TransactionReceipt object.
   */
  [[nodiscard]] static TransactionReceipt fromProtobuf(const proto::TransactionReceipt& proto);

  /**
   * Validate the status and throw if it is not a Status::SUCCESS.
   *
   * @throws ReceiptStatusException If the status is not a Status::SUCCESS.
   */
  void validateStatus() const;

  /**
   * The consensus status of the transaction; is UNKNOWN if consensus has not been reached, or if the associated
   * transaction did not have a valid payer signature.
   */
  Status mStatus = Status::UNKNOWN;

  /**
   * In the receipt of a CryptoCreate, the ID of the newly created account.
   */
  std::optional<AccountId> mAccountId;

  /**
   * In the receipt of a FileCreate, the ID of the newly created file.
   */
  std::optional<FileId> mFileId;

  /**
   * In the receipt of a ContractCreate, the ID of the newly created contract.
   */
  std::optional<ContractId> mContractId;

  /**
   * The exchange rates in effect when the transaction reached consensus.
   */
  std::optional<ExchangeRateSet> mExchangeRates;

  /**
   * In the receipt of a ConsensusCreateTopic, the ID of the newly created topic.
   */
  std::optional<TopicId> mTopicId;

  /**
   * In the receipt of a TokenCreate, the ID of the newly created token.
   */
  std::optional<TokenId> mTokenId;

  /**
   * In the receipt of a TokenMint, TokenWipe, TokenBurn. For FUNGIBLE_COMMON tokens, this is the current total supply
   * of the token. For NON_FUNGIBLE_UNIQUE tokens, this is the total number of NFTs issued for a given token ID.
   */
  std::optional<uint64_t> mNewTotalSupply;

  /**
   * In the receipt of a TokenMint, for tokens of type NON_FUNGIBLE_COMMON, these are the serial numbers of the
   * newly-created NFTs.
   */
  std::vector<uint64_t> mSerialNumbers;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TRANSACTION_RECEIPT_H_
