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
#include "SdkClient.h"
#include "AccountCreateTransaction.h"
#include "AccountDeleteTransaction.h"
#include "AccountId.h"
#include "AccountInfo.h"
#include "AccountInfoQuery.h"
#include "AccountUpdateTransaction.h"
#include "Client.h"
#include "EvmAddress.h"
#include "HbarUnit.h"
#include "KeyHelper.h"
#include "PrivateKey.h"
#include "Status.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "impl/HexConverter.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <proto/basic_types.pb.h>
#include <stdexcept>
#include <string>

namespace Hedera::TCK::SdkClient
{
namespace
{
// The Hedera C++ SDK Client the SdkClient will use to communicate with the network.
Client mClient;
}

//-----
nlohmann::json generateKey(const KeyRequest& params)
{
  nlohmann::json response;
  response["key"] = processKeyRequest(params, response);
  return response;
}

//-----
nlohmann::json reset(const ResetParams&)
{
  mClient.close();
  return {
    {"status", "SUCCESS"}
  };
}

//-----
nlohmann::json setup(const SetupParams& params)
{
  std::string clientType;

  if (params.nodeIp.has_value() && params.nodeAccountId.has_value() && params.mirrorNetworkIp.has_value())
  {
    mClient = Client::forNetwork({
      {params.nodeIp.value(), Hedera::AccountId::fromString(params.nodeAccountId.value())}
    });
    mClient.setMirrorNetwork({ params.mirrorNetworkIp.value() });
    clientType = "custom";
  }
  else
  {
    mClient = Client::forTestnet();
    clientType = "testnet";
  }

  mClient.setOperator(AccountId::fromString(params.operatorAccountId),
                      PrivateKey::fromStringDer(params.operatorPrivateKey));
  mClient.setRequestTimeout(DEFAULT_TCK_REQUEST_TIMEOUT);

  return {
    {"message", "Successfully setup " + clientType + " client."},
    { "status", "SUCCESS"                                      }
  };
}

//-----
const Client& getClient()
{
  return mClient;
}

} // namespace Hedera::TCK::SdkClient
