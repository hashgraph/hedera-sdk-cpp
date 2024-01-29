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
#include "JsonTypeMapper.h"
#include "SdkClient.h"
#include "TckServer.h"
#include "impl/EntityIdHelper.h"

using namespace Hedera::TCK;

int main(int argc, char** argv)
{
  // Initialize the TCK server with the port number.
  TckServer tckServer(
    (argc > 1) ? static_cast<int>(Hedera::internal::EntityIdHelper::getNum(argv[1])) : // NOLINT

      
                        TckServer::DEFAULT_HTTP_PORT);

  // Add the SDK client functions.
  tckServer.add("createAccount",
                getHandle(&SdkClient::createAccount),
                { "publicKey",
                  "initialBalance",
                  "receiverSignatureRequired",
                  "maxAutomaticTokenAssociations",
                  "stakedAccountId",
                  "stakedNodeId",
                  "declineStakingReward",
                  "accountMemo" });
  tckServer.add("generatePrivateKey", getHandle(&SdkClient::generatePrivateKey));
  tckServer.add("generatePublicKey", getHandle(&SdkClient::generatePublicKey), { "privateKey" });
  tckServer.add("setup",
                getHandle(&SdkClient::setup),
                { "operatorAccountId", "operatorPrivateKey", "nodeIp", "nodeAccountId", "mirrorNetworkIp" });
  tckServer.add("reset", getHandle(&SdkClient::reset));

  // Start listening for requests.
  tckServer.startServer();

  return 0;
}
