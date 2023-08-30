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
#include "BaseIntegrationTest.h"
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "impl/Utilities.h"

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

using json = nlohmann::json;
using namespace std;

namespace Hedera
{
//-----
void BaseIntegrationTest::SetUp()
{
  const string_view networkTag = "network";
  const string_view operatorTag = "operator";
  const string_view accountIdTag = "accountId";
  const string_view privateKeyTag = "privateKey";

  const string testPathToJSON = (filesystem::current_path() / "local_node.json").string();

  ifstream testInputFile(testPathToJSON, ios::in);

  unordered_map<string, string> networksMap;
  unordered_map<string, AccountId> networkAccountsMap;
  unordered_map<string, string>::iterator it;

  json jsonData = json::parse(testInputFile);
  jsonData[networkTag].get_to(networksMap);

  for (it = networksMap.begin(); it != networksMap.end(); it++)
  {
    const string_view accountIdStr = (*it).first;
    const string nodeAddressString = (*it).second;

    networkAccountsMap.try_emplace(nodeAddressString, AccountId::fromString(accountIdStr));
  }

  AccountId operatorAccountId;
  string operatorAccountPrivateKey;

  if (jsonData[operatorTag][accountIdTag].is_string() && jsonData[operatorTag][privateKeyTag].is_string())
  {
    string operatorAccountIdStr = jsonData[operatorTag][accountIdTag];
    operatorAccountPrivateKey = jsonData[operatorTag][privateKeyTag];

    operatorAccountId = AccountId::fromString(operatorAccountIdStr);
  }

  testInputFile.close();

  mClient = Client::forNetwork(networkAccountsMap);
  mClient.setOperator(operatorAccountId, ED25519PrivateKey::fromString(operatorAccountPrivateKey).get());
  mClient.setMirrorNetwork({ "127.0.0.1:5600" });

  mFileContent = internal::Utilities::stringToByteVector(
    json::parse(std::ifstream(std::filesystem::current_path() / "hello_world.json", std::ios::in))["object"]
      .get<std::string>());
}

} // namespace Hedera
