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
#include "Transaction.h"
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "AccountUpdateTransaction.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "TransferTransaction.h"
#include "impl/DurationConverter.h"
#include "impl/TimestampConverter.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>
#include <proto/transaction_contents.pb.h>

using namespace Hedera;

class TransactionTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    // Initialize the CryptoCreate unique_ptr
    mCryptoCreateTransactionBody->set_allocated_key(mPublicKey->toProtobuf().release());
    mCryptoCreateTransactionBody->set_initialbalance(static_cast<uint64_t>(mInitialBalance.toTinybars()));
    mCryptoCreateTransactionBody->set_receiversigrequired(mReceiverSignatureRequired);
    mCryptoCreateTransactionBody->set_allocated_autorenewperiod(
      internal::DurationConverter::toProtobuf(mAutoRenewPeriod));
    mCryptoCreateTransactionBody->set_allocated_memo(new std::string{ mAccountMemo.cbegin(), mAccountMemo.cend() });
    mCryptoCreateTransactionBody->set_max_automatic_token_associations(static_cast<int32_t>(mMaxTokenAssociations));
    mCryptoCreateTransactionBody->set_allocated_staked_account_id(mAccountId.toProtobuf().release());
    mCryptoCreateTransactionBody->set_decline_reward(mDeclineStakingReward);

    const std::vector<std::byte> testPublicKeyBytes = mPublicKey->toBytesDer();
    const std::vector<std::byte> testEvmAddressBytes = mEvmAddress.toBytes();
    mCryptoCreateTransactionBody->set_allocated_alias(
      new std::string(internal::Utilities::byteVectorToString(testPublicKeyBytes)));
    mCryptoCreateTransactionBody->set_allocated_evm_address(
      new std::string(internal::Utilities::byteVectorToString(testEvmAddressBytes)));

    // Initialize the CryptoTransfer unique_ptr
    proto::AccountAmount* amount = mCryptoTransferTransactionBody->mutable_transfers()->add_accountamounts();
    amount->set_allocated_accountid(getTestAccountId().toProtobuf().release());
    amount->set_amount(getTestAmount().toTinybars());
    amount->set_is_approval(getTestApproval());

    proto::TokenTransferList* list = mCryptoTransferTransactionBody->add_tokentransfers();
    list->set_allocated_token(getTestTokenId().toProtobuf().release());
    list->mutable_expected_decimals()->set_value(getTestExpectedDecimals());

    amount = list->add_transfers();
    amount->set_allocated_accountid(getTestAccountId().toProtobuf().release());
    amount->set_amount(getTestAmount().toTinybars());
    amount->set_is_approval(getTestApproval());

    list = mCryptoTransferTransactionBody->add_tokentransfers();
    list->set_allocated_token(getTestNftId().getTokenId().toProtobuf().release());

    proto::NftTransfer* nft = list->add_nfttransfers();
    nft->set_allocated_senderaccountid(getTestAccountId().toProtobuf().release());
    nft->set_allocated_receiveraccountid(getTestAccountId().toProtobuf().release());
    nft->set_serialnumber(static_cast<int64_t>(getTestNftId().getSerialNum()));
    nft->set_is_approval(getTestApproval());

    // Initialize the CryptoUpdate unique_ptr
    mCryptoUpdateTransactionBody->set_allocated_accountidtoupdate(mAccountId.toProtobuf().release());
    mCryptoUpdateTransactionBody->set_allocated_key(mPublicKey->toProtobuf().release());
    mCryptoUpdateTransactionBody->set_allocated_autorenewperiod(
      internal::DurationConverter::toProtobuf(mAutoRenewPeriod));
    mCryptoUpdateTransactionBody->set_allocated_expirationtime(
      internal::TimestampConverter::toProtobuf(mExpirationTime));
    mCryptoUpdateTransactionBody->set_staked_node_id(static_cast<int64_t>(mNodeId));

    auto boolValue = std::make_unique<google::protobuf::BoolValue>();
    boolValue->set_value(mReceiverSignatureRequired);
    mCryptoUpdateTransactionBody->set_allocated_receiversigrequiredwrapper(boolValue.release());

    auto strValue = std::make_unique<google::protobuf::StringValue>();
    strValue->set_value(mAccountMemo);
    mCryptoUpdateTransactionBody->set_allocated_memo(strValue.release());

    auto uInt32Value = std::make_unique<google::protobuf::Int32Value>();
    uInt32Value->set_value(static_cast<int32_t>(mMaxTokenAssociations));
    mCryptoUpdateTransactionBody->set_allocated_max_automatic_token_associations(uInt32Value.release());

    boolValue = std::make_unique<google::protobuf::BoolValue>();
    boolValue->set_value(mDeclineStakingReward);
    mCryptoUpdateTransactionBody->set_allocated_decline_reward(boolValue.release());

    // Initialize the CryptoDelete unique_ptr
    mCryptoDeleteTransactionBody->set_allocated_deleteaccountid(getTestAccountId().toProtobuf().release());
    mCryptoDeleteTransactionBody->set_allocated_transferaccountid(getTestAccountId().toProtobuf().release());
  }

  [[nodiscard]] inline const std::unique_ptr<proto::CryptoCreateTransactionBody>& getTestCryptoCreateTransactionBody()
    const
  {
    return mCryptoCreateTransactionBody;
  }
  [[nodiscard]] inline const std::unique_ptr<proto::CryptoTransferTransactionBody>&
  getTestCryptoTransferTransactionBody() const
  {
    return mCryptoTransferTransactionBody;
  }
  [[nodiscard]] inline const std::unique_ptr<proto::CryptoUpdateTransactionBody>& getTestCryptoUpdateTransactionBody()
    const
  {
    return mCryptoUpdateTransactionBody;
  }
  [[nodiscard]] inline const std::unique_ptr<proto::CryptoDeleteTransactionBody>& getTestCryptoDeleteTransactionBody()
    const
  {
    return mCryptoDeleteTransactionBody;
  }
  [[nodiscard]] inline const std::shared_ptr<PublicKey>& getTestPublicKey() const { return mPublicKey; }
  [[nodiscard]] inline const Hbar& getTestInitialBalance() const { return mInitialBalance; }
  [[nodiscard]] inline bool getTestReceiverSignatureRequired() const { return mReceiverSignatureRequired; }
  [[nodiscard]] inline const std::chrono::duration<double>& getTestAutoRenewPeriod() const { return mAutoRenewPeriod; }
  [[nodiscard]] inline const std::string& getTestAccountMemo() const { return mAccountMemo; }
  [[nodiscard]] inline uint32_t getTestMaximumTokenAssociations() const { return mMaxTokenAssociations; }
  [[nodiscard]] inline const AccountId& getTestAccountId() const { return mAccountId; }
  [[nodiscard]] inline const uint64_t& getTestNodeId() const { return mNodeId; }
  [[nodiscard]] inline bool getTestDeclineStakingReward() const { return mDeclineStakingReward; }
  [[nodiscard]] inline const EvmAddress& getTestEvmAddress() const { return mEvmAddress; }
  [[nodiscard]] inline const TokenId& getTestTokenId() const { return mTokenId; }
  [[nodiscard]] inline const NftId& getTestNftId() const { return mNftId; }
  [[nodiscard]] inline const Hbar& getTestAmount() const { return mAmount; }
  [[nodiscard]] inline uint32_t getTestExpectedDecimals() const { return mExpectedDecimals; }
  [[nodiscard]] inline bool getTestApproval() const { return mApproval; }
  [[nodiscard]] inline const std::chrono::system_clock::time_point& getTestExpirationTime() const
  {
    return mExpirationTime;
  }

