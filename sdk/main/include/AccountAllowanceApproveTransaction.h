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
#ifndef HEDERA_SDK_CPP_ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION_H_
#define HEDERA_SDK_CPP_ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION_H_

#include "HbarAllowance.h"
#include "TokenAllowance.h"
#include "TokenNftAllowance.h"
#include "Transaction.h"

#include <vector>

namespace proto
{
class CryptoApproveAllowanceTransactionBody;
class TransactionBody;
}

namespace Hedera
{
class AccountId;
class Hbar;
class NftId;
class TokenId;
}

namespace Hedera
{
/**
 * A transaction that allows a token owner to delegate a token spender to spend the specified token amount on behalf of
 * the token owner. An owner can provide a token allowance for HBARs, non-fungible and fungible tokens. The owner is the
 * account that owns the tokens and grants the allowance to the spender. The spender is the account that spends tokens
 * authorized by the owner from the owners account. The spender pays for the transaction fees when transferring tokens
 * from the owners account to another recipient.
 *
 * The total number of approvals in this transaction cannot exceed 20. Note that each NFT serial number counts as a
 * single approval, hence a transaction granting 20 serial numbers to a spender will use all of the approvals permitted
 * for the transaction.
 *
 * A single NFT serial number can only be granted to one spender at a time. If an approval assigns a previously approved
 * NFT serial number to a new user, the old user will have their approval removed.
 *
 * Each account is limited to 100 allowances. This limit spans HBAR and fungible token allowances and non-fungible token
 * approved_for_all grants. There is no limit on the number of NFT serial number approvals an owner may grant.
 *
 * The number of allowances set on an account will increase the auto-renewal fee for the account. Conversely, removing
 * allowances will decrease the auto-renewal fee for the account.
 *
 * To decrease the allowance for a given spender, you will need to set the amount to the value you would like to
 * authorize the spender account for. If the spender account was authorized to spend 25 HBARs and the owner now wants to
 * modify their allowance to 5 HBARs, the owner would submit the AccountAllowanceApproveTransaction for 5 HBARs.
 *
 * Only when a spender is set on an explicit NFT ID of a token, do we return the spender ID in TokenNftInfoQuery for the
 * respective NFT. If approveTokenNftAllowanceAllSerials is used to approve all NFTs for a given token class and no NFT
 * ID is specified, we will not return a spender ID for all the serial numbers of that token.
 */
class AccountAllowanceApproveTransaction : public Transaction<AccountAllowanceApproveTransaction>
{
public:
  AccountAllowanceApproveTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a CryptoApproveAllowance transaction.
   */
  explicit AccountAllowanceApproveTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Add an Hbar allowance to this AccountAllowanceApproveTransaction.
   *
   * @param ownerAccountId   The ID of the account that is allowing the spending of its Hbar.
   * @param spenderAccountId The ID of the account that is being allowed to spend the owning account's Hbar.
   * @param amount           The amount of Hbar that is being approved to spend. This amount must be positive.
   * @return A reference to this AccountAllowanceApproveTransaction object with the added Hbar allowance.
   * @throws std::invalid_argument If the amount of Hbar is negative.
   * @throws IllegalStateException If this AccountAllowanceApproveTransaction is frozen.
   */
  AccountAllowanceApproveTransaction& approveHbarAllowance(const AccountId& ownerAccountId,
                                                           const AccountId& spenderAccountId,
                                                           const Hbar& amount);

  /**
   * Add a token allowance to this AccountAllowanceApproveTransaction.
   *
   * @param tokenId          The ID of the token of which to approve the spending.
   * @param ownerAccountId   The ID of the account that is allowing the spending of its tokens.
   * @param spenderAccountId The ID of the account that is being allowed to spend the owning account's tokens.
   * @param amount           The amount of the token that is being approved to spend.
   * @return A reference to this AccountAllowanceApproveTransaction object with the added token allowance.
   * @throws IllegalStateException If this AccountAllowanceApproveTransaction is frozen.
   */
  AccountAllowanceApproveTransaction& approveTokenAllowance(const TokenId& tokenId,
                                                            const AccountId& ownerAccountId,
                                                            const AccountId& spenderAccountId,
                                                            const uint64_t& amount);

