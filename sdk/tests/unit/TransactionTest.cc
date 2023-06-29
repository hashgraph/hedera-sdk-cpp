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
#include "AccountAllowanceApproveTransaction.h"
#include "AccountAllowanceDeleteTransaction.h"
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "AccountUpdateTransaction.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractExecuteTransaction.h"
#include "ContractUpdateTransaction.h"
#include "EthereumTransaction.h"
#include "FileAppendTransaction.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileUpdateTransaction.h"
#include "TokenAssociateTransaction.h"
#include "TokenCreateTransaction.h"
#include "TokenDeleteTransaction.h"
#include "TokenMintTransaction.h"
#include "TokenUpdateTransaction.h"
#include "TokenWipeTransaction.h"
#include "TransferTransaction.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>
#include <proto/transaction_contents.pb.h>

using namespace Hedera;

class TransactionTest : public ::testing::Test
{
};

//-----
TEST_F(TransactionTest, AccountCreateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptocreateaccount(new proto::CryptoCreateTransactionBody);

  // When
  const auto [index, txVariant] = Transaction<AccountCreateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 0);
  EXPECT_NO_THROW(const AccountCreateTransaction accountCreateTransaction = std::get<0>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountCreateTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptocreateaccount(new proto::CryptoCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] = Transaction<AccountCreateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 0);
  EXPECT_NO_THROW(const AccountCreateTransaction accountCreateTransaction = std::get<0>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountCreateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptocreateaccount(new proto::CryptoCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<AccountCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 0);
  EXPECT_NO_THROW(const AccountCreateTransaction accountCreateTransaction = std::get<0>(txVariant));
}

//-----
TEST_F(TransactionTest, TransferTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptotransfer(new proto::CryptoTransferTransactionBody);

  // When
  const auto [index, txVariant] =
    Transaction<TransferTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 1);
  EXPECT_NO_THROW(const TransferTransaction transferTransaction = std::get<1>(txVariant));
}

//-----
TEST_F(TransactionTest, TransferTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptotransfer(new proto::CryptoTransferTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] =
    Transaction<TransferTransaction>::fromBytes(internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 1);
  EXPECT_NO_THROW(const TransferTransaction transferTransaction = std::get<1>(txVariant));
}

//-----
TEST_F(TransactionTest, TransferTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptotransfer(new proto::CryptoTransferTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<TransferTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 1);
  EXPECT_NO_THROW(const TransferTransaction transferTransaction = std::get<1>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountUpdateTransactionFromTransactionBodyByte)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoupdateaccount(new proto::CryptoUpdateTransactionBody);

  // When
  const auto [index, txVariant] = Transaction<AccountUpdateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 2);
  EXPECT_NO_THROW(const AccountUpdateTransaction accountUpdateTransaction = std::get<2>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountUpdateTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoupdateaccount(new proto::CryptoUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] = Transaction<AccountUpdateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 2);
  EXPECT_NO_THROW(const AccountUpdateTransaction accountUpdateTransaction = std::get<2>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountUpdateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoupdateaccount(new proto::CryptoUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<AccountUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 2);
  EXPECT_NO_THROW(const AccountUpdateTransaction accountUpdateTransaction = std::get<2>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountDeleteTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodelete(new proto::CryptoDeleteTransactionBody);

  // When
  const auto [index, txVariant] = Transaction<AccountDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 3);
  EXPECT_NO_THROW(const AccountDeleteTransaction accountDeleteTransaction = std::get<3>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountDeleteTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodelete(new proto::CryptoDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] = Transaction<AccountDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 3);
  EXPECT_NO_THROW(const AccountDeleteTransaction accountDeleteTransaction = std::get<3>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountDeleteTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodelete(new proto::CryptoDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<AccountDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 3);
  EXPECT_NO_THROW(const AccountDeleteTransaction accountDeleteTransaction = std::get<3>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountApproveAllowanceFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoapproveallowance(new proto::CryptoApproveAllowanceTransactionBody);

  // When
  const auto [index, txVariant] = Transaction<AccountAllowanceApproveTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 4);
  EXPECT_NO_THROW(const AccountAllowanceApproveTransaction accountAllowanceApproveTransaction = std::get<4>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountApproveAllowanceFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoapproveallowance(new proto::CryptoApproveAllowanceTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] = Transaction<AccountAllowanceApproveTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 4);
  EXPECT_NO_THROW(const AccountAllowanceApproveTransaction accountAllowanceApproveTransaction = std::get<4>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountApproveAllowanceFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoapproveallowance(new proto::CryptoApproveAllowanceTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] = Transaction<AccountAllowanceApproveTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 4);
  EXPECT_NO_THROW(const AccountAllowanceApproveTransaction accountAllowanceApproveTransaction = std::get<4>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountDeleteAllowanceFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodeleteallowance(new proto::CryptoDeleteAllowanceTransactionBody);

  // When
  const auto [index, txVariant] = Transaction<AccountAllowanceDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 5);
  EXPECT_NO_THROW(const AccountAllowanceDeleteTransaction accountAllowanceDeleteTransaction = std::get<5>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountDeleteAllowanceFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodeleteallowance(new proto::CryptoDeleteAllowanceTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] = Transaction<AccountAllowanceDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 5);
  EXPECT_NO_THROW(const AccountAllowanceDeleteTransaction accountAllowanceDeleteTransaction = std::get<5>(txVariant));
}

//-----
TEST_F(TransactionTest, AccountDeleteAllowanceFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodeleteallowance(new proto::CryptoDeleteAllowanceTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] = Transaction<AccountAllowanceDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 5);
  EXPECT_NO_THROW(const AccountAllowanceDeleteTransaction accountAllowanceDeleteTransaction = std::get<5>(txVariant));
}

