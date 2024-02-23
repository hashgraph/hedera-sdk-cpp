/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "AccountInfo.h"
#include "AccountInfoQuery.h"
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractFunctionParameters.h"
#include "ContractId.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "ED25519PrivateKey.h"
#include "EthereumTransaction.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileId.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "TransferTransaction.h"
#include "exceptions/OpenSSLException.h"
#include "impl/HexConverter.h"
#include "impl/RLPItem.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <vector>

using namespace Hedera;

class EthereumTransactionIntegrationTests : public BaseIntegrationTest
{
};

//-----
TEST_F(EthereumTransactionIntegrationTests, SignerNonceChangedOnEthereumTransaction)
{
  // Given
  const std::shared_ptr<ECDSAsecp256k1PrivateKey> testPrivateKey = ECDSAsecp256k1PrivateKey::generatePrivateKey();
  const std::shared_ptr<ECDSAsecp256k1PublicKey> testPublicKey =
    std::dynamic_pointer_cast<ECDSAsecp256k1PublicKey>(testPrivateKey->getPublicKey());
  const AccountId aliasAccountId = testPublicKey->toAccountId();

  TransactionReceipt aliasTransferTxReciept;
  EXPECT_NO_THROW(aliasTransferTxReciept =
                    TransferTransaction()
                      .addHbarTransfer(getTestClient().getOperatorAccountId().value(), Hbar(1LL).negated())
                      .addHbarTransfer(aliasAccountId, Hbar(1LL))
                      .execute(getTestClient())
                      .getReceipt(getTestClient()));

  AccountInfo accountInfo;
  EXPECT_NO_THROW(accountInfo = AccountInfoQuery().setAccountId(aliasAccountId).execute(getTestClient()));

  FileId fileId;
  EXPECT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ getTestClient().getOperatorPublicKey() })
                             .setContents(internal::Utilities::stringToByteVector(getTestSmartContractBytecode()))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .mFileId.value());

  const std::string memo = "[e2e::ContractCreateTransaction]";
  ContractId contractId;
  EXPECT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setBytecodeFileId(fileId)
                      .setAdminKey(getTestClient().getOperatorPublicKey())
                      .setGas(200000ULL)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                      .setMemo(memo)
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .mContractId.value());

  // Prepare byte vectors for passing to RLP serialization
  std::vector<std::byte> type = internal::HexConverter::hexToBytes("02");
  std::vector<std::byte> chainId = internal::HexConverter::hexToBytes("012a");
  std::vector<std::byte> nonce = internal::HexConverter::hexToBytes("00");
  std::vector<std::byte> maxPriorityGas = internal::HexConverter::hexToBytes("00");
  std::vector<std::byte> maxGas = internal::HexConverter::hexToBytes("d1385c7bf0");
  std::vector<std::byte> gasLimit = internal::HexConverter::hexToBytes("0249f0");
  std::vector<std::byte> to = internal::HexConverter::hexToBytes(contractId.toSolidityAddress());
  std::vector<std::byte> value = internal::HexConverter::hexToBytes("00");
  std::vector<std::byte> callData = ContractFunctionParameters().addString("new message").toBytes("setMessage");
  std::vector<std::byte> accessList = {};

  // Serialize bytes to RLP format for signing
  RLPItem list(RLPItem::RLPType::LIST_TYPE);
  list.pushBack(chainId);
  list.pushBack(RLPItem());
  list.pushBack(maxPriorityGas);
  list.pushBack(maxGas);
  list.pushBack(gasLimit);
  list.pushBack(to);
  list.pushBack(RLPItem());
  list.pushBack(callData);
  RLPItem accessListItem(accessList);
  accessListItem.setType(RLPItem::RLPType::LIST_TYPE);
  list.pushBack(accessListItem);

  // signed bytes in r,s form
  std::vector<std::byte> signedBytes =
    testPrivateKey->sign(internal::Utilities::concatenateVectors({ type, list.write() }));

  std::vector<std::byte> r(signedBytes.begin(),
                           signedBytes.begin() + std::min(signedBytes.size(), static_cast<size_t>(32)));

  std::vector<std::byte> s(signedBytes.end() - std::min(signedBytes.size(), static_cast<size_t>(32)),
                           signedBytes.end());

  std::vector<std::byte> recoveryId = internal::HexConverter::hexToBytes("01");

  // recId, r, s should be added to original RLP list as Ethereum Transactions require
  list.pushBack(recoveryId);
  list.pushBack(r);
  list.pushBack(s);

  std::vector<std::byte> ethereumTransactionData = list.write();
  // Type should be concatenated to RLP as this is a service side requirement
  ethereumTransactionData = internal::Utilities::concatenateVectors({ type, ethereumTransactionData });

  // When Then
  EthereumTransaction ethereumTransaction;
  EXPECT_NO_THROW(ethereumTransaction = EthereumTransaction().setEthereumData(ethereumTransactionData));

  TransactionResponse txResponse;
  EXPECT_NO_THROW(txResponse = ethereumTransaction.execute(getTestClient()));

  EXPECT_TRUE(txResponse.getRecord(getTestClient()).mContractFunctionResult.has_value());
  //  mSignerNonce should be incremented to 1 after the first contract execution
  EXPECT_EQ(txResponse.getRecord(getTestClient()).mContractFunctionResult.value().mSignerNonce, 1);

  // Clean up
  EXPECT_NO_THROW(
    const TransactionReceipt txReceipt =
      ContractDeleteTransaction().setContractId(contractId).execute(getTestClient()).getReceipt(getTestClient()));

  EXPECT_NO_THROW(const TransactionReceipt txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}