  /**
   * Add an NFT allowance to this AccountAllowanceApproveTransaction.
   *
   * @param nftId            The ID of the NFT of which to approve the spending.
   * @param ownerAccountId   The ID of the account that is allowing the spending of the NFT.
   * @param spenderAccountId The ID of the account that is being allowed to spend the owning account's NFT.
   * @return A reference to this AccountAllowanceApproveTransaction object with the added NFT allowance.
   * @throws IllegalStateException If this AccountAllowanceApproveTransaction is frozen.
   */
  AccountAllowanceApproveTransaction& approveTokenNftAllowance(const NftId& nftId,
                                                               const AccountId& ownerAccountId,
                                                               const AccountId& spenderAccountId);

  /**
   * Add an allowance for all NFTs of a specific token ID to this AccountAllowanceApproveTransaction.
   *
   * @param tokenId          The ID of the NFT tokens of which to approve the spending.
   * @param ownerAccountId   The ID of the account that is allowing the spending of its NFTs.
   * @param spenderAccountId The ID of the account that is being allowed to spend the owning account's NFTs.
   * @return A reference to this AccountAllowanceApproveTransaction object with the added NFT allowances.
   * @throws IllegalStateException If this AccountAllowanceApproveTransaction is frozen.
   */
  AccountAllowanceApproveTransaction& approveNftAllowanceAllSerials(const TokenId& tokenId,
                                                                    const AccountId& ownerAccountId,
                                                                    const AccountId& spenderAccountId);

  /**
   * Get the list of Hbar allowance approvals added to this AccountAllowanceApproveTransaction.
   *
   * @return The list of Hbar allowance approvals added to this AccountAllowanceApproveTransaction.
   */
  [[nodiscard]] inline std::vector<HbarAllowance> getHbarApprovals() const { return mHbarAllowances; }

  /**
   * Get the list of fungible token allowance approvals added to this AccountAllowanceApproveTransaction.
   *
   * @return The list of fungible token allowance approvals added to this AccountAllowanceApproveTransaction.
   */
  [[nodiscard]] inline std::vector<TokenAllowance> getTokenApprovals() const { return mTokenAllowances; }

  /**
   * Extract the list of NFT allowance approvals.
   *
   * @return The list of NFT allowance approvals.
   */
  [[nodiscard]] inline std::vector<TokenNftAllowance> getNftApprovals() const { return mNftAllowances; }

private:
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this AccountAllowanceApproveTransaction
   * object.
   *
   * @param client The Client trying to construct this AccountAllowanceApproveTransaction.
   * @param node   The Node to which this AccountAllowanceApproveTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this AccountAllowanceApproveTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                AccountAllowanceApproveTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this AccountAllowanceApproveTransaction to a Node.
   *
   * @param client   The Client submitting this AccountAllowanceApproveTransaction.
   * @param deadline The deadline for submitting this AccountAllowanceApproveTransaction.
   * @param node     Pointer to the Node to which this AccountAllowanceApproveTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;

  /**
   * Build a CryptoApproveAllowanceTransactionBody protobuf object from this AccountAllowanceApproveTransaction object.
   *
   * @return A pointer to a CryptoApproveAllowanceTransactionBody protobuf object filled with this
   *         AccountAllowanceApproveTransaction object's data.
   */
  [[nodiscard]] proto::CryptoApproveAllowanceTransactionBody* build() const;

  /**
   * The list of Hbar allowances to be approved.
   */
  std::vector<HbarAllowance> mHbarAllowances;

  /**
   * The list of fungible token allowances to be approved.
   */
  std::vector<TokenAllowance> mTokenAllowances;

  /**
   * The list of NFT allowances to be approved.
   */
  std::vector<TokenNftAllowance> mNftAllowances;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION_H_