//-----
TEST_F(TransactionTest, ContractCreateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractcreateinstance(new proto::ContractCreateTransactionBody);

  // When
  const auto [index, txVariant] = Transaction<ContractCreateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 6);
  EXPECT_NO_THROW(const ContractCreateTransaction contractCreateTransaction = std::get<6>(txVariant));
}

//-----
TEST_F(TransactionTest, ContractCreateTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractcreateinstance(new proto::ContractCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] = Transaction<ContractCreateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 6);
  EXPECT_NO_THROW(const ContractCreateTransaction contractCreateTransaction = std::get<6>(txVariant));
}

//-----
TEST_F(TransactionTest, ContractCreateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractcreateinstance(new proto::ContractCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<ContractCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 6);
  EXPECT_NO_THROW(const ContractCreateTransaction contractCreateTransaction = std::get<6>(txVariant));
}

//-----
TEST_F(TransactionTest, ContractDeleteTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractdeleteinstance(new proto::ContractDeleteTransactionBody());

  // When
  const auto [index, txVariant] = Transaction<ContractDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 7);
  EXPECT_NO_THROW(const ContractDeleteTransaction contractDeleteTransaction = std::get<7>(txVariant));
}

//-----
TEST_F(TransactionTest, ContractDeleteTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractdeleteinstance(new proto::ContractDeleteTransactionBody());

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] = Transaction<ContractDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 7);
  EXPECT_NO_THROW(const ContractDeleteTransaction contractDeleteTransaction = std::get<7>(txVariant));
}

//-----
TEST_F(TransactionTest, ContractDeleteTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractdeleteinstance(new proto::ContractDeleteTransactionBody());

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<ContractDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 7);
  EXPECT_NO_THROW(const ContractDeleteTransaction contractDeleteTransaction = std::get<7>(txVariant));
}

//-----
TEST_F(TransactionTest, FileCreateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_filecreate(new proto::FileCreateTransactionBody);

  // When
  const auto [index, txVariant] =
    Transaction<FileCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 8);
  EXPECT_NO_THROW(const FileCreateTransaction fileCreateTransaction = std::get<8>(txVariant));
}

//-----
TEST_F(TransactionTest, FileCreateTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_filecreate(new proto::FileCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] = Transaction<FileCreateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 8);
  EXPECT_NO_THROW(const FileCreateTransaction fileCreateTransaction = std::get<8>(txVariant));
}

//-----
TEST_F(TransactionTest, FileCreateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_filecreate(new proto::FileCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<FileCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 8);
  EXPECT_NO_THROW(const FileCreateTransaction fileCreateTransaction = std::get<8>(txVariant));
}

//-----
TEST_F(TransactionTest, FileDeleteTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_filedelete(new proto::FileDeleteTransactionBody);

  // When
  const auto [index, txVariant] =
    Transaction<FileDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 9);
  EXPECT_NO_THROW(const FileDeleteTransaction fileDeleteTransaction = std::get<9>(txVariant));
}

//-----
TEST_F(TransactionTest, FileDeleteTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_filedelete(new proto::FileDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] = Transaction<FileDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 9);
  EXPECT_NO_THROW(const FileDeleteTransaction fileDeleteTransaction = std::get<9>(txVariant));
}

