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
#include "AccountId.h"
#include "Client.h"
#include "ContractCallQuery.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractExecuteTransaction.h"
#include "ContractFunctionParameters.h"
#include "ContractFunctionResult.h"
#include "ContractId.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileId.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "exceptions/ReceiptStatusException.h"
#include "impl/HexConverter.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

using json = nlohmann::json;
using namespace Hedera;

class ContractExecuteTransactionIntegrationTest : public ::testing::Test
{
protected:
  [[nodiscard]] inline const Client& getTestClient() const { return mClient; }
  [[nodiscard]] inline const std::string& getTestSmartContractBytecode() const { return mTestContractBytecodeHex; }

  void SetUp() override
  {
    const auto accountId = AccountId::fromString("0.0.3");
    const std::string_view accountIdStr = "0.0.3";
    const std::string_view networkTag = "network";
    const std::string_view operatorTag = "operator";
    const std::string_view accountIdTag = "accountId";
    const std::string_view privateKeyTag = "privateKey";

    const std::string testPathToJSON = (std::filesystem::current_path() / "local_node.json").string();
    const std::unique_ptr<PrivateKey> testPrivateKey = ED25519PrivateKey::generatePrivateKey();
    const std::shared_ptr<PublicKey> testPublicKey = testPrivateKey->getPublicKey();

    AccountId operatorAccountId;
    std::string operatorAccountPrivateKey;
    std::ifstream testInputFile(testPathToJSON, std::ios::in);
    std::string nodeAddressString;
    json jsonData = json::parse(testInputFile);

    if (jsonData[networkTag][accountIdStr].is_string())
    {
      nodeAddressString = jsonData[networkTag][accountIdStr];
    }

    if (jsonData[operatorTag][accountIdTag].is_string() && jsonData[operatorTag][privateKeyTag].is_string())
    {
      std::string operatorAccountIdStr = jsonData[operatorTag][accountIdTag];

      operatorAccountId = AccountId::fromString(operatorAccountIdStr);
      operatorAccountPrivateKey = jsonData[operatorTag][privateKeyTag];
    }

    testInputFile.close();

    std::unordered_map<std::string, AccountId> networkMap;
    networkMap.try_emplace(nodeAddressString, accountId);

    mClient = Client::forNetwork(networkMap);
    mClient.setOperator(operatorAccountId, ED25519PrivateKey::fromString(operatorAccountPrivateKey));
  }

private:
  Client mClient;
  const std::string mTestContractBytecodeHex =
    "608060405234801561001057600080fd5b506040516104d73803806104d78339818101604052602081101561003357600080fd5b8101908080"
    "51604051939291908464010000000082111561005357600080fd5b90830190602082018581111561006857600080fd5b825164010000000081"
    "118282018810171561008257600080fd5b82525081516020918201929091019080838360005b838110156100af578181015183820152602001"
    "610097565b50505050905090810190601f1680156100dc5780820380516001836020036101000a031916815260200191505b50604052505060"
    "0080546001600160a01b0319163317905550805161010890600190602084019061010f565b50506101aa565b82805460018160011615610100"
    "0203166002900490600052602060002090601f016020900481019282601f1061015057805160ff191683800117855561017d565b8280016001"
    "018555821561017d579182015b8281111561017d578251825591602001919060010190610162565b5061018992915061018d565b5090565b61"
    "01a791905b808211156101895760008155600101610193565b90565b61031e806101b96000396000f3fe608060405234801561001057600080"
    "fd5b50600436106100415760003560e01c8063368b87721461004657806341c0e1b5146100ee578063ce6d41de146100f6575b600080fd5b61"
    "00ec6004803603602081101561005c57600080fd5b81019060208101813564010000000081111561007757600080fd5b820183602082011115"
    "61008957600080fd5b803590602001918460018302840111640100000000831117156100ab57600080fd5b91908080601f0160208091040260"
    "20016040519081016040528093929190818152602001838380828437600092019190915250929550610173945050505050565b005b6100ec61"
    "01a2565b6100fe6101ba565b6040805160208082528351818301528351919283929083019185019080838360005b8381101561013857818101"
    "5183820152602001610120565b50505050905090810190601f1680156101655780820380516001836020036101000a03191681526020019150"
    "5b509250505060405180910390f35b6000546001600160a01b0316331461018a5761019f565b805161019d906001906020840190610250565b"
    "505b50565b6000546001600160a01b03163314156101b85733ff5b565b60018054604080516020601f60026000196101008789161502019095"
    "1694909404938401819004810282018101909252828152606093909290918301828280156102455780601f1061021a57610100808354040283"
    "529160200191610245565b820191906000526020600020905b81548152906001019060200180831161022857829003601f168201915b505050"
    "505090505b90565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f106102915780"
    "5160ff19168380011785556102be565b828001600101855582156102be579182015b828111156102be57825182559160200191906001019061"
    "02a3565b506102ca9291506102ce565b5090565b61024d91905b808211156102ca57600081556001016102d456fea264697066735822122084"
    "964d4c3f6bc912a9d20e14e449721012d625aa3c8a12de41ae5519752fc89064736f6c63430006000033";
};