private:
  std::unique_ptr<proto::CryptoCreateTransactionBody> mCryptoCreateTransactionBody =
    std::make_unique<proto::CryptoCreateTransactionBody>();
  std::unique_ptr<proto::CryptoTransferTransactionBody> mCryptoTransferTransactionBody =
    std::make_unique<proto::CryptoTransferTransactionBody>();
  std::unique_ptr<proto::CryptoUpdateTransactionBody> mCryptoUpdateTransactionBody =
    std::make_unique<proto::CryptoUpdateTransactionBody>();
  std::unique_ptr<proto::CryptoDeleteTransactionBody> mCryptoDeleteTransactionBody =
    std::make_unique<proto::CryptoDeleteTransactionBody>();

  const std::shared_ptr<PublicKey> mPublicKey = ECDSAsecp256k1PrivateKey::generatePrivateKey()->getPublicKey();
  const Hbar mInitialBalance = Hbar(1LL);
  const bool mReceiverSignatureRequired = true;
  const std::chrono::duration<double> mAutoRenewPeriod = std::chrono::hours(2);
  const std::string mAccountMemo = "test account memo";
  const uint32_t mMaxTokenAssociations = 3U;
  const AccountId mAccountId = AccountId(4ULL);
  const uint64_t mNodeId = 5ULL;
  const bool mDeclineStakingReward = true;
  const EvmAddress mEvmAddress = EvmAddress::fromString("303132333435363738396162636465666768696a");
  const TokenId mTokenId = TokenId(6ULL);
  const NftId mNftId = NftId(mTokenId, 7ULL);
  const Hbar mAmount = Hbar(8ULL);
  const uint32_t mExpectedDecimals = 9U;
  const bool mApproval = true;
  const std::chrono::system_clock::time_point mExpirationTime = std::chrono::system_clock::now();
};

