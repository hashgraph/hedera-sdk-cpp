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
#ifndef ACCOUNT_ALLOWANCE_DELETE_TRANSACTION_H_
#define ACCOUNT_ALLOWANCE_DELETE_TRANSACTION_H_

#include "TokenNftAllowance.h"
#include "Transaction.h"

#include <unordered_map>
#include <vector>

namespace proto
{
class TransactionBody;
}

namespace Hedera
{
template<typename T>
class InitType;

class NftId;
class TokenId;
}

namespace Hedera
{
/**
 * This transaction type is for deleting an account allowance.
 */
class AccountAllowanceDeleteTransaction : public Transaction<AccountAllowanceDeleteTransaction>
{
public:
  /**
   * Default constructor.
   */
  AccountAllowanceDeleteTransaction();

  /**
   * Construct from a map of transaction ID's to their corresponding account
   * ID's and protobuf transactions.
   *
   * @param transactions Map of transaction IDs to their corresponding account
   *                     ID's and protobuf transactions.
   */
  explicit AccountAllowanceDeleteTransaction(
    const std::unordered_map<TransactionId, std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Construct from a protobuf transaction object.
   *
   * @param transaction The protobuf transaction object from which to construct
   *                    this transaction.
   */
  explicit AccountAllowanceDeleteTransaction(const proto::TransactionBody& transaction);

  /**
   * Validate the checksums.
   *
   * @param client The client with which to validate the checksums
   */
  virtual void validateChecksums(const Client& client) const override;

  /**
   * Derived from Transaction. Called in freezeWith(Client) just before the
   * transaction body is built. The intent is for the derived class to assign
   * their data variant to the transaction body.
   *
   * @param body The transaction's body part. (XXX)
   */
  virtual void onFreeze(proto::TransactionBody* body) const override;

  /**
   * Called in schedule() when converting transaction into a scheduled version.
   *
   * @param body The schedulable transaction's body part. (XXX)
   */
  virtual void onScheduled(proto::SchedulableTransactionBody* body) const override;

  /**
   * Build an account allowance delete protobuf message based on the data in
   * this class.
   *
   * @return An account allowance delete protobuf message.
   */
  proto::CryptoDeleteAllowanceTransactionBody* build() const;

  /**
   * Delete all NFT allowances.
   *
   * @param nftId            The token's ID.
   * @param ownerAccountId   The owner's account ID.
   * @return Reference to this AccountAllowanceDeleteTransaction object
   */
  AccountAllowanceDeleteTransaction& deleteAllNftAllowances(const NftId& nftId, const AccountId& ownerAccountId);

  /**
   * Extract the list of token NFT allowance approvals.
   *
   * @return The list of token NFT allowance approvals.
   */
  inline std::vector<NftAllowance> getNftAllowanceDeletions() const { return mNftAllowances; }

private:
  /**
   * Function used to initialize an AccountAllowanceDeleteTransaction object
   * from a protobuf TransactionBody object
   */
  void initFromTransactionBody();

  /**
   * Save an NFT serial number for a specific token.
   *
   * @param serial           The serial number to save.
   * @param tokenId          The token's ID.
   * @param ownerAccountId   The owner's account ID.
   */
  void saveNftSerial(const int64_t& serial, const TokenId& tokenId, const AccountId& ownerAccountId);

  /**
   * A list of added NFT allowances.
   */
  std::vector<NftAllowance> mNftAllowances;

  /**
   * A map of NFT information.
   * Key is "{ownerId}:{tokenId}". The value is the index into the
   * mNftAllowances list.
   */
  std::unordered_map<std::string, size_t> mNftMap;
};

} // namespace Hedera

#endif // ACCOUNT_ALLOWANCE_DELETE_TRANSACTION_H_