//-----
TEST_F(ContractExecuteTransactionIntegrationTest, ExecuteContractExecuteTransaction)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  const std::string newMessage = "new message";
  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKey(operatorKey->getPublicKey())
                             .setContents(internal::HexConverter::hexToBytes(getTestSmartContractBytecode()))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .getFileId()
                             .value());
  ContractId contractId;
  ASSERT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setBytecodeFileId(fileId)
                      .setAdminKey(operatorKey->getPublicKey())
                      .setGas(100000ULL)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .getContractId()
                      .value());

  // When
  TransactionReceipt txReceipt;
  EXPECT_NO_THROW(txReceipt = ContractExecuteTransaction()
                                .setContractId(contractId)
                                .setGas(100000ULL)
                                .setFunction("setMessage", ContractFunctionParameters().addString(newMessage))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));

  // Then
  EXPECT_EQ(ContractCallQuery()
              .setContractId(contractId)
              .setGas(100000ULL)
              .setFunction("getMessage")
              .execute(getTestClient())
              .getString(0),
            newMessage);

  // Clean up
  ASSERT_NO_THROW(txReceipt = ContractDeleteTransaction()
                                .setContractId(contractId)
                                .setTransferAccountId(AccountId(2ULL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
  ASSERT_NO_THROW(txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(ContractExecuteTransactionIntegrationTest, CannotExecuteContractWithoutContractId)
{
  // Given / When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt =
                 ContractExecuteTransaction()
                   .setGas(100000ULL)
                   .setFunction("setMessage", ContractFunctionParameters().addString("new message"))
                   .execute(getTestClient())
                   .getReceipt(getTestClient()),
               ReceiptStatusException); // INVALID_CONTRACT_ID
}

//-----
TEST_F(ContractExecuteTransactionIntegrationTest, CannotExecuteContractWithNoFunctionParameters)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKey(operatorKey->getPublicKey())
                             .setContents(internal::HexConverter::hexToBytes(getTestSmartContractBytecode()))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .getFileId()
                             .value());
  ContractId contractId;
  ASSERT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setBytecodeFileId(fileId)
                      .setAdminKey(operatorKey->getPublicKey())
                      .setGas(100000ULL)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .getContractId()
                      .value());

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = ContractExecuteTransaction()
                                                      .setContractId(contractId)
                                                      .setGas(100000ULL)
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // CONTRACT_REVERT_EXECUTED

  // Clean up
  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = ContractDeleteTransaction()
                                .setContractId(contractId)
                                .setTransferAccountId(AccountId(2ULL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
  ASSERT_NO_THROW(txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}

//-----
TEST_F(ContractExecuteTransactionIntegrationTest, CannotExecuteContractWithNoGas)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKey(operatorKey->getPublicKey())
                             .setContents(internal::HexConverter::hexToBytes(getTestSmartContractBytecode()))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .getFileId()
                             .value());
  ContractId contractId;
  ASSERT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setBytecodeFileId(fileId)
                      .setAdminKey(operatorKey->getPublicKey())
                      .setGas(100000ULL)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .getContractId()
                      .value());

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt =
                 ContractExecuteTransaction()
                   .setContractId(contractId)
                   .setFunction("setMessage", ContractFunctionParameters().addString("new message"))
                   .execute(getTestClient())
                   .getReceipt(getTestClient()),
               ReceiptStatusException); // INSUFFICIENT_GAS

  // Clean up
  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = ContractDeleteTransaction()
                                .setContractId(contractId)
                                .setTransferAccountId(AccountId(2ULL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
  ASSERT_NO_THROW(txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}