//-----
TEST_F(TransactionTest, AccountCreateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptocreateaccount(
    std::make_unique<proto::CryptoCreateTransactionBody>(*getTestCryptoCreateTransactionBody()).release());

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

  // Then
  ASSERT_EQ(index, 0);

  const AccountCreateTransaction accountCreateTransaction = std::get<0>(txVariant);
  EXPECT_EQ(accountCreateTransaction.getKey()->toStringDer(), getTestPublicKey()->toStringDer());
  EXPECT_EQ(accountCreateTransaction.getInitialBalance(), getTestInitialBalance());
  EXPECT_EQ(accountCreateTransaction.getReceiverSignatureRequired(), getTestReceiverSignatureRequired());
  EXPECT_EQ(accountCreateTransaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  EXPECT_EQ(accountCreateTransaction.getAccountMemo(), getTestAccountMemo());
  EXPECT_EQ(accountCreateTransaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
  ASSERT_TRUE(accountCreateTransaction.getStakedAccountId().has_value());
  EXPECT_EQ(*accountCreateTransaction.getStakedAccountId(), getTestAccountId());
  EXPECT_FALSE(accountCreateTransaction.getStakedNodeId().has_value());
  EXPECT_EQ(accountCreateTransaction.getDeclineStakingReward(), getTestDeclineStakingReward());
  EXPECT_EQ(accountCreateTransaction.getAlias()->toBytesDer(), getTestPublicKey()->toBytesDer());
  ASSERT_TRUE(accountCreateTransaction.getEvmAddress().has_value());
  EXPECT_EQ(accountCreateTransaction.getEvmAddress()->toBytes(), getTestEvmAddress().toBytes());
}

//-----
TEST_F(TransactionTest, AccountCreateTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptocreateaccount(
    std::make_unique<proto::CryptoCreateTransactionBody>(*getTestCryptoCreateTransactionBody()).release());

  proto::SignedTransaction signedTx;
  signedTx.set_allocated_bodybytes(new std::string(txBody.SerializeAsString()));
  // SignatureMap not required

  const std::string serialized = signedTx.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

  // Then
  ASSERT_EQ(index, 0);

  const AccountCreateTransaction accountCreateTransaction = std::get<0>(txVariant);
  EXPECT_EQ(accountCreateTransaction.getKey()->toStringDer(), getTestPublicKey()->toStringDer());
  EXPECT_EQ(accountCreateTransaction.getInitialBalance(), getTestInitialBalance());
  EXPECT_EQ(accountCreateTransaction.getReceiverSignatureRequired(), getTestReceiverSignatureRequired());
  EXPECT_EQ(accountCreateTransaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  EXPECT_EQ(accountCreateTransaction.getAccountMemo(), getTestAccountMemo());
  EXPECT_EQ(accountCreateTransaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
  ASSERT_TRUE(accountCreateTransaction.getStakedAccountId().has_value());
  EXPECT_EQ(*accountCreateTransaction.getStakedAccountId(), getTestAccountId());
  EXPECT_FALSE(accountCreateTransaction.getStakedNodeId().has_value());
  EXPECT_EQ(accountCreateTransaction.getDeclineStakingReward(), getTestDeclineStakingReward());
  EXPECT_EQ(accountCreateTransaction.getAlias()->toBytesDer(), getTestPublicKey()->toBytesDer());
  ASSERT_TRUE(accountCreateTransaction.getEvmAddress().has_value());
  EXPECT_EQ(accountCreateTransaction.getEvmAddress()->toBytes(), getTestEvmAddress().toBytes());
}

//-----
TEST_F(TransactionTest, AccountCreateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptocreateaccount(
    std::make_unique<proto::CryptoCreateTransactionBody>(*getTestCryptoCreateTransactionBody()).release());

  proto::SignedTransaction signedTx;
  signedTx.set_allocated_bodybytes(new std::string(txBody.SerializeAsString()));
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_allocated_signedtransactionbytes(new std::string(signedTx.SerializeAsString()));

  const std::string serialized = tx.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

  // Then
  ASSERT_EQ(index, 0);

  const AccountCreateTransaction accountCreateTransaction = std::get<0>(txVariant);
  EXPECT_EQ(accountCreateTransaction.getKey()->toStringDer(), getTestPublicKey()->toStringDer());
  EXPECT_EQ(accountCreateTransaction.getInitialBalance(), getTestInitialBalance());
  EXPECT_EQ(accountCreateTransaction.getReceiverSignatureRequired(), getTestReceiverSignatureRequired());
  EXPECT_EQ(accountCreateTransaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  EXPECT_EQ(accountCreateTransaction.getAccountMemo(), getTestAccountMemo());
  EXPECT_EQ(accountCreateTransaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
  ASSERT_TRUE(accountCreateTransaction.getStakedAccountId().has_value());
  EXPECT_EQ(*accountCreateTransaction.getStakedAccountId(), getTestAccountId());
  EXPECT_FALSE(accountCreateTransaction.getStakedNodeId().has_value());
  EXPECT_EQ(accountCreateTransaction.getDeclineStakingReward(), getTestDeclineStakingReward());
  EXPECT_EQ(accountCreateTransaction.getAlias()->toBytesDer(), getTestPublicKey()->toBytesDer());
  ASSERT_TRUE(accountCreateTransaction.getEvmAddress().has_value());
  EXPECT_EQ(accountCreateTransaction.getEvmAddress()->toBytes(), getTestEvmAddress().toBytes());
}

//-----
TEST_F(TransactionTest, TransferTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptotransfer(
    std::make_unique<proto::CryptoTransferTransactionBody>(*getTestCryptoTransferTransactionBody()).release());

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

  // Then
  ASSERT_EQ(index, 1);

  const TransferTransaction transferTransaction = std::get<1>(txVariant);
  const std::unordered_map<AccountId, Hbar> hbarTransfers = transferTransaction.getHbarTransfers();
  const std::unordered_map<TokenId, std::unordered_map<AccountId, int64_t>> tokenTransfers =
    transferTransaction.getTokenTransfers();
  const std::unordered_map<TokenId, std::vector<TokenNftTransfer>> nftTransfers = transferTransaction.getNftTransfers();
  const std::unordered_map<TokenId, uint32_t> tokenDecimals = transferTransaction.getTokenIdDecimals();

  ASSERT_EQ(hbarTransfers.size(), 1);
  EXPECT_EQ(hbarTransfers.cbegin()->first, getTestAccountId());
  EXPECT_EQ(hbarTransfers.cbegin()->second, getTestAmount());

  ASSERT_EQ(tokenTransfers.size(), 1);
  EXPECT_EQ(tokenTransfers.cbegin()->first, getTestTokenId());
  EXPECT_EQ(tokenTransfers.cbegin()->second.cbegin()->first, getTestAccountId());
  EXPECT_EQ(tokenTransfers.cbegin()->second.cbegin()->second, getTestAmount().toTinybars());

  ASSERT_EQ(nftTransfers.size(), 1);
  EXPECT_EQ(nftTransfers.cbegin()->first, getTestNftId().getTokenId());
  ASSERT_EQ(nftTransfers.cbegin()->second.size(), 1);
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getNftId(), getTestNftId());
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getSenderAccountId(), getTestAccountId());
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getReceiverAccountId(), getTestAccountId());
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getApproval(), getTestApproval());
}

//-----
TEST_F(TransactionTest, TransferTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptotransfer(
    std::make_unique<proto::CryptoTransferTransactionBody>(*getTestCryptoTransferTransactionBody()).release());

  proto::SignedTransaction signedTx;
  signedTx.set_allocated_bodybytes(new std::string(txBody.SerializeAsString()));
  // SignatureMap not required

  const std::string serialized = signedTx.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

  // Then
  ASSERT_EQ(index, 1);

  const TransferTransaction transferTransaction = std::get<1>(txVariant);
  const std::unordered_map<AccountId, Hbar> hbarTransfers = transferTransaction.getHbarTransfers();
  const std::unordered_map<TokenId, std::unordered_map<AccountId, int64_t>> tokenTransfers =
    transferTransaction.getTokenTransfers();
  const std::unordered_map<TokenId, std::vector<TokenNftTransfer>> nftTransfers = transferTransaction.getNftTransfers();
  const std::unordered_map<TokenId, uint32_t> tokenDecimals = transferTransaction.getTokenIdDecimals();

  ASSERT_EQ(hbarTransfers.size(), 1);
  EXPECT_EQ(hbarTransfers.cbegin()->first, getTestAccountId());
  EXPECT_EQ(hbarTransfers.cbegin()->second, getTestAmount());

  ASSERT_EQ(tokenTransfers.size(), 1);
  EXPECT_EQ(tokenTransfers.cbegin()->first, getTestTokenId());
  EXPECT_EQ(tokenTransfers.cbegin()->second.cbegin()->first, getTestAccountId());
  EXPECT_EQ(tokenTransfers.cbegin()->second.cbegin()->second, getTestAmount().toTinybars());

  ASSERT_EQ(nftTransfers.size(), 1);
  EXPECT_EQ(nftTransfers.cbegin()->first, getTestNftId().getTokenId());
  ASSERT_EQ(nftTransfers.cbegin()->second.size(), 1);
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getNftId(), getTestNftId());
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getSenderAccountId(), getTestAccountId());
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getReceiverAccountId(), getTestAccountId());
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getApproval(), getTestApproval());
}

