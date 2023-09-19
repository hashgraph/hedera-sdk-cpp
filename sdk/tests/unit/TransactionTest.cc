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
#include "FreezeTransaction.h"
#include "ScheduleCreateTransaction.h"
#include "ScheduleDeleteTransaction.h"
#include "ScheduleSignTransaction.h"
#include "SystemDeleteTransaction.h"
#include "SystemUndeleteTransaction.h"
#include "TokenAssociateTransaction.h"
#include "TokenBurnTransaction.h"
#include "TokenCreateTransaction.h"
#include "TokenDeleteTransaction.h"
#include "TokenDissociateTransaction.h"
#include "TokenFeeScheduleUpdateTransaction.h"
#include "TokenFreezeTransaction.h"
#include "TokenGrantKycTransaction.h"
#include "TokenMintTransaction.h"
#include "TokenPauseTransaction.h"
#include "TokenRevokeKycTransaction.h"
#include "TokenUnfreezeTransaction.h"
#include "TokenUnpauseTransaction.h"
#include "TokenUpdateTransaction.h"
#include "TokenWipeTransaction.h"
#include "TopicCreateTransaction.h"
#include "TopicDeleteTransaction.h"
#include "TopicMessageSubmitTransaction.h"
#include "TopicUpdateTransaction.h"
#include "TransactionType.h"
#include "TransferTransaction.h"
#include "WrappedTransaction.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <proto/transaction.pb.h>
#include <proto/transaction_body.pb.h>
#include <proto/transaction_contents.pb.h>
#include <proto/transaction_list.pb.h>

using namespace Hedera;

class TransactionTest : public ::testing::Test
{
};

