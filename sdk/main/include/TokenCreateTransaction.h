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
#ifndef HEDERA_SDK_CPP_TOKEN_CREATE_TRANSACTION_H_
#define HEDERA_SDK_CPP_TOKEN_CREATE_TRANSACTION_H_

#include "AccountId.h"
#include "CustomFee.h"
#include "Defaults.h"
#include "Key.h"
#include "TokenSupplyType.h"
#include "TokenType.h"
#include "Transaction.h"

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace proto
{
class TokenCreateTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * Create a new fungible or non-fungible token (NFT) on the Hedera network. After you submit the transaction to the
 * Hedera network, you can obtain the new token ID by requesting the receipt.
 *
 * For non-fungible tokens, the token ID represents an NFT class. Once the token is created, you will have to mint each
 * NFT using the token mint operation.
 *
 * Transaction Signing Requirements:
 *  - Treasury key is required to sign.
 *  - Admin key, if specified.
 *  - Transaction fee payer key.
 */
class TokenCreateTransaction : public Transaction<TokenCreateTransaction>
{
public:
  /**
   * Default constructor. Sets the maximum transaction fee to 40 Hbars.
   */
  TokenCreateTransaction();

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a TokenCreation transaction.
   */
  explicit TokenCreateTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Construct from a map of TransactionIds to node account IDs and their respective Transaction protobuf objects.
   *
   * @param transactions The map of TransactionIds to node account IDs and their respective Transaction protobuf
   *                     objects.
   */
  explicit TokenCreateTransaction(const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions);

  /**
   * Set the desired name for the new token.
   *
   * @param name The desired name for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set token name.
   */
  TokenCreateTransaction& setTokenName(std::string_view name);

  /**
   * Set the desired symbol for the new token.
   *
   * @param symbol The desired symbol for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set token symbol.
   */
  TokenCreateTransaction& setTokenSymbol(std::string_view symbol);

  /**
   * Set the desired number of decimals for the new FUNGIBLE_COMMON token. If the new token is to be of type
   * NON_FUNGIBLE_UNIQUE, this must be 0 (to which it defaults).
   *
   * @param decimals The desired number of decimals for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set number of decimals.
   */
  TokenCreateTransaction& setDecimals(uint32_t decimals);

  /**
   * Set the desired initial supply of FUNGIBLE_COMMON tokens. If the new token is to be of type NON_FUNGIBLE_UNIQUE,
   * this must be 0 (to which it defaults).
   *
   * @param amount The desired initial supply of the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set initial supply.
   */
  TokenCreateTransaction& setInitialSupply(const uint64_t& amount);

  /**
   * Set the desired ID of the treasury account, which will receive the specified initial supply of new tokens.
   *
   * @param accountId The ID of the desired treasury account for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set treasury account ID.
   */
  TokenCreateTransaction& setTreasuryAccountId(const AccountId& accountId);

  /**
   * Set the desired admin key for the new token.
   *
   * @param key The desired admin key for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set admin key.
   */
  TokenCreateTransaction& setAdminKey(const std::shared_ptr<Key>& key);

  /**
   * Set the desired KYC key for the new token.
   *
   * @param key The desired KYC key for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set KYC key.
   */
  TokenCreateTransaction& setKycKey(const std::shared_ptr<Key>& key);

  /**
   * Set the desired freeze key for the new token.
   *
   * @param key The desired freeze key for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set freeze key.
   */
  TokenCreateTransaction& setFreezeKey(const std::shared_ptr<Key>& key);

  /**
   * Set the desired wipe key for the new token.
   *
   * @param key The desired wipe key for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set wipe key.
   */
  TokenCreateTransaction& setWipeKey(const std::shared_ptr<Key>& key);

  /**
   * Set the desired supply key for the new token.
   *
   * @param key The desired supply key for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set supply key.
   */
  TokenCreateTransaction& setSupplyKey(const std::shared_ptr<Key>& key);

  /**
   * Set the desired default freeze status for the new token.
   *
   * @param freeze The desired default freeze status for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set default freeze status.
   */
  TokenCreateTransaction& setFreezeDefault(bool freeze);

  /**
   * Set the desired expiration time for the new token.
   *
   * @param expiration The desired expiration time for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set default expiration time.
   */
  TokenCreateTransaction& setExpirationTime(const std::chrono::system_clock::time_point& expiration);

  /**
   * Set the ID of the desired auto-renew account for the new token.
   *
   * @param accountId The ID of the desired auto-renew account for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set auto-renew account ID.
   */
  TokenCreateTransaction& setAutoRenewAccountId(const AccountId& accountId);

  /**
   * Set the desired auto-renew period for the new token.
   *
   * @param period The desired auto-renew period for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set auto-renew period.
   */
  TokenCreateTransaction& setAutoRenewPeriod(const std::chrono::system_clock::duration& period);

  /**
   * Set the desired memo for the new token.
   *
   * @param memo The desired memo for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set memo.
   */
  TokenCreateTransaction& setTokenMemo(std::string_view memo);

  /**
   * Set the desired type for the new token.
   *
   * @param type The desired type for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set token type.
   */
  TokenCreateTransaction& setTokenType(TokenType type);

  /**
   * Set the desired supply type for the new token.
   *
   * @param type The desired supply type for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set supply type.
   */
  TokenCreateTransaction& setSupplyType(TokenSupplyType type);

  /**
   * Set the desired maximum supply for the new token.
   *
   * @param max The desired maximum supply for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set maximum supply.
   */
  TokenCreateTransaction& setMaxSupply(const uint64_t& max);

  /**
   * Set the desired fee schedule key for the new token.
   *
   * @param key The desired fee schedule key for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set fee schedule key.
   */
  TokenCreateTransaction& setFeeScheduleKey(const std::shared_ptr<Key>& key);

  /**
   * Set the desired custom fees to be assessed during a TransferTransaction that transfers units of the new token.
   *
   * @param fees The desired custom fees to be assessed during a TransferTransaction that transfers units of the new
   *             token.
   * @return A reference to this TokenCreateTransaction with the newly-set fee custom fees.
   */
  TokenCreateTransaction& setCustomFees(const std::vector<std::shared_ptr<CustomFee>>& fees);

  /**
   * Set the desired pause key for the new token.
   *
   * @param key The desired pause key for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set pause key.
   */
  TokenCreateTransaction& setPauseKey(const std::shared_ptr<Key>& key);

  /**
   * Set the desired lock key for the new token.
   *
   * @param key The desired lock key for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set lock key.
   */
  TokenCreateTransaction& setLockKey(const std::shared_ptr<Key>& key);

  /**
   * Set the desired partition key for the new token.
   *
   * @param key The desired partition key for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set partition key.
   */
  TokenCreateTransaction& setPartitionKey(const std::shared_ptr<Key>& key);

  /**
   * Set the desired partition move key for the new token.
   *
   * @param key The desired partition move key for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set partition move key.
   */
  TokenCreateTransaction& setPartitionMoveKey(const std::shared_ptr<Key>& key);

  /**
   * Get the desired name for the new token.
   *
   * @return The desired name for the new token.
   */
  [[nodiscard]] inline std::string getTokenName() const { return mTokenName; }

  /**
   * Get the desired symbol for the new token.
   *
   * @return The desired symbol for the new token.
   */
  [[nodiscard]] inline std::string getTokenSymbol() const { return mTokenSymbol; }

  /**
   * Get the desired number of decimals for the new token.
   *
   * @return The desired number of decimals for the new token.
   */
  [[nodiscard]] inline uint32_t getDecimals() const { return mDecimals; }

  /**
   * Get the desired initial supply of new tokens.
   *
   * @return The desired initial supply of FUNGIBLE_COMMON new tokens.
   */
  [[nodiscard]] inline uint64_t getInitialSupply() const { return mInitialSupply; }

  /**
   * Get the ID of the desired treasury account for the supply of new tokens.
   *
   * @return The ID of the desired treasury account for the supply of new tokens. Returns uninitialized if no treasury
   *         account has been set yet.
   */
  [[nodiscard]] inline std::optional<AccountId> getTreasuryAccountId() const { return mTreasuryAccountId; }

  /**
   * Get the desired admin key for the new token.
   *
   * @return The desired admin key for the new token.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getAdminKey() const { return mAdminKey; }

  /**
   * Get the desired KYC key for the new token.
   *
   * @return The desired KYC key for the new token.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getKycKey() const { return mKycKey; }

  /**
   * Get the desired freeze key for the new token.
   *
   * @return The desired freeze key for the new token.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getFreezeKey() const { return mFreezeKey; }

  /**
   * Get the desired wipe key for the new token.
   *
   * @return The desired wipe key for the new token.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getWipeKey() const { return mWipeKey; }

  /**
   * Get the desired supply key for the new token.
   *
   * @return The desired supply key for the new token.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getSupplyKey() const { return mSupplyKey; }

  /**
   * Get the desired default freeze status for the new token.
   *
   * @return The desired default freeze status for the new token.
   */
  [[nodiscard]] inline bool getFreezeDefault() const { return mFreezeDefault; }

  /**
   * Get the desired expiration time for the new token.
   *
   * @return The desired expiration time for the new token.
   */
  [[nodiscard]] inline std::chrono::system_clock::time_point getExpirationTime() const { return mExpirationTime; }

  /**
   * Get the ID of the desired auto-renew account for the new token.
   *
   * @return The ID of the desired auto-renew account for the new token. Returns uninitialized if no auto-renew account
   *         ID has been set yet.
   */
  [[nodiscard]] inline std::optional<AccountId> getAutoRenewAccountId() const { return mAutoRenewAccountId; }

  /**
   * Get the desired auto-renew period for the new token.
   *
   * @return The desired auto-renew period for the new token.
   */
  [[nodiscard]] inline std::chrono::system_clock::duration getAutoRenewPeriod() const { return mAutoRenewPeriod; }

  /**
   * Get the desired memo for the new token.
   *
   * @return The desired memo for the new token.
   */
  [[nodiscard]] inline std::string getTokenMemo() const { return mTokenMemo; }

  /**
   * Get the desired type for the new token.
   *
   * @return The desired type for the new token.
   */
  [[nodiscard]] inline TokenType getTokenType() const { return mTokenType; }

  /**
   * Get the desired token supply type for the new token.
   *
   * @return The desired token supply type for the new token.
   */
  [[nodiscard]] inline TokenSupplyType getSupplyType() const { return mSupplyType; }

  /**
   * Get the desired maximum supply for the new token.
   *
   * @return The desired maximum supply for the new token.
   */
  [[nodiscard]] inline uint64_t getMaxSupply() const { return mMaxSupply; }

  /**
   * Get the desired fee schedule key for the new token.
   *
   * @return The desired fee schedule key for the new token.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getFeeScheduleKey() const { return mFeeScheduleKey; }

  /**
   * Get the desired custom fees to be assessed during a TransferTransaction that transfers units of the new token.
   *
   * @return The desired custom fees to be assessed during a TransferTransaction that transfers units of the new token.
   */
  [[nodiscard]] inline std::vector<std::shared_ptr<CustomFee>> getCustomFees() const { return mCustomFees; }

  /**
   * Get the desired pause key for the new token.
   *
   * @return The desired pause key for the new token.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getPauseKey() const { return mPauseKey; }

  /**
   * Get the desired lock key for the new token.
   *
   * @return The desired lock key for the new token.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getLockKey() const { return mLockKey; }

  /**
   * Get the desired partition key for the new token.
   *
   * @return The desired partition key for the new token.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getPartitionKey() const { return mPartitionKey; }

  /**
   * Get the desired partition move key for the new token.
   *
   * @return The desired partition move key for the new token.
   */
  [[nodiscard]] inline std::shared_ptr<Key> getPartitionMoveKey() const { return mPartitionMoveKey; }

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Submit a Transaction protobuf object which contains this TokenCreateTransaction's data to
   * a Node.
   *
   * @param request  The Transaction protobuf object to submit.
   * @param node     The Node to which to submit the request.
   * @param deadline The deadline for submitting the request.
   * @param response Pointer to the ProtoResponseType object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const proto::Transaction& request,
                                           const std::shared_ptr<internal::Node>& node,
                                           const std::chrono::system_clock::time_point& deadline,
                                           proto::TransactionResponse* response) const override;

  /**
   * Derived from Transaction. Verify that all the checksums in this TokenCreateTransaction are valid.
   *
   * @param client The Client that should be used to validate the checksums.
   * @throws BadEntityException This TokenCreateTransaction's checksums are not valid.
   */
  void validateChecksums(const Client& client) const override;

  /**
   * Derived from Transaction. Build and add the TokenCreateTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Initialize this TokenCreateTransaction from its source TransactionBody protobuf object.
   */
  void initFromSourceTransactionBody();

  /**
   * Build a TokenCreateTransactionBody protobuf object from this TokenCreateTransaction object.
   *
   * @return A pointer to a TokenCreateTransactionBody protobuf object filled with this TokenCreateTransaction object's
   *         data.
   */
  [[nodiscard]] proto::TokenCreateTransactionBody* build() const;

  /**
   * The publicly visible name of the token. The token name is specified as a string of UTF-8 characters in Unicode.
   * UTF-8 encoding of this Unicode cannot contain the 0 byte (NUL). The token name is not unique. Maximum of 100
   * characters.
   */
  std::string mTokenName;

  /**
   * The publicly visible token symbol. The token symbol is specified as a string of UTF-8 characters in Unicode. UTF-8
   * encoding of this Unicode cannot contain the 0 byte (NUL). The token symbol is not unique. Maximum of 100
   * characters.
   */
  std::string mTokenSymbol;

  /**
   * For tokens of type FUNGIBLE_COMMON, the number of decimal places by which a token is divisible. This field can
   * never be changed. For tokens of type NON_FUNGIBLE_UNIQUE, this must be 0.
   */
  uint32_t mDecimals = 0U;

  /**
   * The initial supply of FUNGIBLE_COMMON tokens to be put in circulation. The initial supply is sent to the
   * Treasury Account. The supply is in the lowest denomination possible. For tokens of type NON_FUNGIBLE_UNIQUE, this
   * must be 0.
   */
  uint64_t mInitialSupply = 0ULL;

  /**
   * The account which will act as a treasury for the token. This account will receive the specified initial supply or
   * the newly minted NFTs in the case for NON_FUNGIBLE_UNIQUE type.
   */
  std::optional<AccountId> mTreasuryAccountId;

  /**
   * The Key which can perform update/delete operations on the token. If nullptr, the token can be perceived as
   * immutable (not being able to be updated/deleted).
   */
  std::shared_ptr<Key> mAdminKey = nullptr;

  /**
   * The Key which can grant or revoke KYC of an account for the token's transactions. If nullptr, KYC is not required,
   * and KYC grant or revoke operations are not possible.
   */
  std::shared_ptr<Key> mKycKey = nullptr;

  /**
   * The Key which can sign to freeze or unfreeze an account for token transactions. If nullptr, freezing is not
   * possible.
   */
  std::shared_ptr<Key> mFreezeKey = nullptr;

  /**
   * The Key which can wipe the token balance of an account. If nullptr, wipe is not possible.
   */
  std::shared_ptr<Key> mWipeKey = nullptr;

  /**
   * The Key which can change the supply of a token. The Key is used to sign token mint and burn operations.
   */
  std::shared_ptr<Key> mSupplyKey = nullptr;

  /**
   * The default Freeze status (frozen or unfrozen) of Hedera accounts relative to this token. If true, an account must
   * be unfrozen before it can receive the token.
   */
  bool mFreezeDefault = false;

  /**
   * The epoch second at which the token should expire. If mAutoRenewAccountId and mAutoRenewPeriod are specified, this
   * is coerced to the current epoch second plus the mAutoRenewPeriod.
   */
  std::chrono::system_clock::time_point mExpirationTime = std::chrono::system_clock::now() + DEFAULT_AUTO_RENEW_PERIOD;

  /**
   * The ID of the account which will be automatically charged to renew the token's expiration, at the interval
   * specified in mAutoRenewPeriod.
   */
  std::optional<AccountId> mAutoRenewAccountId;

  /**
   * The interval at which the auto-renew account will be charged to extend the token's expiry.
   */
  std::chrono::system_clock::duration mAutoRenewPeriod = DEFAULT_AUTO_RENEW_PERIOD;

  /**
   * The memo associated with the token (UTF-8 encoding max 100 bytes).
   */
  std::string mTokenMemo;

  /**
   * IWA compatibility. Specifies the token type.
   */
  TokenType mTokenType = TokenType::FUNGIBLE_COMMON;

  /**
   * IWA compatibility. Specifies the token supply type.
   */
  TokenSupplyType mSupplyType = TokenSupplyType::INFINITE;

  /**
   * IWA Compatibility. Depends on TokenSupplyType. For tokens of type FUNGIBLE_COMMON - the maximum number of tokens
   * that can be in circulation. For tokens of type NON_FUNGIBLE_UNIQUE - the maximum number of NFTs (serial numbers)
   * that can be minted. This field can never be changed.
   */
  uint64_t mMaxSupply = 0ULL;

  /**
   * The Key which can change the token's custom fee schedule. This Key must sign a TokenFeeScheduleUpdate transaction.
   */
  std::shared_ptr<Key> mFeeScheduleKey = nullptr;

  /**
   * The custom fees to be assessed during a TransferTransaction that transfers units of the new token.
   */
  std::vector<std::shared_ptr<CustomFee>> mCustomFees;

  /**
   * The Key which can pause and unpause the new token. If nullptr, the token pause status defaults to
   * PauseNotApplicable, otherwise Unpaused.
   */
  std::shared_ptr<Key> mPauseKey = nullptr;

  /**
   * The key which can lock, unlock, or transfer locked tokens in an account. Each fungible token balance of a
   * token-definition with a lock_key will have both a balance, and a quantity of locked tokens, where the quantity of
   * locked tokens may be 0. If this key is desired, it must be set at the time the token-definition is created. It can
   * only be set for token definitions with a TokenType of FUNGIBLE_COMMON and NON_FUNGIBLE_UNIQUE. If set, it may be
   * updated, but only if the update transaction is signed both by the lock key and the new lock key. Once null, it
   * cannot be set again.
   *
   * If set on a token-definition that also sets the partition_key, then the lock_key may also be used to lock balances
   * on those partitions.
   */
  std::shared_ptr<Key> mLockKey = nullptr;

  /**
   * The key which can create, update, and delete partitions of this token type. If this key is desired, it must be set
   * at the time the token-definition is created. It is applicable to both FUNGIBLE_COMMON and NON_FUNGIBLE_UNIQUE token
   * types. If set, it may be updated, but only if the update transaction is signed both by the old partition key and
   * the new partition key. Once null, it cannot be set again.
   */
  std::shared_ptr<Key> mPartitionKey = nullptr;

  /**
   * The key which can move balances from the token type's supply into any partition of any user, or move balance from
   * one partition to another of different types, either in the same account, or in different accounts.
   *
   * For example, if two users both have partitions "tranche-A" and "tranche-B", then either user could move tokens from
   * their "tranche-A" to the other user's "tranche-A", or from their "tranche-B" to the other user's "tranche-B", but
   * they cannot transfer from their "tranche-A" to the other user's "tranche-B", or from their "tranche-A" to their own
   * "tranche-B". That is, under normal circumstances, you can transfer funds between partitions of the same type, but
   * not between partitions of different types.
   *
   * However, a transaction signed by this key *can* transfer funds between partitions of different types, either for
   * the same user, or for different users. So user Alice can transfer balance from her "Tranche-A" to user Bob's
   * "Tranche-B", if the transaction is signed both by Alice, and by the partition-move-key. In addition, balance may be
   * transferred from Alice's "Tranche-A" into Alice's "Tranche-B", if the transaction is signed by the
   * partition-move-key. Transferring balances across partitions in the user's account does not require the user to sign
   * the transaction.
   *
   * If this key is desired, it must be set at the time the token-definition is created. It is applicable to both
   * FUNGIBLE_COMMON and NON_FUNGIBLE_UNIQUE token types. If set, it may be updated, but only if the update transaction
   * is signed both by the old partition move key and the new partition move key. Once null, it cannot be set again.
   */
  std::shared_ptr<Key> mPartitionMoveKey = nullptr;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_CREATE_TRANSACTION_H_
