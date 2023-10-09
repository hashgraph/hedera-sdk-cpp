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
#include "CustomFixedFee.h"
#include "CustomFractionalFee.h"
#include "CustomRoyaltyFee.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "TokenInfo.h"
#include "impl/DurationConverter.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/token_get_info.pb.h>

using namespace Hedera;

class TokenInfoTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTestTokenId; }
  [[nodiscard]] inline const std::string& getTestTokenName() const { return mTestTokenName; }
  [[nodiscard]] inline const std::string& getTestTokenSymbol() const { return mTestTokenSymbol; }
  [[nodiscard]] inline uint32_t getTestDecimals() const { return mTestDecimals; }
  [[nodiscard]] inline const uint64_t& getTestTotalSupply() const { return mTestTotalSupply; }
  [[nodiscard]] inline const AccountId& getTestTreasuryAccountId() const { return mTestTreasuryAccountId; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestAdminKey() const { return mTestAdminKey; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestKycKey() const { return mTestKycKey; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestFreezeKey() const { return mTestFreezeKey; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestWipeKey() const { return mTestWipeKey; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestSupplyKey() const { return mTestSupplyKey; }
  [[nodiscard]] inline const std::optional<bool>& getTestDefaultFreezeStatus() const
  {
    return mTestDefaultFreezeStatus;
  }
  [[nodiscard]] inline const std::optional<bool>& getTestDefaultKycStatus() const { return mTestDefaultKycStatus; }
  [[nodiscard]] inline bool getTestIsDeleted() const { return mTestIsDeleted; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestExpirationTime() const
  {
    return mTestExpirationTime;
  }
  [[nodiscard]] inline const AccountId& getTestAutoRenewAccountId() const { return mTestAutoRenewAccountId; }
  [[nodiscard]] inline const std::chrono::duration<double>& getTestAutoRenewPeriod() const
  {
    return mTestAutoRenewPeriod;
  }
  [[nodiscard]] inline const std::string& getTestTokenMemo() const { return mTestTokenMemo; }
  [[nodiscard]] inline TokenType getTestTokenType() const { return mTestTokenType; }
  [[nodiscard]] inline TokenSupplyType getTestTokenSupplyType() const { return mTestTokenSupplyType; }
  [[nodiscard]] inline const uint64_t& getTestMaxSupply() const { return mTestMaxSupply; }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestFeeScheduleKey() const { return mTestFeeScheduleKey; }
  [[nodiscard]] inline const std::vector<std::shared_ptr<CustomFee>>& getTestCustomFees() const
  {
    return mTestCustomFees;
  }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPauseKey() const { return mTestPauseKey; }
  [[nodiscard]] inline const std::optional<bool>& getTestPauseStatus() const { return mTestPauseStatus; }
  [[nodiscard]] inline const LedgerId& getTestLedgerId() const { return mTestLedgerId; }

private:
  const TokenId mTestTokenId = TokenId(1ULL, 2ULL, 3ULL);
  const std::string mTestTokenName = "test name";
  const std::string mTestTokenSymbol = "test symbol";
  const uint32_t mTestDecimals = 4U;
  const uint64_t mTestTotalSupply = 5ULL;
  const AccountId mTestTreasuryAccountId = AccountId(6ULL, 7ULL, 8ULL);
  const std::shared_ptr<PublicKey> mTestAdminKey = ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey();
  const std::shared_ptr<PublicKey> mTestKycKey = ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey();
  const std::shared_ptr<PublicKey> mTestFreezeKey = ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey();
  const std::shared_ptr<PublicKey> mTestWipeKey = ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey();
  const std::shared_ptr<PublicKey> mTestSupplyKey = ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey();
  const std::optional<bool> mTestDefaultFreezeStatus = true;
  const std::optional<bool> mTestDefaultKycStatus = true;
  const bool mTestIsDeleted = true;
  const std::chrono::system_clock::time_point mTestExpirationTime = std::chrono::system_clock::now();
  const AccountId mTestAutoRenewAccountId = AccountId(9ULL, 10ULL, 11ULL);
  const std::chrono::duration<double> mTestAutoRenewPeriod = std::chrono::hours(12);
  const std::string mTestTokenMemo = "test memo";
  const TokenType mTestTokenType = TokenType::NON_FUNGIBLE_UNIQUE;
  const TokenSupplyType mTestTokenSupplyType = TokenSupplyType::FINITE;
  const uint64_t mTestMaxSupply = 13ULL;
  const std::shared_ptr<PublicKey> mTestFeeScheduleKey = ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey();
  const std::vector<std::shared_ptr<CustomFee>> mTestCustomFees = { std::make_shared<CustomFixedFee>(),
                                                                    std::make_shared<CustomFractionalFee>(),
                                                                    std::make_shared<CustomRoyaltyFee>() };
  const std::shared_ptr<PublicKey> mTestPauseKey = ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey();
  const std::optional<bool> mTestPauseStatus = true;
  const LedgerId mTestLedgerId = LedgerId({ std::byte(0x0E), std::byte(0x0F) });
};

//-----
TEST_F(TokenInfoTest, FromProtobuf)
{
  // Given
  proto::TokenInfo protoTokenInfo;
  protoTokenInfo.set_allocated_tokenid(getTestTokenId().toProtobuf().release());
  protoTokenInfo.set_name(getTestTokenName());
  protoTokenInfo.set_symbol(getTestTokenSymbol());
  protoTokenInfo.set_decimals(getTestDecimals());
  protoTokenInfo.set_totalsupply(getTestTotalSupply());
  protoTokenInfo.set_allocated_treasury(getTestTreasuryAccountId().toProtobuf().release());
  protoTokenInfo.set_allocated_adminkey(getTestAdminKey()->toProtobufKey().release());
  protoTokenInfo.set_allocated_kyckey(getTestKycKey()->toProtobufKey().release());
  protoTokenInfo.set_allocated_freezekey(getTestFreezeKey()->toProtobufKey().release());
  protoTokenInfo.set_allocated_wipekey(getTestWipeKey()->toProtobufKey().release());
  protoTokenInfo.set_allocated_supplykey(getTestSupplyKey()->toProtobufKey().release());

  if (getTestDefaultFreezeStatus().has_value())
  {
    protoTokenInfo.set_defaultfreezestatus(getTestDefaultFreezeStatus().value() ? proto::TokenFreezeStatus::Frozen
                                                                                : proto::TokenFreezeStatus::Unfrozen);
  }
  else
  {
    protoTokenInfo.set_defaultfreezestatus(proto::TokenFreezeStatus::FreezeNotApplicable);
  }

  if (getTestDefaultKycStatus().has_value())
  {
    protoTokenInfo.set_defaultkycstatus(getTestDefaultKycStatus().value() ? proto::TokenKycStatus::Granted
                                                                          : proto::TokenKycStatus::Revoked);
  }
  else
  {
    protoTokenInfo.set_defaultkycstatus(proto::TokenKycStatus::KycNotApplicable);
  }

  protoTokenInfo.set_deleted(getTestIsDeleted());
  protoTokenInfo.set_allocated_autorenewaccount(getTestAutoRenewAccountId().toProtobuf().release());
  protoTokenInfo.set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(getTestAutoRenewPeriod()));
  protoTokenInfo.set_allocated_expiry(internal::TimestampConverter::toProtobuf(getTestExpirationTime()));
  protoTokenInfo.set_memo(getTestTokenMemo());
  protoTokenInfo.set_tokentype(gTokenTypeToProtobufTokenType.at(getTestTokenType()));
  protoTokenInfo.set_supplytype(gTokenSupplyTypeToProtobufTokenSupplyType.at(getTestTokenSupplyType()));
  protoTokenInfo.set_maxsupply(static_cast<int64_t>(getTestMaxSupply()));
  protoTokenInfo.set_allocated_fee_schedule_key(getTestFeeScheduleKey()->toProtobufKey().release());

  for (const auto& fee : getTestCustomFees())
  {
    *protoTokenInfo.add_custom_fees() = *fee->toProtobuf();
  }

  protoTokenInfo.set_allocated_pause_key(getTestPauseKey()->toProtobufKey().release());

  if (getTestPauseStatus().has_value())
  {
    protoTokenInfo.set_pause_status(getTestPauseStatus().value() ? proto::TokenPauseStatus::Paused
                                                                 : proto::TokenPauseStatus::Unpaused);
  }
  else
  {
    protoTokenInfo.set_pause_status(proto::TokenPauseStatus::PauseNotApplicable);
  }

  protoTokenInfo.set_ledger_id(internal::Utilities::byteVectorToString(getTestLedgerId().toBytes()));

  // When
  const TokenInfo tokenInfo = TokenInfo::fromProtobuf(protoTokenInfo);

  // Then
  EXPECT_EQ(tokenInfo.mTokenId, getTestTokenId());
  EXPECT_EQ(tokenInfo.mTokenName, getTestTokenName());
  EXPECT_EQ(tokenInfo.mTokenSymbol, getTestTokenSymbol());
  EXPECT_EQ(tokenInfo.mDecimals, getTestDecimals());
  EXPECT_EQ(tokenInfo.mTotalSupply, getTestTotalSupply());
  EXPECT_EQ(tokenInfo.mTreasuryAccountId, getTestTreasuryAccountId());
  EXPECT_EQ(tokenInfo.mAdminKey->toBytes(), getTestAdminKey()->toBytes());
  EXPECT_EQ(tokenInfo.mKycKey->toBytes(), getTestKycKey()->toBytes());
  EXPECT_EQ(tokenInfo.mFreezeKey->toBytes(), getTestFreezeKey()->toBytes());
  EXPECT_EQ(tokenInfo.mWipeKey->toBytes(), getTestWipeKey()->toBytes());
  EXPECT_EQ(tokenInfo.mSupplyKey->toBytes(), getTestSupplyKey()->toBytes());
  EXPECT_EQ(tokenInfo.mDefaultFreezeStatus, getTestDefaultFreezeStatus());
  EXPECT_EQ(tokenInfo.mDefaultKycStatus, getTestDefaultKycStatus());
  EXPECT_EQ(tokenInfo.mIsDeleted, getTestIsDeleted());
  EXPECT_EQ(tokenInfo.mAutoRenewAccountId, getTestAutoRenewAccountId());
  EXPECT_EQ(tokenInfo.mAutoRenewPeriod, getTestAutoRenewPeriod());
  EXPECT_EQ(tokenInfo.mExpirationTime, getTestExpirationTime());
  EXPECT_EQ(tokenInfo.mTokenMemo, getTestTokenMemo());
  EXPECT_EQ(tokenInfo.mTokenType, getTestTokenType());
  EXPECT_EQ(tokenInfo.mSupplyType, getTestTokenSupplyType());
  EXPECT_EQ(tokenInfo.mMaxSupply, getTestMaxSupply());
  EXPECT_EQ(tokenInfo.mFeeScheduleKey->toBytes(), getTestFeeScheduleKey()->toBytes());
  EXPECT_EQ(tokenInfo.mCustomFees.size(), 3);
  EXPECT_EQ(tokenInfo.mPauseKey->toBytes(), getTestPauseKey()->toBytes());
  EXPECT_EQ(tokenInfo.mPauseStatus, getTestPauseStatus());
  EXPECT_EQ(tokenInfo.mLedgerId.toBytes(), getTestLedgerId().toBytes());
}

//-----
TEST_F(TokenInfoTest, FromBytes)
{
  // Given
  proto::TokenInfo protoTokenInfo;
  protoTokenInfo.set_allocated_tokenid(getTestTokenId().toProtobuf().release());
  protoTokenInfo.set_name(getTestTokenName());
  protoTokenInfo.set_symbol(getTestTokenSymbol());
  protoTokenInfo.set_decimals(getTestDecimals());
  protoTokenInfo.set_totalsupply(getTestTotalSupply());
  protoTokenInfo.set_allocated_treasury(getTestTreasuryAccountId().toProtobuf().release());
  protoTokenInfo.set_allocated_adminkey(getTestAdminKey()->toProtobufKey().release());
  protoTokenInfo.set_allocated_kyckey(getTestKycKey()->toProtobufKey().release());
  protoTokenInfo.set_allocated_freezekey(getTestFreezeKey()->toProtobufKey().release());
  protoTokenInfo.set_allocated_wipekey(getTestWipeKey()->toProtobufKey().release());
  protoTokenInfo.set_allocated_supplykey(getTestSupplyKey()->toProtobufKey().release());

  if (getTestDefaultFreezeStatus().has_value())
  {
    protoTokenInfo.set_defaultfreezestatus(getTestDefaultFreezeStatus().value() ? proto::TokenFreezeStatus::Frozen
                                                                                : proto::TokenFreezeStatus::Unfrozen);
  }
  else
  {
    protoTokenInfo.set_defaultfreezestatus(proto::TokenFreezeStatus::FreezeNotApplicable);
  }

  if (getTestDefaultKycStatus().has_value())
  {
    protoTokenInfo.set_defaultkycstatus(getTestDefaultKycStatus().value() ? proto::TokenKycStatus::Granted
                                                                          : proto::TokenKycStatus::Revoked);
  }
  else
  {
    protoTokenInfo.set_defaultkycstatus(proto::TokenKycStatus::KycNotApplicable);
  }

  protoTokenInfo.set_deleted(getTestIsDeleted());
  protoTokenInfo.set_allocated_autorenewaccount(getTestAutoRenewAccountId().toProtobuf().release());
  protoTokenInfo.set_allocated_autorenewperiod(internal::DurationConverter::toProtobuf(getTestAutoRenewPeriod()));
  protoTokenInfo.set_allocated_expiry(internal::TimestampConverter::toProtobuf(getTestExpirationTime()));
  protoTokenInfo.set_memo(getTestTokenMemo());
  protoTokenInfo.set_tokentype(gTokenTypeToProtobufTokenType.at(getTestTokenType()));
  protoTokenInfo.set_supplytype(gTokenSupplyTypeToProtobufTokenSupplyType.at(getTestTokenSupplyType()));
  protoTokenInfo.set_maxsupply(static_cast<int64_t>(getTestMaxSupply()));
  protoTokenInfo.set_allocated_fee_schedule_key(getTestFeeScheduleKey()->toProtobufKey().release());

  for (const auto& fee : getTestCustomFees())
  {
    *protoTokenInfo.add_custom_fees() = *fee->toProtobuf();
  }

  protoTokenInfo.set_allocated_pause_key(getTestPauseKey()->toProtobufKey().release());

  if (getTestPauseStatus().has_value())
  {
    protoTokenInfo.set_pause_status(getTestPauseStatus().value() ? proto::TokenPauseStatus::Paused
                                                                 : proto::TokenPauseStatus::Unpaused);
  }
  else
  {
    protoTokenInfo.set_pause_status(proto::TokenPauseStatus::PauseNotApplicable);
  }

  protoTokenInfo.set_ledger_id(internal::Utilities::byteVectorToString(getTestLedgerId().toBytes()));

  // When
  const TokenInfo tokenInfo =
    TokenInfo::fromBytes(internal::Utilities::stringToByteVector(protoTokenInfo.SerializeAsString()));

  // Then
  EXPECT_EQ(tokenInfo.mTokenId, getTestTokenId());
  EXPECT_EQ(tokenInfo.mTokenName, getTestTokenName());
  EXPECT_EQ(tokenInfo.mTokenSymbol, getTestTokenSymbol());
  EXPECT_EQ(tokenInfo.mDecimals, getTestDecimals());
  EXPECT_EQ(tokenInfo.mTotalSupply, getTestTotalSupply());
  EXPECT_EQ(tokenInfo.mTreasuryAccountId, getTestTreasuryAccountId());
  EXPECT_EQ(tokenInfo.mAdminKey->toBytes(), getTestAdminKey()->toBytes());
  EXPECT_EQ(tokenInfo.mKycKey->toBytes(), getTestKycKey()->toBytes());
  EXPECT_EQ(tokenInfo.mFreezeKey->toBytes(), getTestFreezeKey()->toBytes());
  EXPECT_EQ(tokenInfo.mWipeKey->toBytes(), getTestWipeKey()->toBytes());
  EXPECT_EQ(tokenInfo.mSupplyKey->toBytes(), getTestSupplyKey()->toBytes());
  EXPECT_EQ(tokenInfo.mDefaultFreezeStatus, getTestDefaultFreezeStatus());
  EXPECT_EQ(tokenInfo.mDefaultKycStatus, getTestDefaultKycStatus());
  EXPECT_EQ(tokenInfo.mIsDeleted, getTestIsDeleted());
  EXPECT_EQ(tokenInfo.mAutoRenewAccountId, getTestAutoRenewAccountId());
  EXPECT_EQ(tokenInfo.mAutoRenewPeriod, getTestAutoRenewPeriod());
  EXPECT_EQ(tokenInfo.mExpirationTime, getTestExpirationTime());
  EXPECT_EQ(tokenInfo.mTokenMemo, getTestTokenMemo());
  EXPECT_EQ(tokenInfo.mTokenType, getTestTokenType());
  EXPECT_EQ(tokenInfo.mSupplyType, getTestTokenSupplyType());
  EXPECT_EQ(tokenInfo.mMaxSupply, getTestMaxSupply());
  EXPECT_EQ(tokenInfo.mFeeScheduleKey->toBytes(), getTestFeeScheduleKey()->toBytes());
  EXPECT_EQ(tokenInfo.mCustomFees.size(), 3);
  EXPECT_EQ(tokenInfo.mPauseKey->toBytes(), getTestPauseKey()->toBytes());
  EXPECT_EQ(tokenInfo.mPauseStatus, getTestPauseStatus());
  EXPECT_EQ(tokenInfo.mLedgerId.toBytes(), getTestLedgerId().toBytes());
}

//-----
TEST_F(TokenInfoTest, ToProtobuf)
{
  // Given
  TokenInfo tokenInfo;
  tokenInfo.mTokenId = getTestTokenId();
  tokenInfo.mTokenName = getTestTokenName();
  tokenInfo.mTokenSymbol = getTestTokenSymbol();
  tokenInfo.mDecimals = getTestDecimals();
  tokenInfo.mTotalSupply = getTestTotalSupply();
  tokenInfo.mTreasuryAccountId = getTestTreasuryAccountId();
  tokenInfo.mAdminKey = getTestAdminKey();
  tokenInfo.mKycKey = getTestKycKey();
  tokenInfo.mFreezeKey = getTestFreezeKey();
  tokenInfo.mWipeKey = getTestWipeKey();
  tokenInfo.mSupplyKey = getTestSupplyKey();
  tokenInfo.mDefaultFreezeStatus = getTestDefaultFreezeStatus();
  tokenInfo.mDefaultKycStatus = getTestDefaultKycStatus();
  tokenInfo.mIsDeleted = getTestIsDeleted();
  tokenInfo.mAutoRenewAccountId = getTestAutoRenewAccountId();
  tokenInfo.mAutoRenewPeriod = getTestAutoRenewPeriod();
  tokenInfo.mExpirationTime = getTestExpirationTime();
  tokenInfo.mTokenMemo = getTestTokenMemo();
  tokenInfo.mTokenType = getTestTokenType();
  tokenInfo.mSupplyType = getTestTokenSupplyType();
  tokenInfo.mMaxSupply = getTestMaxSupply();
  tokenInfo.mFeeScheduleKey = getTestFeeScheduleKey();
  tokenInfo.mCustomFees = getTestCustomFees();
  tokenInfo.mPauseKey = getTestPauseKey();
  tokenInfo.mPauseStatus = getTestPauseStatus();
  tokenInfo.mLedgerId = getTestLedgerId();

  // When
  const std::unique_ptr<proto::TokenInfo> protoTokenInfo = tokenInfo.toProtobuf();

  // Then
  EXPECT_EQ(protoTokenInfo->tokenid().shardnum(), getTestTokenId().mShardNum);
  EXPECT_EQ(protoTokenInfo->tokenid().realmnum(), getTestTokenId().mRealmNum);
  EXPECT_EQ(protoTokenInfo->tokenid().tokennum(), getTestTokenId().mTokenNum);
  EXPECT_EQ(protoTokenInfo->name(), getTestTokenName());
  EXPECT_EQ(protoTokenInfo->symbol(), getTestTokenSymbol());
  EXPECT_EQ(protoTokenInfo->decimals(), getTestDecimals());
  EXPECT_EQ(protoTokenInfo->totalsupply(), getTestTotalSupply());
  EXPECT_EQ(protoTokenInfo->treasury().shardnum(), getTestTreasuryAccountId().mShardNum);
  EXPECT_EQ(protoTokenInfo->treasury().realmnum(), getTestTreasuryAccountId().mRealmNum);
  EXPECT_EQ(protoTokenInfo->treasury().accountnum(), getTestTreasuryAccountId().mAccountNum);
  EXPECT_EQ(protoTokenInfo->adminkey().ecdsa_secp256k1(),
            internal::Utilities::byteVectorToString(getTestAdminKey()->toBytesRaw()));
  EXPECT_EQ(protoTokenInfo->kyckey().ecdsa_secp256k1(),
            internal::Utilities::byteVectorToString(getTestKycKey()->toBytesRaw()));
  EXPECT_EQ(protoTokenInfo->freezekey().ecdsa_secp256k1(),
            internal::Utilities::byteVectorToString(getTestFreezeKey()->toBytesRaw()));
  EXPECT_EQ(protoTokenInfo->wipekey().ecdsa_secp256k1(),
            internal::Utilities::byteVectorToString(getTestWipeKey()->toBytesRaw()));
  EXPECT_EQ(protoTokenInfo->supplykey().ecdsa_secp256k1(),
            internal::Utilities::byteVectorToString(getTestSupplyKey()->toBytesRaw()));
  EXPECT_EQ(protoTokenInfo->defaultfreezestatus(),
            (!getTestDefaultFreezeStatus().has_value()
               ? proto::TokenFreezeStatus::FreezeNotApplicable
               : (getTestDefaultFreezeStatus().value() ? proto::TokenFreezeStatus::Frozen
                                                       : proto::TokenFreezeStatus::Unfrozen)));
  EXPECT_EQ(
    protoTokenInfo->defaultkycstatus(),
    (!getTestDefaultKycStatus().has_value()
       ? proto::TokenKycStatus::KycNotApplicable
       : (getTestDefaultKycStatus().value() ? proto::TokenKycStatus::Granted : proto::TokenKycStatus::Revoked)));
  EXPECT_EQ(protoTokenInfo->deleted(), getTestIsDeleted());
  EXPECT_EQ(protoTokenInfo->autorenewaccount().shardnum(), getTestAutoRenewAccountId().mShardNum);
  EXPECT_EQ(protoTokenInfo->autorenewaccount().realmnum(), getTestAutoRenewAccountId().mRealmNum);
  EXPECT_EQ(protoTokenInfo->autorenewaccount().accountnum(), getTestAutoRenewAccountId().mAccountNum);
  EXPECT_EQ(protoTokenInfo->autorenewperiod().seconds(),
            internal::DurationConverter::toProtobuf(getTestAutoRenewPeriod())->seconds());
  EXPECT_EQ(protoTokenInfo->expiry().seconds(),
            internal::TimestampConverter::toProtobuf(getTestExpirationTime())->seconds());
  EXPECT_EQ(protoTokenInfo->memo(), getTestTokenMemo());
  EXPECT_EQ(protoTokenInfo->tokentype(), gTokenTypeToProtobufTokenType.at(getTestTokenType()));
  EXPECT_EQ(protoTokenInfo->supplytype(), gTokenSupplyTypeToProtobufTokenSupplyType.at(getTestTokenSupplyType()));
  EXPECT_EQ(protoTokenInfo->maxsupply(), getTestMaxSupply());
  EXPECT_EQ(protoTokenInfo->fee_schedule_key().ecdsa_secp256k1(),
            internal::Utilities::byteVectorToString(getTestFeeScheduleKey()->toBytesRaw()));
  EXPECT_EQ(protoTokenInfo->custom_fees_size(), getTestCustomFees().size());
  EXPECT_EQ(protoTokenInfo->pause_key().ecdsa_secp256k1(),
            internal::Utilities::byteVectorToString(getTestPauseKey()->toBytesRaw()));
  EXPECT_EQ(protoTokenInfo->pause_status(),
            (!getTestPauseStatus().has_value()
               ? proto::TokenPauseStatus::PauseNotApplicable
               : (getTestPauseStatus().value() ? proto::TokenPauseStatus::Paused : proto::TokenPauseStatus::Unpaused)));
  EXPECT_EQ(protoTokenInfo->ledger_id(), internal::Utilities::byteVectorToString(getTestLedgerId().toBytes()));
}

//-----
TEST_F(TokenInfoTest, ToBytes)
{
  // Given
  TokenInfo tokenInfo;
  tokenInfo.mTokenId = getTestTokenId();
  tokenInfo.mTokenName = getTestTokenName();
  tokenInfo.mTokenSymbol = getTestTokenSymbol();
  tokenInfo.mDecimals = getTestDecimals();
  tokenInfo.mTotalSupply = getTestTotalSupply();
  tokenInfo.mTreasuryAccountId = getTestTreasuryAccountId();
  tokenInfo.mAdminKey = getTestAdminKey();
  tokenInfo.mKycKey = getTestKycKey();
  tokenInfo.mFreezeKey = getTestFreezeKey();
  tokenInfo.mWipeKey = getTestWipeKey();
  tokenInfo.mSupplyKey = getTestSupplyKey();
  tokenInfo.mDefaultFreezeStatus = getTestDefaultFreezeStatus();
  tokenInfo.mDefaultKycStatus = getTestDefaultKycStatus();
  tokenInfo.mIsDeleted = getTestIsDeleted();
  tokenInfo.mAutoRenewAccountId = getTestAutoRenewAccountId();
  tokenInfo.mAutoRenewPeriod = getTestAutoRenewPeriod();
  tokenInfo.mExpirationTime = getTestExpirationTime();
  tokenInfo.mTokenMemo = getTestTokenMemo();
  tokenInfo.mTokenType = getTestTokenType();
  tokenInfo.mSupplyType = getTestTokenSupplyType();
  tokenInfo.mMaxSupply = getTestMaxSupply();
  tokenInfo.mFeeScheduleKey = getTestFeeScheduleKey();
  tokenInfo.mCustomFees = getTestCustomFees();
  tokenInfo.mPauseKey = getTestPauseKey();
  tokenInfo.mPauseStatus = getTestPauseStatus();
  tokenInfo.mLedgerId = getTestLedgerId();

  // When
  const std::vector<std::byte> bytes = tokenInfo.toBytes();

  // Then
  EXPECT_EQ(bytes, internal::Utilities::stringToByteVector(tokenInfo.toProtobuf()->SerializeAsString()));
}