//-----
TEST_F(TransactionTest, TransferTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptotransfer(
    std::make_unique<proto::CryptoTransferTransactionBody>(*getTestCryptoTransferTransactionBody()).release());

  proto::SignedTransaction signedTx;
  signedTx.set_allocated_bodybytes(new std::string(txBody.SerializeAsString()));
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_allocated_signedtransactionbytes(new std::string(signedTx.SerializeAsString()));

  const std::string serialized = tx.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

  // Then
  ASSERT_EQ(index, 1);

  const TransferTransaction transferTransaction = std::get<1>(txVariant);
  const std::unordered_map<AccountId, Hbar> hbarTransfers = transferTransaction.getHbarTransfers();
  const std::unordered_map<TokenId, std::unordered_map<AccountId, int64_t>> tokenTransfers =
    transferTransaction.getTokenTransfers();
  const std::unordered_map<TokenId, std::vector<TokenNftTransfer>> nftTransfers = transferTransaction.getNftTransfers();
  const std::unordered_map<TokenId, uint32_t> tokenDecimals = transferTransaction.getTokenIdDecimals();

  ASSERT_EQ(hbarTransfers.size(), 1);
  EXPECT_EQ(hbarTransfers.cbegin()->first, getTestAccountId());
  EXPECT_EQ(hbarTransfers.cbegin()->second, getTestAmount());

  ASSERT_EQ(tokenTransfers.size(), 1);
  EXPECT_EQ(tokenTransfers.cbegin()->first, getTestTokenId());
  EXPECT_EQ(tokenTransfers.cbegin()->second.cbegin()->first, getTestAccountId());
  EXPECT_EQ(tokenTransfers.cbegin()->second.cbegin()->second, getTestAmount().toTinybars());

  ASSERT_EQ(nftTransfers.size(), 1);
  EXPECT_EQ(nftTransfers.cbegin()->first, getTestNftId().getTokenId());
  ASSERT_EQ(nftTransfers.cbegin()->second.size(), 1);
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getNftId(), getTestNftId());
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getSenderAccountId(), getTestAccountId());
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getReceiverAccountId(), getTestAccountId());
  EXPECT_EQ(nftTransfers.cbegin()->second.cbegin()->getApproval(), getTestApproval());
}

