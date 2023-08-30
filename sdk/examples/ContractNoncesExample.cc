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
#include "Client.h"
#include "ContractCreateTransaction.h"
#include "ContractDeleteTransaction.h"
#include "ContractFunctionResult.h"
#include "ED25519PrivateKey.h"
#include "FileAppendTransaction.h"
#include "FileCreateTransaction.h"
#include "FileDeleteTransaction.h"
#include "FileId.h"
#include "FileInfo.h"
#include "FileInfoQuery.h"
#include "Status.h"
#include "TransactionReceipt.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "impl/Utilities.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace Hedera;

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    std::cout << "Please input account ID and private key" << std::endl;
    return 1;
  }

  // Get a client for the Hedera testnet, and set the operator account ID and key such that all generated transactions
  // will be paid for by this account and be signed by this key.
  Client client = Client::forTestnet();
  const AccountId operatorId = AccountId::fromString(argv[1]);
  std::unique_ptr<PrivateKey> operatorKey = ED25519PrivateKey::fromString(argv[2]);
  const std::shared_ptr<PublicKey> operatorPublicKey = operatorKey->getPublicKey();
  client.setOperator(operatorId, operatorKey.get());

  // Contract's bytecode as hex
  const std::string mBytecodeHexWithContractNonceInfo =
    "6080604052348015600f57600080fd5b50604051601a90603b565b604051809103906000f0801580156035573d6000803e3d6000fd5"
    "b50506047565b605c8061009483390190565b603f806100556000396000f3fe6080604052600080fdfea2646970667358221220a201"
    "22cbad3457fedcc0600363d6e895f17048f5caa4afdab9e655123737567d64736f6c634300081200336080604052348015600f57600"
    "080fd5b50603f80601d6000396000f3fe6080604052600080fdfea264697066735822122053dfd8835e3dc6fedfb8b4806460b9b716"
    "3f8a7248bac510c6d6808d9da9d6d364736f6c63430008120033";

  // Create the contract's bytecode file
  TransactionReceipt txReceipt =
    FileCreateTransaction()
      .setKeys({ operatorPublicKey.get() })
      .setContents(internal::Utilities::stringToByteVector(mBytecodeHexWithContractNonceInfo))
      .setMaxTransactionFee(Hbar(2LL))
      .execute(client)
      .getReceipt(client);
  std::cout << "FileCreateTransaction execution completed with status: " << gStatusToString.at(txReceipt.mStatus)
            << std::endl;
  if (!txReceipt.mFileId.has_value())
  {
    std::cout << "No file created!" << std::endl;
    return 1;
  }

  const FileId newFileId = txReceipt.mFileId.value();
  std::cout << "Contract bytecode file created with ID " << newFileId.toString() << std::endl;

  // Create the actual contract
  TransactionResponse contractCreateTxResponse = ContractCreateTransaction()
                                                   .setAdminKey(operatorPublicKey.get())
                                                   .setGas(100000ULL)
                                                   .setBytecodeFileId(newFileId)
                                                   .setMemo("[e2e::ContractADeploysContractBInConstructor]")
                                                   .execute(client);

  TransactionReceipt contractCreateTxReceipt = contractCreateTxResponse.getReceipt(client);
  std::cout << "ContractCreateTransaction execution completed with status: "
            << gStatusToString.at(contractCreateTxReceipt.mStatus) << std::endl;
  if (!contractCreateTxReceipt.mContractId.has_value())
  {
    std::cout << "No contract created!" << std::endl;
    return 1;
  }

  const ContractId contractId = contractCreateTxReceipt.mContractId.value();
  std::cout << "Smart contract created with ID " << contractId.toString() << std::endl;

  // Get contract's function results
  ContractFunctionResult contractFunctionResult =
    contractCreateTxResponse.getRecord(client).mContractFunctionResult.value();

  std::cout << "Contract Nonces: " << std::endl;
  for (auto it = contractFunctionResult.mContractNonces.begin(); it != contractFunctionResult.mContractNonces.end();
       ++it)
  {
    std::cout << "ContractId: " << (*it).mContractId.toString() << std::endl;
    std::cout << "Nonce: " << (*it).mNonce << std::endl << std::endl;
  }

  std::cout << std::endl << std::endl;

  // Now delete the contract
  txReceipt = ContractDeleteTransaction()
                .setContractId(contractId)
                .setTransferAccountId(operatorId)
                .setMaxTransactionFee(Hbar(1LL))
                .execute(client)
                .getReceipt(client);
  std::cout << "ContractDeleteTransaction execution completed with status: " << gStatusToString.at(txReceipt.mStatus)
            << std::endl;

  return 0;
}
