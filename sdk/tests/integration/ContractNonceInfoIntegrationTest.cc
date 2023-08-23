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
#include "BaseIntegrationTest.h"
#include "Client.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractFunctionParameters.h"
#include "ContractFunctionResult.h"
#include "ContractId.h"
#include "ContractInfo.h"
#include "ContractInfoQuery.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileId.h"
#include "PrivateKey.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "exceptions/ReceiptStatusException.h"
#include "impl/Utilities.h"

#include <chrono>
#include <gtest/gtest.h>
#include <iostream>

using namespace Hedera;
using namespace std;

class ContractNonceInfoIntegrationTest : public BaseIntegrationTest
{
};

//-----
TEST_F(ContractNonceInfoIntegrationTest, ContractADeploysContractBInConstructor)
{
  // Given
  const std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(
    "302e020100300506032b65700422042091132178e72057a1d7528025956fe39b0b847f200ab59b2fdd367017f3087137");
  const std::string memo = "[e2e::ContractADeploysContractBInConstructor]";
  const std::chrono::duration<double> autoRenewPeriod = std::chrono::hours(2016);
  FileId fileId;
  ASSERT_NO_THROW(fileId = FileCreateTransaction()
                             .setKeys({ operatorKey->getPublicKey().get() })
                             .setContents(internal::Utilities::stringToByteVector(getTestSmartContractBytecode()))
                             .execute(getTestClient())
                             .getReceipt(getTestClient())
                             .mFileId.value());

  TransactionResponse response;
  ASSERT_NO_THROW(response = ContractCreateTransaction()
                               .setBytecodeFileId(fileId)
                               .setAdminKey(operatorKey->getPublicKey().get())
                               .setGas(100000ULL)
                               .setMemo(memo)
                               .execute(getTestClient()));

  ContractFunctionResult contractFunctionResult;
  ASSERT_NO_THROW(contractFunctionResult = response.getRecord(getTestClient()).mContractFunctionResult.value());

  ContractId contractA = contractFunctionResult.mContractId;
  std::vector<ContractNonceInfo> contractNonces;

  cout << "Step 1 - FINISHED!" << endl << endl;

  for (auto it = contractFunctionResult.mContractNonces.begin(); it != contractFunctionResult.mContractNonces.end();
       ++it)
  {
    if (!((*it).mContractId == contractA))
    {
      contractNonces.push_back(*it);
    }
  }

  cout << "Step 2 - FINISHED!" << endl << endl;
  cout << "contractNonces.size(): " << contractNonces.size() << endl << endl;

  ContractId contractB = (*contractNonces.begin()).mContractId;

  ContractNonceInfo contractANonceInfo;
  ContractNonceInfo contractBNonceInfo;

  cout << "Step 3 - FINISHED!" << endl << endl;

  for (auto it = contractFunctionResult.mContractNonces.begin(); it != contractFunctionResult.mContractNonces.end();
       ++it)
  {
    if ((*it).mContractId == contractA)
    {
      contractANonceInfo = *it;
    }
    else if ((*it).mContractId == contractB)
    {
      contractBNonceInfo = *it;
    }
  }

  cout << "Step 4 - FINISHED!" << endl << endl;

  // When
  EXPECT_EQ(contractANonceInfo.mNonce, 2); // A.nonce = 2
  EXPECT_EQ(contractBNonceInfo.mNonce, 1); // B.nonce = 1

  // Clean up
  ContractId contractId;
  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(contractId = response.getReceipt(getTestClient()).mContractId.value());
  ASSERT_NO_THROW(txReceipt = ContractDeleteTransaction()
                                .setContractId(contractId)
                                .setTransferAccountId(AccountId(2ULL))
                                .execute(getTestClient())
                                .getReceipt(getTestClient()));
  ASSERT_NO_THROW(txReceipt =
                    FileDeleteTransaction().setFileId(fileId).execute(getTestClient()).getReceipt(getTestClient()));
}
