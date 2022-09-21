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
#ifndef ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION_H_
#define ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION_H_

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

class AccountId;
class Hbar;
class HbarAllowance;
class NftAllowance;
class NftId;
class TokenAllowance;
class TokenId;
class TransactionId;
}

namespace Hedera
{
/**
 * This transaction type is for approving account allowance.
 */
class AccountAllowanceApproveTransaction
  : public Transaction<AccountAllowanceApproveTransaction>
{
public:
  /**
   * Default constructor.
   */
  AccountAllowanceApproveTransaction();

  /**
   * Construct from a map of transaction ID's to their corresponding account
   * ID's and protobuf transactions.
   *
   * @param transactions Map of transaction IDs to their corresponding account
   *                     ID's and protobuf transactions.
   */
  explicit AccountAllowanceApproveTransaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Construct from a protobuf transaction object.
   *
   * @param transaction The protobuf transaction object from which to construct
   *                    this transaction.
   */
  explicit AccountAllowanceApproveTransaction(
    const proto::TransactionBody& transaction);

  /**
   * Approve an hbar allowance.
   *
   * @param ownerAccountId   The owner's account ID. Can be null if there is no
   *                         owner.
   * @param spenderAccountId The spender's account ID.
   * @param amount           The amount of hbar to add. This amount must be
   *                         positive.
   * @return Reference to this AccountAllowanceApproveTransaction object
   */
  AccountAllowanceApproveTransaction& approveHbarAllowance(
    const InitType<AccountId>& ownerAccountId,
    const AccountId& spenderAccountId,
    const Hbar& amount);

  /**
   * Approve a token allowance.
   *
   * @param tokenId          The token's ID.
   * @param ownerAccountId   The owner's account ID. Can be null if there is no
   *                         owner.
   * @param spenderAccountId The spender's account ID.
   * @param amount           The amount of tokens.
   * @return Reference to this AccountAllowanceApproveTransaction object
   */
  AccountAllowanceApproveTransaction& approveTokenAllowance(
    const TokenId& tokenId,
    const InitType<AccountId>& ownerAccountId,
    const AccountId& spenderAccountId,
    const int64_t& amount);

  /**
   * Approves the NFT allowance.
   *
   * @param nftId            The NFT's ID.
   * @param ownerAccountId   The owner's account ID. Can be null if there is no
   *                         owner.
   * @param spenderAccountId The spender's account ID.
   * @return Reference to this AccountAllowanceApproveTransaction object
   */
  AccountAllowanceApproveTransaction& approveNftAllowance(
    const NftId& nftId,
    const InitType<AccountId>& ownerAccountId,
    const AccountId& spenderAccountId);

  /**
   * Approve the NFT allowance on all serials for a specific token.
   *
   * @param tokenId          The token's ID.
   * @param ownerAccountId   The owner's account ID. Can be null if there is no
   *                         owner.
   * @param spenderAccountId The spender's account ID.
   * @return Reference to this AccountAllowanceApproveTransaction object
   */
  AccountAllowanceApproveTransaction& approveNftAllowanceAllSerials(
    const TokenId& tokenId,
    const InitType<AccountId>& ownerAccountId,
    const AccountId& spenderAccountId);

  /**
   * Extract the list of Hbar allowance approvals.
   *
   * @return The list of Hbar allowance approvals.
   */
  inline std::vector<HbarAllowance> getHbarApprovals() const
  {
    return mHbarAllowances;
  }

  /**
   * Extract the list of token allowance approvals.
   *
   * @return The list of token allowance approvals.
   */
  inline std::vector<TokenAllowance> getTokenApprovals() const
  {
    return mTokenAllowances;
  }

  /**
   * Extract the list of token NFT allowance approvals.
   *
   * @return The list of token NFT allowance approvals.
   */
  inline std::vector<NftAllowance> getTokenNftApprovals() const
  {
    return mNftAllowances;
  }

private:
  /**
   * Initialize this object from a protobuf transaction body.
   */
  void initFromTransactionBody();

  /**
   * Return the NFT serial numbers for a specific token.
   *
   * @param tokenId          The token's ID.
   * @param ownerAccountId   The owner's account ID. Can be null if there is no
   *                         owner.
   * @param spenderAccountId The spender's account ID.
   * @return The list of NFT serial numbers for the input token.
   */
  std::vector<int64_t> getNftSerials(const TokenId& tokenId,
                                     const InitType<AccountId>& ownerAccountId,
                                     const AccountId& spenderAccountId);

  /**
   * Save an NFT serial number for a specific token.
   *
   * @param serial           The serial number to save.
   * @param tokenId          The token's ID.
   * @param ownerAccountId   The owner's account ID. Can be null if there is no
   *                         owner.
   * @param spenderAccountId The spender's account ID.
   */
  void saveNftSerial(const int64_t& serial,
                     const TokenId& tokenId,
                     const InitType<AccountId>& ownerAccountId,
                     const AccountId& spenderAccountId);

  /**
   * Save all NFT serial numbers for a specific token.
   *
   * @param tokenId          The token's ID.
   * @param ownerAccountId   The owner's account ID. Can be null if there is no
   *                         owner.
   * @param spenderAccountId The spender's account ID.
   */
  void saveAllNftSerials(const TokenId& tokenId,
                         const InitType<AccountId>& ownerAccountId,
                         const AccountId& spenderAccountId);

  /**
   * Get the key into the NFT map.
   *
   * @param ownerAccountId   The owner's account ID. Can be null if there is no
   *                         owner.
   * @param spenderAccountId The spender's account ID.
   * @param tokenId          The token's ID.
   */
  std::string getNftMapKey(const InitType<AccountId>& ownerAccountId,
                           const AccountId& spenderAccountId,
                           const TokenId& tokenId) const;

  /**
   * A list of added Hbar allowances.
   */
  std::vector<HbarAllowance> mHbarAllowances;

  /**
   * A list of added token allowances.
   */
  std::vector<TokenAllowance> mTokenAllowances;

  /**
   * A list of added NFT allowances.
   */
  std::vector<NftAllowance> mNftAllowances;

  /**
   * A map of NFT information.
   * Key is "{ownerId}:{spenderId}:{tokenId}". OwnerId may be "FEE_PAYER" if the
   * owner is not valid. The value is the index into the mNftAllowances list.
   */
  std::unordered_map<std::string, size_t> mNftMap;
};

} // namespace Hedera

#endif // ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION_H_
