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
#include "AccountCreateTransaction.h"
#include "AccountId.h"
#include "Client.h"
#include "ED25519PrivateKey.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Status.h"
#include "TransactionReceipt.h"
#include "TransactionResponse.h"
#include "impl/EntityIdHelper.h"

#include <httplib.h>
#include <jsonrpccxx/server.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>

class TckServer
{
public:
  /**
   * Start the TCK server listening on a specific port.
   * @param port The port number on which to listen for HTTP requests.
   */
  explicit TckServer(uint16_t port)
    : mPort(port)
  {
    // Set up the request handles.
    mServer.Add("setup",
                jsonrpccxx::GetHandle(&TckServer::setup, *this),
                { "operatorAccountId", "operatorPrivateKey", "nodeIp", "nodeAccountId", "mirrorNetworkIp" });
    mServer.Add("createAccount",
                jsonrpccxx::GetHandle(&TckServer::createAccount, *this),
                { "publicKey",
                  "initialBalance",
                  "receiverSignatureRequired",
                  "maxAutomaticTokenAssociations",
                  "stakedAccountId",
                  "stakedNodeId",
                  "declineStakingReward",
                  "accountMemo" });
    mServer.Add("generatePrivateKey", jsonrpccxx::GetHandle(&TckServer::generatePrivateKey, *this));
    mServer.Add("generatePublicKey", jsonrpccxx::GetHandle(&TckServer::generatePublicKey, *this), { "privateKey" });

    // Start listening for requests.
    mHttpServer.Post(
      "/", [this](const httplib::Request& request, httplib::Response& response) { handleRequest(request, response); });
  }

  /**
   * Stop listening when trying to destroy.
   */
  ~TckServer() { stopListening(); }

  /**
   * Start listening for requests from the TCK.
   */
  bool startListening() { return mHttpServer.listen("localhost", mPort); }

  /**
   * Stop listening for requests from the TCK.
   */
  void stopListening()
  {
    if (mHttpServer.is_running())
    {
      mHttpServer.stop();
    }
  }

  /**
   * Set up the Client to use when executing requests.
   *
   * @param operatorAccountId  The ID of the operator account to use.
   * @param operatorPrivateKey The private key of the operator to use.
   * @param nodeIp             Optionally, the IP of the node with which the Client should communicate.
   * @param nodeAccountId      Optionally, the ID of the node account running the node with which this Client should
   *                           communicate.
   * @param mirrorNetworkIp    Optionally, the IP of the mirror node with which the Client should communicate.
   */
  nlohmann::json setup(const std::string& operatorAccountId,
                       const std::string& operatorPrivateKey,
                       const std::string& nodeIp,
                       const std::string& nodeAccountId,
                       const std::string& mirrorNetworkIp)
  {
    std::string clientType;
    std::cout << operatorAccountId << std::endl;
    std::cout << operatorPrivateKey << std::endl;
    std::cout << nodeIp << std::endl;
    std::cout << nodeAccountId << std::endl;
    std::cout << mirrorNetworkIp << std::endl;

    if (!nodeIp.empty() && !nodeAccountId.empty() && !mirrorNetworkIp.empty())
    {
      mClient = Hedera::Client::forNetwork({
        {nodeIp.data(), Hedera::AccountId::fromString("0.0." + nodeAccountId)}
      });
      clientType = "custom";
    }
    else
    {
      mClient = Hedera::Client::forTestnet();
      clientType = "testnet";
    }

    mClient.setOperator(Hedera::AccountId::fromString(operatorAccountId),
                        Hedera::PrivateKey::fromStringDer(operatorPrivateKey));
    mClient.setRequestTimeout(std::chrono::seconds(30));

    return {
      {"message", "Successfully setup " + clientType + " client."},
      { "status", "SUCCESS"                                      }
    };
  }

  nlohmann::json reset()
  {
    mClient.close();
    return { "status", "SUCCESS" };
  }