//-----
TEST_F(TransactionTest, AccountUpdateTransactionFromTransactionBodyByte)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoupdateaccount(
    std::make_unique<proto::CryptoUpdateTransactionBody>(*getTestCryptoUpdateTransactionBody()).release());

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

  // Then
  ASSERT_EQ(index, 2);

  const AccountUpdateTransaction accountUpdateTransaction = std::get<2>(txVariant);
  EXPECT_EQ(accountUpdateTransaction.getAccountId(), getTestAccountId());
  EXPECT_EQ(accountUpdateTransaction.getKey()->toStringDer(), getTestPublicKey()->toStringDer());
  EXPECT_EQ(accountUpdateTransaction.getReceiverSignatureRequired(), getTestReceiverSignatureRequired());
  EXPECT_EQ(accountUpdateTransaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  EXPECT_EQ(accountUpdateTransaction.getExpirationTime(), getTestExpirationTime());
  EXPECT_EQ(accountUpdateTransaction.getAccountMemo(), getTestAccountMemo());
  EXPECT_EQ(accountUpdateTransaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
  EXPECT_FALSE(accountUpdateTransaction.getStakedAccountId().has_value());
  ASSERT_TRUE(accountUpdateTransaction.getStakedNodeId().has_value());
  EXPECT_EQ(*accountUpdateTransaction.getStakedNodeId(), getTestNodeId());
  EXPECT_EQ(accountUpdateTransaction.getDeclineStakingReward(), getTestDeclineStakingReward());
}

//-----
TEST_F(TransactionTest, AccountUpdateTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoupdateaccount(
    std::make_unique<proto::CryptoUpdateTransactionBody>(*getTestCryptoUpdateTransactionBody()).release());

  proto::SignedTransaction signedTx;
  signedTx.set_allocated_bodybytes(new std::string(txBody.SerializeAsString()));
  // SignatureMap not required

  const std::string serialized = signedTx.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

  // Then
  ASSERT_EQ(index, 2);

  const AccountUpdateTransaction accountUpdateTransaction = std::get<2>(txVariant);
  EXPECT_EQ(accountUpdateTransaction.getAccountId(), getTestAccountId());
  EXPECT_EQ(accountUpdateTransaction.getKey()->toStringDer(), getTestPublicKey()->toStringDer());
  EXPECT_EQ(accountUpdateTransaction.getReceiverSignatureRequired(), getTestReceiverSignatureRequired());
  EXPECT_EQ(accountUpdateTransaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  EXPECT_EQ(accountUpdateTransaction.getExpirationTime(), getTestExpirationTime());
  EXPECT_EQ(accountUpdateTransaction.getAccountMemo(), getTestAccountMemo());
  EXPECT_EQ(accountUpdateTransaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
  EXPECT_FALSE(accountUpdateTransaction.getStakedAccountId().has_value());
  ASSERT_TRUE(accountUpdateTransaction.getStakedNodeId().has_value());
  EXPECT_EQ(*accountUpdateTransaction.getStakedNodeId(), getTestNodeId());
  EXPECT_EQ(accountUpdateTransaction.getDeclineStakingReward(), getTestDeclineStakingReward());
}

//-----
TEST_F(TransactionTest, AccountUpdateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoupdateaccount(
    std::make_unique<proto::CryptoUpdateTransactionBody>(*getTestCryptoUpdateTransactionBody()).release());

  proto::SignedTransaction signedTx;
  signedTx.set_allocated_bodybytes(new std::string(txBody.SerializeAsString()));
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_allocated_signedtransactionbytes(new std::string(signedTx.SerializeAsString()));

  const std::string serialized = tx.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

  // Then
  ASSERT_EQ(index, 2);

  const AccountUpdateTransaction accountUpdateTransaction = std::get<2>(txVariant);
  EXPECT_EQ(accountUpdateTransaction.getAccountId(), getTestAccountId());
  EXPECT_EQ(accountUpdateTransaction.getKey()->toStringDer(), getTestPublicKey()->toStringDer());
  EXPECT_EQ(accountUpdateTransaction.getReceiverSignatureRequired(), getTestReceiverSignatureRequired());
  EXPECT_EQ(accountUpdateTransaction.getAutoRenewPeriod(), getTestAutoRenewPeriod());
  EXPECT_EQ(accountUpdateTransaction.getExpirationTime(), getTestExpirationTime());
  EXPECT_EQ(accountUpdateTransaction.getAccountMemo(), getTestAccountMemo());
  EXPECT_EQ(accountUpdateTransaction.getMaxAutomaticTokenAssociations(), getTestMaximumTokenAssociations());
  EXPECT_FALSE(accountUpdateTransaction.getStakedAccountId().has_value());
  ASSERT_TRUE(accountUpdateTransaction.getStakedNodeId().has_value());
  EXPECT_EQ(*accountUpdateTransaction.getStakedNodeId(), getTestNodeId());
  EXPECT_EQ(accountUpdateTransaction.getDeclineStakingReward(), getTestDeclineStakingReward());
}

//-----
TEST_F(TransactionTest, AccountDeleteTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodelete(
    std::make_unique<proto::CryptoDeleteTransactionBody>(*getTestCryptoDeleteTransactionBody()).release());

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

  // Then
  ASSERT_EQ(index, 3);

  const AccountDeleteTransaction accountDeleteTransaction = std::get<3>(txVariant);
  EXPECT_EQ(accountDeleteTransaction.getDeleteAccountId(), getTestAccountId());
  EXPECT_EQ(accountDeleteTransaction.getTransferAccountId(), getTestAccountId());
}

//-----
TEST_F(TransactionTest, AccountDeleteTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodelete(
    std::make_unique<proto::CryptoDeleteTransactionBody>(*getTestCryptoDeleteTransactionBody()).release());

  proto::SignedTransaction signedTx;
  signedTx.set_allocated_bodybytes(new std::string(txBody.SerializeAsString()));
  // SignatureMap not required

  const std::string serialized = signedTx.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

  // Then
  ASSERT_EQ(index, 3);

  const AccountDeleteTransaction accountDeleteTransaction = std::get<3>(txVariant);
  EXPECT_EQ(accountDeleteTransaction.getDeleteAccountId(), getTestAccountId());
  EXPECT_EQ(accountDeleteTransaction.getTransferAccountId(), getTestAccountId());
}

//-----
TEST_F(TransactionTest, AccountDeleteTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodelete(
    std::make_unique<proto::CryptoDeleteTransactionBody>(*getTestCryptoDeleteTransactionBody()).release());

  proto::SignedTransaction signedTx;
  signedTx.set_allocated_bodybytes(new std::string(txBody.SerializeAsString()));
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_allocated_signedtransactionbytes(new std::string(signedTx.SerializeAsString()));

  const std::string serialized = tx.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

  // Then
  ASSERT_EQ(index, 3);

  const AccountDeleteTransaction accountDeleteTransaction = std::get<3>(txVariant);
  EXPECT_EQ(accountDeleteTransaction.getDeleteAccountId(), getTestAccountId());
  EXPECT_EQ(accountDeleteTransaction.getTransferAccountId(), getTestAccountId());
}