//-----
TEST_F(TransactionTest, FileDeleteTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_filedelete(new proto::FileDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<FileDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 9);
  EXPECT_NO_THROW(const FileDeleteTransaction fileDeleteTransaction = std::get<9>(txVariant));
}

//-----
TEST_F(TransactionTest, ContractExecuteTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractcall(new proto::ContractCallTransactionBody);

  // When
  const auto [index, txVariant] = Transaction<ContractExecuteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 10);
  EXPECT_NO_THROW(const ContractExecuteTransaction contractExecuteTransaction = std::get<10>(txVariant));
}

//-----
TEST_F(TransactionTest, ContractExecuteTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractcall(new proto::ContractCallTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] = Transaction<ContractExecuteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 10);
  EXPECT_NO_THROW(const ContractExecuteTransaction contractExecuteTransaction = std::get<10>(txVariant));
}

//-----
TEST_F(TransactionTest, ContractExecuteTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractcall(new proto::ContractCallTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<ContractExecuteTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 10);
  EXPECT_NO_THROW(const ContractExecuteTransaction contractExecuteTransaction = std::get<10>(txVariant));
}

//-----
TEST_F(TransactionTest, ContractUpdateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractupdateinstance(new proto::ContractUpdateTransactionBody);

  // When
  const auto [index, txVariant] = Transaction<ContractUpdateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 11);
  EXPECT_NO_THROW(const ContractUpdateTransaction contractUpdateTransaction = std::get<11>(txVariant));
}

//-----
TEST_F(TransactionTest, ContractUpdateTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractupdateinstance(new proto::ContractUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] = Transaction<ContractUpdateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 11);
  EXPECT_NO_THROW(const ContractUpdateTransaction contractUpdateTransaction = std::get<11>(txVariant));
}

//-----
TEST_F(TransactionTest, ContractUpdateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractupdateinstance(new proto::ContractUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<ContractUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 11);
  EXPECT_NO_THROW(const ContractUpdateTransaction contractUpdateTransaction = std::get<11>(txVariant));
}

//-----
TEST_F(TransactionTest, EthereumTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_ethereumtransaction(new proto::EthereumTransactionBody);

  // When
  const auto [index, txVariant] =
    Transaction<EthereumTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 12);
  EXPECT_NO_THROW(const EthereumTransaction ethereumTransaction = std::get<12>(txVariant));
}

//-----
TEST_F(TransactionTest, EthereumTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_ethereumtransaction(new proto::EthereumTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] =
    Transaction<EthereumTransaction>::fromBytes(internal::Utilities::stringToByteVector(signedTx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 12);
  EXPECT_NO_THROW(const EthereumTransaction ethereumTransaction = std::get<12>(txVariant));
}

//-----
TEST_F(TransactionTest, EthereumTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_ethereumtransaction(new proto::EthereumTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<EthereumTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 12);
  EXPECT_NO_THROW(const EthereumTransaction ethereumTransaction = std::get<12>(txVariant));
}

//-----
TEST_F(TransactionTest, FileUpdateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_fileupdate(new proto::FileUpdateTransactionBody);

  // When
  const auto [index, txVariant] =
    Transaction<FileUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 13);
  EXPECT_NO_THROW(const FileUpdateTransaction fileUpdateTransaction = std::get<13>(txVariant));
}

//-----
TEST_F(TransactionTest, FileUpdateTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_fileupdate(new proto::FileUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] =
    Transaction<FileUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 13);
  EXPECT_NO_THROW(const FileUpdateTransaction fileUpdateTransaction = std::get<13>(txVariant));
}

//-----
TEST_F(TransactionTest, FileUpdateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_fileupdate(new proto::FileUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<FileUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 13);
  EXPECT_NO_THROW(const FileUpdateTransaction fileUpdateTransaction = std::get<13>(txVariant));
}

TEST_F(TransactionTest, FileAppendTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_fileappend(new proto::FileAppendTransactionBody);

  // When
  const auto [index, txVariant] =
    Transaction<FileAppendTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 14);
  EXPECT_NO_THROW(const FileAppendTransaction fileAppendTransaction = std::get<14>(txVariant));
}

//-----
TEST_F(TransactionTest, FileAppendTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_fileappend(new proto::FileAppendTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] =
    Transaction<FileAppendTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 14);
  EXPECT_NO_THROW(const FileAppendTransaction fileAppendTransaction = std::get<14>(txVariant));
}