  nlohmann::json createAccount(const std::string& publicKey,
                               const std::string& initialBalance,
                               const std::string& receiverSignatureRequired,
                               const std::string& maxAutomaticTokenAssociations,
                               const std::string& stakedAccountId,
                               const std::string& stakedNodeId,
                               const std::string& declineStakingReward,
                               const std::string& accountMemo)
  {
    Hedera::AccountCreateTransaction accountCreateTransaction;
    accountCreateTransaction.setGrpcDeadline(std::chrono::seconds(30));

    accountCreateTransaction.setKey(Hedera::PublicKey::fromStringDer(publicKey));

    if (!initialBalance.empty())
    {
      accountCreateTransaction.setInitialBalance(
        Hedera::Hbar(static_cast<int64_t>(Hedera::internal::EntityIdHelper::getNum(initialBalance))));
    }
    if (!receiverSignatureRequired.empty())
    {
      if (receiverSignatureRequired != "true" && receiverSignatureRequired != "false")
      {
        throw std::invalid_argument("receiverSignatureRequired should be true or false");
      }
      accountCreateTransaction.setReceiverSignatureRequired(receiverSignatureRequired == "true");
    }
    if (!maxAutomaticTokenAssociations.empty())
    {
      accountCreateTransaction.setMaxAutomaticTokenAssociations(
        static_cast<uint32_t>(Hedera::internal::EntityIdHelper::getNum(maxAutomaticTokenAssociations)));
    }
    if (!stakedAccountId.empty())
    {
      accountCreateTransaction.setStakedAccountId(Hedera::AccountId::fromString(stakedAccountId));
    }
    if (!stakedNodeId.empty())
    {
      accountCreateTransaction.setStakedNodeId(Hedera::internal::EntityIdHelper::getNum(stakedNodeId));
    }
    if (!declineStakingReward.empty())
    {
      if (declineStakingReward != "true" && declineStakingReward != "false")
      {
        throw std::invalid_argument("declineStakingReward should be true or false");
      }
      accountCreateTransaction.setDeclineStakingReward(declineStakingReward == "true");
    }
    if (!accountMemo.empty())
    {
      accountCreateTransaction.setAccountMemo(accountMemo);
    }

    const Hedera::TransactionReceipt txReceipt = accountCreateTransaction.execute(mClient).getReceipt(mClient);
    return {
      {"accountId", txReceipt.mAccountId->toString()             },
      { "status",   Hedera::gStatusToString.at(txReceipt.mStatus)}
    };
  }

  std::string generatePrivateKey() { return Hedera::ED25519PrivateKey::generatePrivateKey()->toStringDer(); }

  std::string generatePublicKey(const std::string& privateKey)
  {
    return Hedera::ED25519PrivateKey::fromString(privateKey)->getPublicKey()->toStringDer();
  }

private:
  void handleRequest(const httplib::Request& request, httplib::Response& response)
  {
    std::cout << request.body << std::endl;
    response.status = 200;

    // Unfortunately, the json-rpc-cxx library doesn't properly handle empty "params" JSON objects, even though it's
    // valid in the JSON-RPC 2.0 spec. It must be removed manually.
    response.set_content(mServer.HandleRequest(removeEmptyParamsIfExists(request).body), "application/json");
    std::cout << response.body << std::endl;
  }

  httplib::Request removeEmptyParamsIfExists(const httplib::Request& request)
  {
    nlohmann::json json = nlohmann::json::parse(request.body);
    if (!json.contains("params") || !json["params"].empty())
    {
      return request;
    }

    json.erase("params");
    httplib::Request newRequest = request;
    newRequest.body = json.dump();
    return newRequest;
  }

  jsonrpccxx::JsonRpc2Server mServer;
  httplib::Server mHttpServer;
  int mPort;

  Hedera::Client mClient;
};

int main(int argc, char** argv)
{
  uint16_t port = 80; // Default option
  if (argc > 1)
  {
    port = static_cast<uint16_t>(Hedera::internal::EntityIdHelper::getNum(argv[1]));
  }

  // Initialize the TCK server with the port number.
  TckServer tckServer(port);

  // Start listening for requests.
  tckServer.startListening();

  return 0;
}