//-----
TEST_F(TransactionTest, AccountApproveAllowanceFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoapproveallowance(new proto::CryptoApproveAllowanceTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<AccountAllowanceApproveTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountAllowanceApproveTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx = Transaction<AccountAllowanceApproveTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountAllowanceApproveTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, AccountApproveAllowanceFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoapproveallowance(new proto::CryptoApproveAllowanceTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<AccountAllowanceApproveTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_ALLOWANCE_APPROVE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountAllowanceApproveTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, AccountDeleteAllowanceFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodeleteallowance(new proto::CryptoDeleteAllowanceTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<AccountAllowanceDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_ALLOWANCE_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountAllowanceDeleteTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx = Transaction<AccountAllowanceDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_ALLOWANCE_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountAllowanceDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, AccountDeleteAllowanceFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodeleteallowance(new proto::CryptoDeleteAllowanceTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<AccountAllowanceDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_ALLOWANCE_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountAllowanceDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, AccountCreateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptocreateaccount(new proto::CryptoCreateTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<AccountCreateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountCreateTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<AccountCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, AccountCreateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptocreateaccount(new proto::CryptoCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<AccountCreateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, AccountDeleteTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodelete(new proto::CryptoDeleteTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<AccountDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountDeleteTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<AccountDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, AccountDeleteTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptodelete(new proto::CryptoDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<AccountDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, AccountUpdateTransactionFromTransactionBodyByte)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoupdateaccount(new proto::CryptoUpdateTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<AccountUpdateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountUpdateTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<AccountUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, AccountUpdateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptoupdateaccount(new proto::CryptoUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<AccountUpdateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ACCOUNT_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<AccountUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ContractCreateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractcreateinstance(new proto::ContractCreateTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<ContractCreateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::CONTRACT_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ContractCreateTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<ContractCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::CONTRACT_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ContractCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ContractCreateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractcreateinstance(new proto::ContractCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<ContractCreateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::CONTRACT_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ContractCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ContractDeleteTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractdeleteinstance(new proto::ContractDeleteTransactionBody());

  // When
  const WrappedTransaction wrappedTx = Transaction<ContractDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::CONTRACT_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ContractDeleteTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<ContractDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::CONTRACT_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ContractDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ContractDeleteTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractdeleteinstance(new proto::ContractDeleteTransactionBody());

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<ContractDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::CONTRACT_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ContractDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ContractExecuteTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractcall(new proto::ContractCallTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<ContractExecuteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::CONTRACT_EXECUTE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ContractExecuteTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<ContractExecuteTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::CONTRACT_EXECUTE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ContractExecuteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ContractExecuteTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractcall(new proto::ContractCallTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<ContractExecuteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::CONTRACT_EXECUTE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ContractExecuteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ContractUpdateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractupdateinstance(new proto::ContractUpdateTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<ContractUpdateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::CONTRACT_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ContractUpdateTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<ContractUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::CONTRACT_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ContractUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ContractUpdateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_contractupdateinstance(new proto::ContractUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<ContractUpdateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::CONTRACT_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ContractUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, EthereumTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_ethereumtransaction(new proto::EthereumTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<EthereumTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ETHEREUM_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<EthereumTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<EthereumTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ETHEREUM_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<EthereumTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, EthereumTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_ethereumtransaction(new proto::EthereumTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<EthereumTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::ETHEREUM_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<EthereumTransaction>(), nullptr);
}

TEST_F(TransactionTest, FileAppendTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_fileappend(new proto::FileAppendTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<FileAppendTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FILE_APPEND_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FileAppendTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<FileAppendTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FILE_APPEND_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FileAppendTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, FileAppendTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_fileappend(new proto::FileAppendTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<FileAppendTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FILE_APPEND_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FileAppendTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, FileCreateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_filecreate(new proto::FileCreateTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<FileCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FILE_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FileCreateTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<FileCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FILE_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FileCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, FileCreateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_filecreate(new proto::FileCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<FileCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FILE_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FileCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, FileDeleteTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_filedelete(new proto::FileDeleteTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<FileDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FILE_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FileDeleteTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<FileDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FILE_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FileDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, FileDeleteTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_filedelete(new proto::FileDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<FileDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FILE_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FileDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, FileUpdateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_fileupdate(new proto::FileUpdateTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<FileUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FILE_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FileUpdateTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<FileUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FILE_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FileUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, FileUpdateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_fileupdate(new proto::FileUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<FileUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FILE_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FileUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, FreezeTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_freeze(new proto::FreezeTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<FreezeTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FREEZE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FreezeTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, FreezeTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_freeze(new proto::FreezeTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx =
    Transaction<FreezeTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FREEZE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FreezeTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, FreezeTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_freeze(new proto::FreezeTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<FreezeTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::FREEZE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<FreezeTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ScheduleCreateTransactionTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_schedulecreate(new proto::ScheduleCreateTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<ScheduleCreateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SCHEDULE_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ScheduleCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ScheduleCreateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_schedulecreate(new proto::ScheduleCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx =
    Transaction<ScheduleCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SCHEDULE_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ScheduleCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ScheduleCreateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_schedulecreate(new proto::ScheduleCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<ScheduleCreateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SCHEDULE_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ScheduleCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ScheduleDeleteTransactionTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_scheduledelete(new proto::ScheduleDeleteTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<ScheduleDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SCHEDULE_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ScheduleDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ScheduleDeleteTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_scheduledelete(new proto::ScheduleDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx =
    Transaction<ScheduleDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SCHEDULE_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ScheduleDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ScheduleDeleteTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_scheduledelete(new proto::ScheduleDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<ScheduleDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SCHEDULE_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ScheduleDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ScheduleSignTransactionTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_schedulesign(new proto::ScheduleSignTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<ScheduleSignTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SCHEDULE_SIGN_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ScheduleSignTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ScheduleSignTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_schedulesign(new proto::ScheduleSignTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx =
    Transaction<ScheduleSignTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SCHEDULE_SIGN_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ScheduleSignTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, ScheduleSignTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_schedulesign(new proto::ScheduleSignTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<ScheduleSignTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SCHEDULE_SIGN_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<ScheduleSignTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, SystemDeleteTransactionTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_systemdelete(new proto::SystemDeleteTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<SystemDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SYSTEM_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<SystemDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, SystemDeleteTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_systemdelete(new proto::SystemDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx =
    Transaction<SystemDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SYSTEM_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<SystemDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, SystemDeleteTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_systemdelete(new proto::SystemDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<SystemDeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SYSTEM_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<SystemDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, SystemUndeleteTransactionTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_systemundelete(new proto::SystemUndeleteTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<SystemUndeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SYSTEM_UNDELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<SystemUndeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, SystemUndeleteTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_systemundelete(new proto::SystemUndeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx =
    Transaction<SystemUndeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SYSTEM_UNDELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<SystemUndeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, SystemUndeleteTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_systemundelete(new proto::SystemUndeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<SystemUndeleteTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::SYSTEM_UNDELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<SystemUndeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenAssociateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenassociate(new proto::TokenAssociateTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenAssociateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_ASSOCIATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenAssociateTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx = Transaction<TokenAssociateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_ASSOCIATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenAssociateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenAssociateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenassociate(new proto::TokenAssociateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenAssociateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_ASSOCIATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenAssociateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenBurnTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenburn(new proto::TokenBurnTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenBurnTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_BURN_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenBurnTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenBurnTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenburn(new proto::TokenBurnTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenBurnTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_BURN_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenBurnTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenBurnTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenburn(new proto::TokenBurnTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenBurnTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_BURN_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenBurnTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenCreateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokencreation(new proto::TokenCreateTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenCreateTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<TokenCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenCreateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokencreation(new proto::TokenCreateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenDeleteTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokendeletion(new proto::TokenDeleteTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenDeleteTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<TokenDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenDeleteTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokendeletion(new proto::TokenDeleteTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenDissociateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokendissociate(new proto::TokenDissociateTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenDissociateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_DISSOCIATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenDissociateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenDissociateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokendissociate(new proto::TokenDissociateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenDissociateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_DISSOCIATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenDissociateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenDissociateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokendissociate(new proto::TokenDissociateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenDissociateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_DISSOCIATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenDissociateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenFeeScheduleUpdateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_token_fee_schedule_update(new proto::TokenFeeScheduleUpdateTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenFeeScheduleUpdateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_FEE_SCHEDULE_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenFeeScheduleUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenFeeScheduleUpdateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_token_fee_schedule_update(new proto::TokenFeeScheduleUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenFeeScheduleUpdateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_FEE_SCHEDULE_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenFeeScheduleUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenFeeScheduleUpdateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_token_fee_schedule_update(new proto::TokenFeeScheduleUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenFeeScheduleUpdateTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_FEE_SCHEDULE_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenFeeScheduleUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenFreezeTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenfreeze(new proto::TokenFreezeAccountTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenFreezeTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_FREEZE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenFreezeTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenFreezeTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenfreeze(new proto::TokenFreezeAccountTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenFreezeTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_FREEZE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenFreezeTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenFreezeTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenfreeze(new proto::TokenFreezeAccountTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenFreezeTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_FREEZE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenFreezeTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenGrantKycTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokengrantkyc(new proto::TokenGrantKycTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenGrantKycTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_GRANT_KYC_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenGrantKycTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenGrantKycTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokengrantkyc(new proto::TokenGrantKycTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenGrantKycTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_GRANT_KYC_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenGrantKycTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenGrantKycTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokengrantkyc(new proto::TokenGrantKycTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenGrantKycTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_GRANT_KYC_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenGrantKycTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenMintTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenmint(new proto::TokenMintTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenMintTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_MINT_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenMintTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<TokenMintTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_MINT_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenMintTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenMintTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenmint(new proto::TokenMintTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenMintTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_MINT_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenMintTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenPauseTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_token_pause(new proto::TokenPauseTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenPauseTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_PAUSE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenPauseTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenPauseTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_token_pause(new proto::TokenPauseTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenPauseTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_PAUSE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenPauseTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenPauseTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_token_pause(new proto::TokenPauseTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenPauseTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_PAUSE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenPauseTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenRevokeKycTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenrevokekyc(new proto::TokenRevokeKycTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenRevokeKycTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_REVOKE_KYC_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenRevokeKycTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenRevokeKycTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenrevokekyc(new proto::TokenRevokeKycTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenRevokeKycTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_REVOKE_KYC_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenRevokeKycTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenRevokeKycTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenrevokekyc(new proto::TokenRevokeKycTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenRevokeKycTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_REVOKE_KYC_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenRevokeKycTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenUnfreezeTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenunfreeze(new proto::TokenUnfreezeAccountTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenUnfreezeTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_UNFREEZE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenUnfreezeTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenUnfreezeTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenunfreeze(new proto::TokenUnfreezeAccountTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenUnfreezeTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_UNFREEZE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenUnfreezeTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenUnfreezeTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenunfreeze(new proto::TokenUnfreezeAccountTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenUnfreezeTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_UNFREEZE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenUnfreezeTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenUnpauseTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_token_unpause(new proto::TokenUnpauseTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenUnpauseTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_UNPAUSE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenUnpauseTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenUnpauseTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_token_unpause(new proto::TokenUnpauseTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenUnpauseTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_UNPAUSE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenUnpauseTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenUnpauseTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_token_unpause(new proto::TokenUnpauseTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<TokenUnpauseTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_UNPAUSE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenUnpauseTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenUpdateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenupdate(new proto::TokenUpdateTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenUpdateTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<TokenUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenUpdateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenupdate(new proto::TokenUpdateTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenWipeTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenwipe(new proto::TokenWipeAccountTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenWipeTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_WIPE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenWipeTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<TokenWipeTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_WIPE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenWipeTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TokenWipeTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_tokenwipe(new proto::TokenWipeAccountTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TokenWipeTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOKEN_WIPE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TokenWipeTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TopicCreateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_consensuscreatetopic(new proto::ConsensusCreateTopicTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TopicCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOPIC_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TopicCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TopicCreateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_consensuscreatetopic(new proto::ConsensusCreateTopicTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TopicCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOPIC_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TopicCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TopicCreateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_consensuscreatetopic(new proto::ConsensusCreateTopicTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TopicCreateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOPIC_CREATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TopicCreateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TopicDeleteTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_consensusdeletetopic(new proto::ConsensusDeleteTopicTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TopicDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOPIC_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TopicDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TopicDeleteTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_consensusdeletetopic(new proto::ConsensusDeleteTopicTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TopicDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOPIC_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TopicDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TopicDeleteTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_consensusdeletetopic(new proto::ConsensusDeleteTopicTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TopicDeleteTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOPIC_DELETE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TopicDeleteTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TopicMessageSubmitTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_consensussubmitmessage(new proto::ConsensusSubmitMessageTransactionBody);

  // When
  const WrappedTransaction wrappedTx = Transaction<TopicMessageSubmitTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOPIC_MESSAGE_SUBMIT_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TopicMessageSubmitTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TopicMessageSubmitTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_consensussubmitmessage(new proto::ConsensusSubmitMessageTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx = Transaction<TopicMessageSubmitTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOPIC_MESSAGE_SUBMIT_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TopicMessageSubmitTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TopicMessageSubmitTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_consensussubmitmessage(new proto::ConsensusSubmitMessageTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx = Transaction<TopicMessageSubmitTransaction>::fromBytes(
    internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOPIC_MESSAGE_SUBMIT_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TopicMessageSubmitTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TopicUpdateTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_consensusupdatetopic(new proto::ConsensusUpdateTopicTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TopicUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOPIC_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TopicUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TopicUpdateTransactionFromTransactionBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_consensusupdatetopic(new proto::ConsensusUpdateTopicTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TopicUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOPIC_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TopicUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TopicUpdateTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_consensusupdatetopic(new proto::ConsensusUpdateTopicTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TopicUpdateTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TOPIC_UPDATE_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TopicUpdateTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TransferTransactionFromTransactionBodyBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptotransfer(new proto::CryptoTransferTransactionBody);

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TransferTransaction>::fromBytes(internal::Utilities::stringToByteVector(txBody.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TRANSFER_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TransferTransaction>(), nullptr);
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
  const WrappedTransaction wrappedTx =
    Transaction<TransferTransaction>::fromBytes(internal::Utilities::stringToByteVector(tx.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TRANSFER_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TransferTransaction>(), nullptr);
}

//-----
TEST_F(TransactionTest, TransferTransactionFromTransactionListBytes)
{
  // Given
  proto::TransactionBody txBody;
  txBody.set_allocated_cryptotransfer(new proto::CryptoTransferTransactionBody);

  proto::SignedTransaction signedTx;
  signedTx.set_bodybytes(txBody.SerializeAsString());
  // SignatureMap not required

  proto::Transaction tx;
  tx.set_signedtransactionbytes(signedTx.SerializeAsString());

  proto::TransactionList txList;
  *txList.add_transaction_list() = tx;

  // When
  const WrappedTransaction wrappedTx =
    Transaction<TransferTransaction>::fromBytes(internal::Utilities::stringToByteVector(txList.SerializeAsString()));

  // Then
  ASSERT_EQ(wrappedTx.getTransactionType(), TransactionType::TRANSFER_TRANSACTION);
  EXPECT_NE(wrappedTx.getTransaction<TransferTransaction>(), nullptr);
}