//-----
TEST_F(TransactionTest, FileAppendTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_fileappend(new proto::FileAppendTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<FileAppendTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 14);
  EXPECT_NO_THROW(const FileAppendTransaction fileAppendTransaction = std::get<14>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenCreateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokencreation(new proto::TokenCreateTransactionBody);

  // When
  const auto [index, txVariant] =
    Transaction<TokenCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 15);
  EXPECT_NO_THROW(const TokenCreateTransaction tokenCreateTransaction = std::get<15>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenCreateTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokencreation(new proto::TokenCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] =
    Transaction<TokenCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 15);
  EXPECT_NO_THROW(const TokenCreateTransaction tokenCreateTransaction = std::get<15>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenCreateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokencreation(new proto::TokenCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<TokenCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 15);
  EXPECT_NO_THROW(const TokenCreateTransaction tokenCreateTransaction = std::get<15>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenDeleteTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokendeletion(new proto::TokenDeleteTransactionBody);

  // When
  const auto [index, txVariant] =
    Transaction<TokenDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 16);
  EXPECT_NO_THROW(const TokenDeleteTransaction tokenDeleteTransaction = std::get<16>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenDeleteTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokendeletion(new proto::TokenDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] =
    Transaction<TokenDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 16);
  EXPECT_NO_THROW(const TokenDeleteTransaction tokenDeleteTransaction = std::get<16>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenDeleteTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokendeletion(new proto::TokenDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<TokenDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 16);
  EXPECT_NO_THROW(const TokenDeleteTransaction tokenDeleteTransaction = std::get<16>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenAssociateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenassociate(new proto::TokenAssociateTransactionBody);

  // When
  const auto [index, txVariant] = Transaction<TokenAssociateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 17);
  EXPECT_NO_THROW(const TokenAssociateTransaction tokenAssociateTransaction = std::get<17>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenAssociateTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenassociate(new proto::TokenAssociateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] = Transaction<TokenAssociateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 17);
  EXPECT_NO_THROW(const TokenAssociateTransaction tokenAssociateTransaction = std::get<17>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenAssociateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenassociate(new proto::TokenAssociateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] = Transaction<TokenAssociateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 17);
  EXPECT_NO_THROW(const TokenAssociateTransaction tokenAssociateTransaction = std::get<17>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenMintTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenmint(new proto::TokenMintTransactionBody);

  // When
  const auto [index, txVariant] =
    Transaction<TokenMintTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 18);
  EXPECT_NO_THROW(const TokenMintTransaction tokenMintTransaction = std::get<18>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenMintTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenmint(new proto::TokenMintTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] =
    Transaction<TokenMintTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 18);
  EXPECT_NO_THROW(const TokenMintTransaction tokenMintTransaction = std::get<18>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenMintTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenmint(new proto::TokenMintTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<TokenMintTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 18);
  EXPECT_NO_THROW(const TokenMintTransaction tokenMintTransaction = std::get<18>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenUpdateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenupdate(new proto::TokenUpdateTransactionBody);

  // When
  const auto [index, txVariant] =
    Transaction<TokenUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 19);
  EXPECT_NO_THROW(const TokenUpdateTransaction tokenUpdateTransaction = std::get<19>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenUpdateTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenupdate(new proto::TokenUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] =
    Transaction<TokenUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 19);
  EXPECT_NO_THROW(const TokenUpdateTransaction tokenUpdateTransaction = std::get<19>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenUpdateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenupdate(new proto::TokenUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<TokenUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 19);
  EXPECT_NO_THROW(const TokenUpdateTransaction tokenUpdateTransaction = std::get<19>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenWipeTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenwipe(new proto::TokenWipeAccountTransactionBody);

  // When
  const auto [index, txVariant] =
    Transaction<TokenWipeTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 20);
  EXPECT_NO_THROW(const TokenWipeTransaction tokenWipeTransaction = std::get<20>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenWipeTransactionFromSignedTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenwipe(new proto::TokenWipeAccountTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  // When
  const auto [index, txVariant] =
    Transaction<TokenWipeTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 20);
  EXPECT_NO_THROW(const TokenWipeTransaction tokenWipeTransaction = std::get<20>(txVariant));
}

//-----
TEST_F(TransactionTest, TokenWipeTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenwipe(new proto::TokenWipeAccountTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const auto [index, txVariant] =
    Transaction<TokenWipeTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 20);
  EXPECT_NO_THROW(const TokenWipeTransaction tokenWipeTransaction = std::get<20>(txVariant));
}
