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
#include "AccountId.h"
#include "BaseIntegrationTest.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractFunctionParameters.h"
#include "ContractId.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "PrivateKey.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "exceptions/PrecheckStatusException.h"
#include "exceptions/ReceiptStatusException.h"
#include "impl/Utilities.h"

#include <chrono>
#include <gtest/gtest.h>
#include <string_view>

using namespace Hedera;

class ContractDeleteTransactionIntegrationTests : public BaseIntegrationTest
{
  // protected:
  //   [[nodiscard]] inline std::string_view getBytecode() const { return bytecode; }

  // private:
  //   std::string_view bytecode =
  //     "608060405234801561001057600080fd5b5060405161011e38038061011e8339818101604052602081101561003357600080fd5b5051600080"
  //     "61010c6000396000f3fe6080604052348015600f57600080fd5b506004361060325760003560e01c8063d09de08a146037578063f891409614"
  //     "605b575b600080fd5b603d606d565b6040518082815260200191505060405180910390f35b605b60048036036020811015605757600080fd5b"
  //     "81019080803590602001909291905050506071565b005b60008054604080516020600284861615610100026000190190921691909104601f81"
  //     "01849004840282018401909252828152606093909290918301828280156101845780601f106101595761010080835404028352916020019161"
  //     "0184565b820191906000526020600020905b81548152906001019060200180831161016757829003601f168201915b5050505050905090565b"
  //     "828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f106101b757805160ff1916838001"
  //     "1785556101e5565b828001600101855582156101e5579182015b828111156101e55782518255916020019190600101906101ca565b5b509050"
  //     "6101f491906101f8565b5090565b61021891905b808211156102145760008160009055506001016101fc565b5090565b90565b6114f6806102"
  //     "2e6000396000f3fe608060405234801561001057600080fd5b50600436106100515760003560e01c80632e64cec714610056578063925242fa"
  //     "1461008a575b600080fd5b61005e6100c6565b6040518082815260200191505060405180910390f35b6100c46100cf565b6040518082815260"
  //     "200191505060405180910390f35b60005481565b600080546001019055565b60008181548110151561010157fe5b9060005260206000200154"
  //     "60ff16600a0a830260026020908101929092556002820193506101dd565b828054600181600116156101000203166002900490600052602060"
  //     "002090601f016020900481019282601f106101b757805160ff19168380011785556101e5565b828001600101855582156101e5579182015b82"
  //     "8111156101e55782518255916020019190600101906101ca565b5b5090506101f491906101f8565b5090565b60206040519081016040528060"
  //     "0081525090565b61026d91905b80821115610269576000816000905550600101610251565b5090565b905600a165627a7a723058206b034f5b"
  //     "f6e4b8fc3037a6d276b0de4f1d5752cc2fdd23e7a8e626b9f2e217c30029";
};

//-----
TEST_F(ContractDeleteTransactionIntegrationTests, DeleteContractWithAdminKey)
{
  // Given
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

  ContractId contractId;
  ASSERT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setBytecodeFileId(fileId)
                      .setAdminKey(operatorKey->getPublicKey())
                      .setGas(1000000ULL)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .mContractId.value());

  // When / Then
  EXPECT_NO_THROW(const TransactionReceipt txReceipt = ContractDeleteTransaction()
                                                         .setContractId(contractId)
                                                         .setTransferAccountId(AccountId(2ULL))
                                                         .execute(getTestClient())
                                                         .getReceipt(getTestClient()));
}

//-----
TEST_F(ContractDeleteTransactionIntegrationTests, CannotDeleteContractWithNoAdminKey)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");

  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ operatorKey->getPublicKey() })
                             .setContents(internal::Utilities::stringToByteVector(getTestSmartContractBytecode()))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .mFileId.value());

  ContractId contractId;
  ASSERT_NO_THROW(contractId =
                    ContractCreateTransaction()
                      .setBytecodeFileId(fileId)
                      .setGas(1000000ULL)
                      .setConstructorParameters(ContractFunctionParameters().addString("Hello from Hedera.").toBytes())
                      .execute(getTestClient())
                      .getReceipt(getTestClient())
                      .mContractId.value());

  // When / Then
  EXPECT_THROW(const TransactionReceipt txReceipt = ContractDeleteTransaction()
                                                      .setContractId(contractId)
                                                      .setTransferAccountId(AccountId(2ULL))
                                                      .execute(getTestClient())
                                                      .getReceipt(getTestClient()),
               ReceiptStatusException); // MODIFYING_IMMUTABLE_CONTRACT
}

//-----
TEST_F(ContractDeleteTransactionIntegrationTests, CannotDeleteContractWithNoContractId)
{
  // Given / When / Then
  EXPECT_THROW(const TransactionResponse txResponse = ContractDeleteTransaction().execute(getTestClient()),
               PrecheckStatusException); // INVALID_CONTRACT_ID
}
