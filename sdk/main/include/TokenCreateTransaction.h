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
   * Set the desired name for the new token.
   *
   * @param name The desired name for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set token name.
   */
  TokenCreateTransaction& setName(std::string_view name);

  /**
   * Set the desired symbol for the new token.
   *
   * @param symbol The desired symbol for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set token symbol.
   */
  TokenCreateTransaction& setSymbol(std::string_view symbol);

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
  TokenCreateTransaction& setTreasury(const AccountId& accountId);

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
  TokenCreateTransaction& setAutoRenewPeriod(const std::chrono::duration<double>& period);

  /**
   * Set the desired memo for the new token.
   *
   * @param memo The desired memo for the new token.
   * @return A reference to this TokenCreateTransaction with the newly-set memo.
   */
  TokenCreateTransaction& setMemo(std::string_view memo);

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
   * Get the desired name for the new token.
   *
   * @return The desired name for the new token.
   */
  [[nodiscard]] inline std::string getName() const { return mName; }

  /**
   * Get the desired symbol for the new token.
   *
   * @return The desired symbol for the new token.
   */
  [[nodiscard]] inline std::string getSymbol() const { return mSymbol; }

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
   * @return The ID of the desired treasury account for the supply of new tokens.
   */
  [[nodiscard]] inline AccountId getTreasuryAccount() const { return mTreasury; }

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
   * @return The ID of the desired auto-renew account for the new token.
   */
  [[nodiscard]] inline AccountId getAutoRenewAccountId() const { return mAutoRenewAccountId; }

  /**
   * Get the desired auto-renew period for the new token.
   *
   * @return The desired auto-renew period for the new token.
   */
  [[nodiscard]] inline std::chrono::duration<double> getAutoRenewPeriod() const { return mAutoRenewPeriod; }

  /**
   * Get the desired memo for the new token.
   *
   * @return The desired memo for the new token.
   */
  [[nodiscard]] inline std::string getMemo() const { return mMemo; }

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
  [[nodiscard]] inline std::shared_ptr<Key> getFeeScheduleKey() const { return mSupplyKey; }

  /**
   * Get the desired custom fees to be assessed during a TransferTransaction that transfers units of the new token.
   *
   * @return The desired custom fees to be assessed during a TransferTransaction that transfers units of the new token.
   */
  [[nodiscard]] inline std::vector<std::shared_ptr<CustomFee>> getCustomFees() const { return mCustomFees; }

private:
  /**
   * Derived from Executable. Construct a Transaction protobuf object from this TokenCreateTransaction object.
   *
   * @param client The Client trying to construct this TokenCreateTransaction.
   * @param node   The Node to which this TokenCreateTransaction will be sent. This is unused.
   * @return A Transaction protobuf object filled with this TokenCreateTransaction object's data.
   * @throws UninitializedException If the input client has no operator with which to sign this
   *                                TokenCreateTransaction.
   */
  [[nodiscard]] proto::Transaction makeRequest(const Client& client,
                                               const std::shared_ptr<internal::Node>& /*node*/) const override;

  /**
   * Derived from Executable. Submit this TokenCreateTransaction to a Node.
   *
   * @param client   The Client submitting this TokenCreateTransaction.
   * @param deadline The deadline for submitting this TokenCreateTransaction.
   * @param node     Pointer to the Node to which this TokenCreateTransaction should be submitted.
   * @param response Pointer to the TransactionResponse protobuf object that gRPC should populate with the response
   *                 information from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const Client& client,
                                           const std::chrono::system_clock::time_point& deadline,
                                           const std::shared_ptr<internal::Node>& node,
                                           proto::TransactionResponse* response) const override;

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
  std::string mName;

  /**
   * The publicly visible token symbol. The token symbol is specified as a string of UTF-8 characters in Unicode. UTF-8
   * encoding of this Unicode cannot contain the 0 byte (NUL). The token symbol is not unique. Maximum of 100
   * characters.
   */
  std::string mSymbol;

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
  AccountId mTreasury;

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
  std::chrono::system_clock::time_point mExpirationTime = std::chrono::system_clock::now();

  /**
   * The ID of the account which will be automatically charged to renew the token's expiration, at the interval
   * specified in mAutoRenewPeriod.
   */
  AccountId mAutoRenewAccountId;

  /**
   * The interval at which the auto-renew account will be charged to extend the token's expiry.
   */
  std::chrono::duration<double> mAutoRenewPeriod = DEFAULT_AUTO_RENEW_PERIOD;

  /**
   * The memo associated with the token (UTF-8 encoding max 100 bytes).
   */
  std::string mMemo;

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
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_CREATE_TRANSACTION_H_
