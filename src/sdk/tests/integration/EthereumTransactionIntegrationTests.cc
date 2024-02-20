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
#include "AccountBalance.h"
#include "AccountBalanceQuery.h"
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ContractCreateTransaction.h"
#include "ContractFunctionParameters.h"
#include "ContractId.h"
#include "ECDSAsecp256k1PrivateKey.h"
#include "ECDSAsecp256k1PublicKey.h"
#include "ED25519PrivateKey.h"
#include "EthereumTransaction.h"
#include "EthereumTransactionDataEip1559.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileId.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "exceptions/OpenSSLException.h"
#include "impl/HexConverter.h"
#include "impl/Network.h"
#include "impl/RLPItem.h"
#include "impl/Utilities.h"
#include "impl/openssl_utils/OpenSSLUtils.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <thread>
#include <vector>

using json = nlohmann::json;
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
  const EvmAddress testEvmAddress = testPublicKey->toEvmAddress();
  const Hbar testInitialBalance(1000LL, HbarUnit::HBAR());
  const std::chrono::system_clock::duration testAutoRenewPeriod = std::chrono::seconds(2592000);
  const std::string testMemo = "test account memo";
  const uint32_t testMaxAutomaticTokenAssociations = 4U;

  auto senderAccount = AccountCreateTransaction()
                         .setKey(testPublicKey)
                         .setInitialBalance(testInitialBalance)
                         .setReceiverSignatureRequired(true)
                         .setAutoRenewPeriod(testAutoRenewPeriod)
                         .setAccountMemo(testMemo)
                         .setMaxAutomaticTokenAssociations(testMaxAutomaticTokenAssociations)
                         .setDeclineStakingReward(true)
                         .setAlias(testEvmAddress)
                         .freezeWith(&getTestClient())
                         .sign(testPrivateKey)
                         .execute(getTestClient());

  AccountId accountId = senderAccount.getReceipt(getTestClient()).mAccountId.value();
  std::cout << accountId.toString() << std::endl;

  Client ecdsaClient;

  ecdsaClient = Client::fromConfigFile((std::filesystem::current_path() / "local_node.json").string());
  ecdsaClient.setNetworkUpdatePeriod(std::chrono::hours(24));

  std::vector<std::byte> mFileContent;

  mFileContent = internal::Utilities::stringToByteVector(
    json::parse(std::ifstream(std::filesystem::current_path() / "hello_world.json", std::ios::in))["object"]
      .get<std::string>());

  ecdsaClient.setOperator(accountId, testPrivateKey);

  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  const std::string memo = "[e2e::ContractCreateTransaction]";
  const std::chrono::system_clock::duration autoRenewPeriod = std::chrono::hours(2016);

  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ operatorKey->getPublicKey() })
                             .setContents(internal::Utilities::stringToByteVector(getTestSmartContractBytecode()))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .mFileId.value());

  // When
  ContractId contractId;
  EXPECT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setBytecodeFileId(fileId)
                      .setAdminKey(operatorKey->getPublicKey())
                      .setGas(1000000ULL)
                      .setAutoRenewPeriod(autoRenewPeriod)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                      .setMemo(memo)
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .mContractId.value());

  std::vector<std::byte> contractFunctionParameters =
    ContractFunctionParameters().addString("new message").toBytes("setMessage");

  std::vector<std::byte> type = internal::HexConverter::hexToBytes("02");
  std::vector<std::byte> chainId = internal::HexConverter::hexToBytes("012a");
  std::vector<std::byte> nonce = internal::HexConverter::hexToBytes("05");
  std::vector<std::byte> maxPriorityGas = internal::HexConverter::hexToBytes("00");
  std::vector<std::byte> maxGas = internal::HexConverter::hexToBytes("d1385c7bf0");
  std::vector<std::byte> gasLimit = internal::HexConverter::hexToBytes("6050");
  std::vector<std::byte> to = internal::HexConverter::hexToBytes(contractId.toSolidityAddress());
  std::vector<std::byte> value = internal::HexConverter::hexToBytes("02540be400");
  std::vector<std::byte> callData = contractFunctionParameters;
  std::vector<std::byte> accessList = {};

  std::cout << contractId.toSolidityAddress() << std::endl;

  RLPItem list(RLPItem::RLPType::LIST_TYPE);
  list.pushBack(chainId);
  list.pushBack(nonce);
  list.pushBack(RLPItem());
  list.pushBack(maxGas);
  list.pushBack(gasLimit);
  list.pushBack(to);
  list.pushBack(value);
  list.pushBack(RLPItem());
  RLPItem accessListItem(accessList);
  accessListItem.setType(RLPItem::RLPType::LIST_TYPE);
  list.pushBack(accessListItem);

  // signed bytes in r,s form
  std::vector<std::byte> signedBytes = testPrivateKey->sign(internal::OpenSSLUtils::computeKECCAK256(list.write()));

  std::vector<std::byte> r(signedBytes.begin(),
                           signedBytes.begin() + std::min(signedBytes.size(), static_cast<size_t>(32)));

  std::vector<std::byte> s(signedBytes.end() - std::min(signedBytes.size(), static_cast<size_t>(32)),
                           signedBytes.end());

  std::vector<std::byte> recoveryId = internal::HexConverter::hexToBytes("01");

  list.pushBack(recoveryId);
  list.pushBack(r);
  list.pushBack(s);

  std::vector<std::byte> ethereumTransactionData = list.write();
  ethereumTransactionData = internal::Utilities::concatenateVectors({ type, ethereumTransactionData });

  // Prints RLP
  std::cout << std::endl;
  std::cout << "Ether Transaction output: ";
  for (const auto& byte : ethereumTransactionData)
  {
    // Cast std::byte to unsigned char to get its numerical value, then to int for proper I/O stream handling
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(byte))
              << ' ';
  }
  std::cout << std::endl;
  std::cout << std::endl;

  // When
  EthereumTransaction tx =
    EthereumTransaction().setEthereumData(ethereumTransactionData).setMaxTransactionFee(Hbar(11, HbarUnit::HBAR()));

  TransactionResponse txResponse = tx.execute(ecdsaClient);

  std::cout << txResponse.getRecord(ecdsaClient).toString() << std::endl;

  auto contractFunctionResult = txResponse.getRecord(ecdsaClient).mContractFunctionResult;
  std::cout << contractFunctionResult.has_value() << std::endl;
}