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
#include "ContractFunctionResult.h"
#include "ED25519PrivateKey.h"
#include "FileCreateTransaction.h"
#include "FileId.h"
#include "Hbar.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "impl/HexConverter.h"

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
  client.setOperator(operatorId, operatorKey);

  // Get the contract's bytecode
  const std::vector<std::byte> byteCode = internal::HexConverter::hexToBytes(
    json::parse(std::ifstream(std::filesystem::current_path() / "hello_world.json", std::ios::in))["object"]
      .get<std::string>());

  // Create the contract's bytecode file
  TransactionReceipt txReceipt = FileCreateTransaction()
                                   .setKey(operatorPublicKey)
                                   .setContents(byteCode)
                                   .setMaxTransactionFee(Hbar(2LL))
                                   .execute(client)
                                   .getReceipt(client);
  std::cout << "FileCreateTransaction execution completed with status: " << gStatusToString.at(txReceipt.getStatus())
            << std::endl;
  if (!txReceipt.getFileId().has_value())
  {
    std::cout << "No file created!" << std::endl;
    return 1;
  }

  const FileId fileId = txReceipt.getFileId().value();
  std::cout << "Contract bytecode file created with ID " << fileId.toString() << std::endl;

  // Create the actual contract
  txReceipt = ContractCreateTransaction()
                .setGas(500000ULL)
                .setBytecodeFileId(fileId)
                .setAdminKey(operatorPublicKey)
                .setMaxTransactionFee(Hbar(16LL))
                .execute(client)
                .getReceipt(client);
  std::cout << "ContractCreateTransaction execution completed with status: "
            << gStatusToString.at(txReceipt.getStatus()) << std::endl;
  if (!txReceipt.getContractId().has_value())
  {
    std::cout << "No contract created!" << std::endl;
    return 1;
  }

  const ContractId contractId = txReceipt.getContractId().value();
  std::cout << "Smart contract created with ID " << contractId.toString() << std::endl;

  // Call a smart contract function
  ContractFunctionResult contractFunctionResult =
    ContractCallQuery().setGas(500000ULL).setContractId(contractId).setFunction("greet").execute(client);

  if (!contractFunctionResult.mErrorMessage.empty())
  {
    std::cout << "Error calling contract: " << contractFunctionResult.mErrorMessage << std::endl;
    return 1;
  }

  std::cout << "Contract message: " << contractFunctionResult.getString(0) << std::endl;

  // Now delete the contract
  txReceipt = ContractDeleteTransaction()
                .setContractId(contractId)
                .setTransferAccountId(operatorId)
                .setMaxTransactionFee(Hbar(1LL))
                .execute(client)
                .getReceipt(client);
  std::cout << "ContractDeleteTransaction execution completed with status: "
            << gStatusToString.at(txReceipt.getStatus()) << std::endl;

  return 0;
}
