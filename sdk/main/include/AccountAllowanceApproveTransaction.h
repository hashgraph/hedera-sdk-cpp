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

/**
 * Library includes
 */
#include "Transaction.h"

/**
 * STL includes
 */
#include <unordered_map>
#include <vector>

/**
 * Protobuf forward declarations
 */
namespace proto
{
class TransactionBody;
} // namespace proto

/**
 * Hedera forward declarations
 */
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
} // namespace Hedera

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
   * Construct from a map of TransactionId's to their corresponding AccountId's
   * and Transactions
   *
   * @param transactions  Compound list of transaction ID's list of (AccountId,
   *                      TransactionBody) records
   */
  AccountAllowanceApproveTransaction(
    const std::unordered_map<
      TransactionId,
      std::unordered_map<AccountId, proto::TransactionBody>>& transactions);

  /**
   * Constructor from a TransactionBody object
   *
   * @param transaction  protobuf TransactionBody
   */
  AccountAllowanceApproveTransaction(const proto::TransactionBody& transaction);

  /**
   * Approves the Hbar allowance.
   *
   * @param ownerAccountId   The owner's account ID. Can be null if there is no
   *                         owner.
   * @param spenderAccountId The spender's account ID.
   * @param amount           The amount of Hbar to add. This amount must be
   *                         positive.
   * @return Reference to this AccountAllowanceApproveTransaction object
   */
  AccountAllowanceApproveTransaction& approveHbarAllowance(
    const InitType<AccountId>& ownerAccountId,
    const AccountId& spenderAccountId,
    const Hbar& amount);

  /**
   * Approves the token allowance.
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
    const long long& amount);

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
   * Function used to initialize an AccountAllowanceApproveTransaction object
   * from a protobuf TransactionBody object
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
  std::vector<long long> getNftSerials(
    const TokenId& tokenId,
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
  void saveNftSerial(const long long& serial,
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
   * key is "{ownerId}:{spenderId}:{tokenId}". OwnerId may be "FEE_PAYER".
   * The index is the index into the mNftAllowances list.
   */
  std::unordered_map<std::string, size_t> mNftMap;
};

} // namespace Hedera

#endif // ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION_H_
