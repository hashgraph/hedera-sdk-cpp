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
#include "ContractUpdateTransaction.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
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

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

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

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<TransferTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

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

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

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

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

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

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountAllowanceApproveTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

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

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<AccountAllowanceDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

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

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<ContractCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

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

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<ContractDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

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

  const std::string serialized = txBody.SerializeAsString();

  // When
  const auto [index, txVariant] =
    Transaction<FileCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(serialized));

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
TEST_F(TransactionTest, ContractUpdateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractupdateinstance(new proto::ContractUpdateTransactionBody);

  // When
  const auto [index, txVariant] = Transaction<ContractUpdateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(index, 10);
  EXPECT_NO_THROW(const ContractUpdateTransaction contractUpdateTransaction = std::get<10>(txVariant));
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
  ASSERT_EQ(index, 10);
  EXPECT_NO_THROW(const ContractUpdateTransaction contractUpdateTransaction = std::get<10>(txVariant));
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
  ASSERT_EQ(index, 10);
  EXPECT_NO_THROW(const ContractUpdateTransaction contractUpdateTransaction = std::get<10>(txVariant));
}
