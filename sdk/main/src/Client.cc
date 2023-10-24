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
#include "AccountId.h"
#include "AddressBookQuery.h"
#include "Defaults.h"
#include "Hbar.h"
#include "NodeAddressBook.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "exceptions/UninitializedException.h"
#include "impl/BaseNodeAddress.h"
#include "impl/MirrorNetwork.h"
#include "impl/Network.h"
#include "impl/TLSBehavior.h"

#include <condition_variable>
#include <fstream>
#include <stdexcept>
#include <thread>

namespace Hedera
{
//-----
struct Client::ClientImpl
{
  // Pointer to the network object that contains all processing for sending/receiving information to/from a Hedera
  // network.
  std::shared_ptr<internal::Network> mNetwork = nullptr;

  // Pointer to the MirrorNetwork object that contains the mirror nodes for sending/receiving information to/from a
  // Hedera mirror node.
  std::shared_ptr<internal::MirrorNetwork> mMirrorNetwork = nullptr;

  // The ID of the account that will pay for requests using this Client.
  std::optional<AccountId> mOperatorAccountId;

  // Pointer to the private key this Client should use to sign transactions.
  std::shared_ptr<PrivateKey> mOperatorPrivateKey = nullptr;

  // The maximum fee this Client is willing to pay for transactions.
  std::optional<Hbar> mMaxTransactionFee;

  // The maximum payment this Client is willing to make for queries.
  std::optional<Hbar> mMaxQueryPayment;

  // The transaction ID regeneration policy to utilize when transactions submitted by this Client receive a
  // TRANSACTION_EXPIRED response from the network.
  std::optional<bool> mTransactionIdRegenerationPolicy;

  // The maximum length of time this Client should wait to get a response after sending a request to the network.
  std::chrono::duration<double> mRequestTimeout = std::chrono::minutes(2);

  // The maximum number of attempts a request sent by this Client will attempt to send a request. A manually-set
  // maximum number of attempts in the request will override this.
  std::optional<uint32_t> mMaxAttempts;

  // The minimum length of time a request sent to a Node by this Client will wait before attempting to send a request
  // again after a failure to the same Node. A manually-set minimum backoff in the request will override this.
  std::optional<std::chrono::duration<double>> mMinBackoff;

  // The maximum length of time a request sent to a Node by this Client will wait before attempting to send a request
  // again after a failure to the same Node. A manually-set maximum backoff in the request will override this.
  std::optional<std::chrono::duration<double>> mMaxBackoff;

  // The maximum amount of time this Client should spend trying to execute a request before giving up on that request
  // attempt. A manually-set gRPC deadline in the request will override this.
  std::optional<std::chrono::system_clock::duration> mGrpcDeadline;

  // The period of time to wait between network updates.
  std::chrono::duration<double> mNetworkUpdatePeriod = DEFAULT_NETWORK_UPDATE_PERIOD;

  // Should this Client automatically validate entity checksums?
  bool mAutoValidateChecksums = false;

  // Has this Client made its initial network update? This is utilized in case the user updates the network update
  // period before the initial update is made, as it prevents the update period from being overwritten.
  bool mMadeInitialNetworkUpdate = false;

  // Is this Client trying to cancel its network update? Used to signal to the network update thread that it should
  // shut down.
  bool mCancelUpdate = false;

  // The time at which this Client started waiting to update. This is needed in case Clients get moved.
  std::chrono::system_clock::time_point mStartNetworkUpdateWaitTime;

  // The mutex for updating this Client.
  std::mutex mMutex;

  // The condition variable for the network update.
  std::condition_variable mConditionVariable;

  // The thread that handles the network updates.
  std::unique_ptr<std::thread> mNetworkUpdateThread = nullptr;
};

//-----
Client::Client()
  : mImpl(std::make_unique<ClientImpl>())
{
  // Start the network update thread.
  startNetworkUpdateThread(DEFAULT_NETWORK_UPDATE_INITIAL_DELAY);
}

//-----
Client::~Client()
{
  close();
}

//-----
Client::Client(Client&& other) noexcept
  : mImpl(std::make_unique<ClientImpl>())
{
  moveClient(std::move(other));

  // Leave moved-from object in a valid state
  other.mImpl = std::make_unique<ClientImpl>(); // NOLINT
}

//-----
Client& Client::operator=(Client&& other) noexcept
{
  if (this != &other)
  {
    moveClient(std::move(other));

    // Leave moved-from object in a valid state
    other.mImpl = std::make_unique<ClientImpl>(); // NOLINT
  }

  return *this;
}

//-----
Client Client::forNetwork(const std::unordered_map<std::string, AccountId>& networkMap)
{
  Client client;
  client.mImpl->mNetwork = std::make_shared<internal::Network>(internal::Network::forNetwork(networkMap));
  return client;
}

//-----
Client Client::forName(std::string_view name)
{
  // clang-format off
  if      (name == "mainnet")    return Client::forMainnet();
  else if (name == "testnet")    return Client::forTestnet();
  else if (name == "previewnet") return Client::forPreviewnet();
  else throw std::invalid_argument("Unknown Client name");
  // clang-format on
}

//-----
Client Client::forMainnet()
{
  Client client;
  client.mImpl->mNetwork = std::make_shared<internal::Network>(internal::Network::forMainnet());
  client.mImpl->mMirrorNetwork = std::make_shared<internal::MirrorNetwork>(internal::MirrorNetwork::forMainnet());
  return client;
}

//-----
Client Client::forTestnet()
{
  Client client;
  client.mImpl->mNetwork = std::make_shared<internal::Network>(internal::Network::forTestnet());
  client.mImpl->mMirrorNetwork = std::make_shared<internal::MirrorNetwork>(internal::MirrorNetwork::forTestnet());
  return client;
}

//-----
Client Client::forPreviewnet()
{
  Client client;
  client.mImpl->mNetwork = std::make_shared<internal::Network>(internal::Network::forPreviewnet());
  client.mImpl->mMirrorNetwork = std::make_shared<internal::MirrorNetwork>(internal::MirrorNetwork::forPreviewnet());
  return client;
}

//-----
Client Client::fromConfig(std::string_view json)
{
  // Make sure the input string is valid JSON.
  nlohmann::json jsonObj;
  try
  {
    jsonObj = nlohmann::json::parse(json);
  }
  catch (const std::exception& ex)
  {
    throw std::invalid_argument(std::string("Cannot parse JSON: ") + ex.what());
  }

  return fromConfig(jsonObj);
}

//-----
Client Client::fromConfig(const nlohmann::json& json)
{
  Client client;

  // A "network" tag should always be specified.
  if (!json.contains("network"))
  {
    throw std::invalid_argument("Network tag is not set in JSON");
  }

  // If the network tags specifies a dictionary of nodes, parse each one.
  if (const nlohmann::json& jsonNetwork = json["network"]; jsonNetwork.is_object())
  {
    std::unordered_map<std::string, AccountId> networkMap;
    for (const auto& [accountId, url] : jsonNetwork.items())
    {
      networkMap.try_emplace(url, AccountId::fromString(accountId));
    }

    client = Client::forNetwork(networkMap);

    // If a network name is provided, set the ledger ID based on it.
    if (json.contains("networkName"))
    {
      try
      {
        client.setLedgerId(LedgerId::fromString(jsonNetwork["networkName"].get<std::string_view>()));
      }
      catch (const std::exception&)
      {
        throw std::invalid_argument(
          R"(Invalid argument for network name. Should be one of "mainnet", "testnet", or "previewnet")");
      }
    }
  }

  // If the network tag specifies a name, get the Client for that name.
  else if (jsonNetwork.is_string())
  {
    try
    {
      client = Client::forName(jsonNetwork.get<std::string_view>());
    }
    catch (const std::exception&)
    {
      throw std::invalid_argument("Invalid argument for network tag ");
    }
  }

  // If the network tag type is unclear, throw.
  else
  {
    throw std::invalid_argument("Invalid argument for network tag");
  }

  // Check if there's an operator configured.
  if (json.contains("operator"))
  {
    const nlohmann::json& jsonOperator = json["operator"];

    if (!jsonOperator.is_object())
    {
      throw std::invalid_argument("Invalid argument for operator");
    }

    // If an operator is configured, an AccountId and PrivateKey must also be configured.
    if (!jsonOperator.contains("accountId") || !jsonOperator.contains("privateKey"))
    {
      throw std::invalid_argument("An operator must have an accountId and privateKey");
    }

    // Verify the account ID is valid.
    AccountId operatorAccountId;
    try
    {
      operatorAccountId = AccountId::fromString(jsonOperator["accountId"].get<std::string_view>());
    }
    catch (const std::exception&)
    {
      throw std::invalid_argument("Invalid argument for operator accountId");
    }

    // Verify the private key is valid.
    std::shared_ptr<PrivateKey> operatorPrivateKey;
    try
    {
      operatorPrivateKey = PrivateKey::fromStringDer(jsonOperator["privateKey"].get<std::string_view>());
    }
    catch (const std::exception&)
    {
      throw std::invalid_argument("Invalid argument for operator privateKey");
    }

    // Set the operator.
    client.setOperator(operatorAccountId, operatorPrivateKey);
  }

  // Check if there's a mirror network configured.
  if (json.contains("mirrorNetwork"))
  {
    // If the mirror network tags specifies a list of nodes, parse each one.
    if (const nlohmann::json& jsonMirrorNetwork = json["mirrorNetwork"]; jsonMirrorNetwork.is_array())
    {
      std::vector<std::string> mirrorNetwork;
      for (const auto& url : jsonMirrorNetwork)
      {
        mirrorNetwork.push_back(url);
      }

      client.setMirrorNetwork(mirrorNetwork);
    }

    // If the mirror network tag specifies a name, get the mirror network for that name.
    else if (jsonMirrorNetwork.is_string())
    {
      try
      {
        const std::string_view mirrorNetworkName = jsonMirrorNetwork.get<std::string_view>();
        if (mirrorNetworkName == "mainnet")
        {
          client.mImpl->mMirrorNetwork =
            std::make_shared<internal::MirrorNetwork>(internal::MirrorNetwork::forMainnet());
        }
        else if (mirrorNetworkName == "testnet")
        {
          client.mImpl->mMirrorNetwork =
            std::make_shared<internal::MirrorNetwork>(internal::MirrorNetwork::forTestnet());
        }
        else if (mirrorNetworkName == "previewnet")
        {
          client.mImpl->mMirrorNetwork =
            std::make_shared<internal::MirrorNetwork>(internal::MirrorNetwork::forPreviewnet());
        }
        else
        {
          throw std::invalid_argument("Invalid argument for mirrorNetwork tag");
        }
      }
      catch (const std::exception&)
      {
        throw std::invalid_argument("Invalid argument for mirrorNetwork tag");
      }
    }

    // If the mirrorNetwork tag type is unclear, throw.
    else
    {
      throw std::invalid_argument("Invalid argument for mirrorNetwork tag");
    }
  }

  return client;
}

//-----
Client Client::fromConfigFile(std::string_view path)
{
  std::ifstream infile(path.data());
  if (!infile.is_open())
  {
    throw std::invalid_argument(std::string("File cannot be found at ") + path.data());
  }

  // Make sure the input file is valid JSON.
  nlohmann::json jsonObj;
  try
  {
    jsonObj = nlohmann::json::parse(infile);
  }
  catch (const std::exception& ex)
  {
    throw std::invalid_argument(std::string("Cannot parse JSON: ") + ex.what());
  }

  return fromConfig(jsonObj);
}

//-----
Client& Client::setMirrorNetwork(const std::vector<std::string>& network)
{
  mImpl->mMirrorNetwork = std::make_shared<internal::MirrorNetwork>(internal::MirrorNetwork::forNetwork(network));
  return *this;
}

//-----
Client& Client::setOperator(const AccountId& accountId, const std::shared_ptr<PrivateKey>& privateKey)
{
  mImpl->mOperatorAccountId = accountId;
  mImpl->mOperatorPrivateKey = privateKey;

  return *this;
}

void Client::close()
{
  cancelScheduledNetworkUpdate();

  if (mImpl->mNetwork)
  {
    mImpl->mNetwork->close();
  }

  if (mImpl->mMirrorNetwork)
  {
    mImpl->mMirrorNetwork->close();
  }
}

//-----
Client& Client::setNetwork(const std::unordered_map<std::string, AccountId>& networkMap)
{
  mImpl->mNetwork = std::make_shared<internal::Network>(internal::Network::forNetwork(networkMap));
  mImpl->mMirrorNetwork = nullptr;
  return *this;
}

//-----
Client& Client::setRequestTimeout(const std::chrono::duration<double>& timeout)
{
  mImpl->mRequestTimeout = timeout;
  return *this;
}

//-----
Client& Client::setMaxTransactionFee(const Hbar& fee)
{
  if (fee.toTinybars() < 0)
  {
    throw std::invalid_argument("Transaction fee cannot be negative");
  }

  mImpl->mMaxTransactionFee = fee;
  return *this;
}

//-----
Client& Client::setMaxQueryPayment(const Hbar& payment)
{
  if (payment.toTinybars() < 0)
  {
    throw std::invalid_argument("Query payment cannot be negative");
  }

  mImpl->mMaxQueryPayment = payment;
  return *this;
}

//-----
Client& Client::setTransactionIdRegenerationPolicy(bool regenerate)
{
  mImpl->mTransactionIdRegenerationPolicy = regenerate;
  return *this;
}

//-----
Client& Client::setMaxAttempts(uint32_t attempts)
{
  mImpl->mMaxAttempts = attempts;
  return *this;
}

//-----
Client& Client::setMinBackoff(const std::chrono::duration<double>& backoff)
{
  if ((mImpl->mMaxBackoff && backoff > *mImpl->mMaxBackoff) || (!mImpl->mMaxBackoff && backoff > DEFAULT_MAX_BACKOFF) ||
      (!mImpl->mMaxBackoff && backoff < std::chrono::milliseconds(0)))
  {
    throw std::invalid_argument("Minimum backoff would be larger than maximum backoff");
  }

  mImpl->mMinBackoff = backoff;
  return *this;
}

//-----
Client& Client::setMaxBackoff(const std::chrono::duration<double>& backoff)
{
  if ((mImpl->mMinBackoff && backoff < *mImpl->mMinBackoff) || (!mImpl->mMinBackoff && backoff < DEFAULT_MIN_BACKOFF) ||
      (!mImpl->mMinBackoff && backoff > DEFAULT_MAX_BACKOFF))
  {
    throw std::invalid_argument("Maximum backoff would be smaller than minimum backoff");
  }

  mImpl->mMaxBackoff = backoff;
  return *this;
}

//-----
Client& Client::setGrpcDeadline(const std::chrono::system_clock::duration& deadline)
{
  mImpl->mGrpcDeadline = deadline;
  return *this;
}

//-----
Client& Client::setNetworkUpdatePeriod(const std::chrono::duration<double>& update)
{
  // Cancel any previous network updates and wait for the thread to complete.
  cancelScheduledNetworkUpdate();

  // Update the network update period.
  mImpl->mNetworkUpdatePeriod = update;

  // If this was called before the initial network update was made, the initial update should be skipped.
  mImpl->mMadeInitialNetworkUpdate = true;

  // Start the thread with the new network update period.
  startNetworkUpdateThread(mImpl->mNetworkUpdatePeriod);
  return *this;
}

//-----
Client& Client::setAutoValidateChecksums(bool validate)
{
  mImpl->mAutoValidateChecksums = validate;
  return *this;
}

//-----
Client& Client::setLedgerId(const LedgerId& ledgerId)
{
  if (mImpl->mNetwork)
  {
    mImpl->mNetwork->setLedgerId(ledgerId);
  }

  return *this;
}

//-----
std::shared_ptr<internal::Network> Client::getNetwork() const
{
  return mImpl->mNetwork;
}

//-----
std::optional<AccountId> Client::getOperatorAccountId() const
{
  return mImpl->mOperatorAccountId;
}

//-----
std::shared_ptr<PublicKey> Client::getOperatorPublicKey() const
{
  return mImpl->mOperatorPrivateKey ? mImpl->mOperatorPrivateKey->getPublicKey() : nullptr;
}

//-----
std::function<std::vector<std::byte>(const std::vector<std::byte>&)> Client::getOperatorSigner() const
{
  return mImpl->mOperatorPrivateKey ? [this](const std::vector<std::byte>& vec)
  { return this->mImpl->mOperatorPrivateKey->sign(vec); }
                                    : std::function<std::vector<std::byte>(const std::vector<std::byte>&)>();
}

//-----
std::chrono::duration<double> Client::getRequestTimeout() const
{
  return mImpl->mRequestTimeout;
}

//-----
std::optional<Hbar> Client::getMaxTransactionFee() const
{
  return mImpl->mMaxTransactionFee;
}

//-----
std::optional<Hbar> Client::getMaxQueryPayment() const
{
  return mImpl->mMaxQueryPayment;
}

//-----
std::optional<bool> Client::getTransactionIdRegenerationPolicy() const
{
  return mImpl->mTransactionIdRegenerationPolicy;
}

//-----
std::optional<uint32_t> Client::getMaxAttempts() const
{
  return mImpl->mMaxAttempts;
}

//-----
std::optional<std::chrono::duration<double>> Client::getMinBackoff() const
{
  return mImpl->mMinBackoff;
}

//-----
std::optional<std::chrono::duration<double>> Client::getMaxBackoff() const
{
  return mImpl->mMaxBackoff;
}

//-----
std::optional<std::chrono::system_clock::duration> Client::getGrpcDeadline() const
{
  return mImpl->mGrpcDeadline;
}

//-----
std::chrono::duration<double> Client::getNetworkUpdatePeriod() const
{
  std::unique_lock lock(mImpl->mMutex);
  return mImpl->mNetworkUpdatePeriod;
}

//-----
bool Client::isAutoValidateChecksumsEnabled() const
{
  return mImpl->mAutoValidateChecksums;
}

//-----
LedgerId Client::getLedgerId() const
{
  if (!mImpl->mNetwork)
  {
    throw UninitializedException("Client does not have a Network from which to grab the ledger ID");
  }

  return mImpl->mNetwork->getLedgerId();
}

//-----
std::shared_ptr<internal::MirrorNetwork> Client::getMirrorNetwork() const
{
  return mImpl->mMirrorNetwork;
}

//-----
void Client::startNetworkUpdateThread(const std::chrono::duration<double>& period)
{
  std::unique_lock lock(mImpl->mMutex);
  mImpl->mStartNetworkUpdateWaitTime = std::chrono::system_clock::now();
  mImpl->mNetworkUpdatePeriod = period;
  mImpl->mNetworkUpdateThread = std::make_unique<std::thread>(&Client::scheduleNetworkUpdate, this);
}

//-----
void Client::scheduleNetworkUpdate()
{
  // Network updates should keep occurring until they're cancelled.
  while (true)
  {
    if (std::unique_lock lock(mImpl->mMutex); !mImpl->mConditionVariable.wait_for(
          lock, mImpl->mNetworkUpdatePeriod, [this]() { return mImpl->mCancelUpdate; }))
    {
      // Get the address book and set the network based on the address book.
      mImpl->mNetwork->setNetwork(internal::Network::getNetworkFromAddressBook(
        AddressBookQuery().setFileId(FileId::ADDRESS_BOOK).execute(*this),
        mImpl->mNetwork->isTransportSecurity() == internal::TLSBehavior::REQUIRE
          ? internal::BaseNodeAddress::PORT_NODE_TLS
          : internal::BaseNodeAddress::PORT_NODE_PLAIN));

      // Adjust the network update period if this is the initial update.
      if (!mImpl->mMadeInitialNetworkUpdate)
      {
        mImpl->mNetworkUpdatePeriod = DEFAULT_NETWORK_UPDATE_PERIOD;
        mImpl->mMadeInitialNetworkUpdate = true;
      }

      // Schedule the next network update.
      mImpl->mStartNetworkUpdateWaitTime = std::chrono::system_clock::now();
    }

    // The network update was cancelled, stop looping.
    else
    {
      break;
    }
  }
}

//-----
void Client::cancelScheduledNetworkUpdate()
{
  // Only cancel if there's a network update scheduled.
  if (!mImpl->mNetworkUpdateThread)
  {
    return;
  }

  // Signal the thread to stop and wait for it to stop.
  mImpl->mCancelUpdate = true;
  mImpl->mConditionVariable.notify_all();
  if (mImpl->mNetworkUpdateThread->joinable())
  {
    mImpl->mNetworkUpdateThread->join();
    mImpl->mNetworkUpdateThread = nullptr;
  }

  // The update thread is closed at this point, reset mCancelUpdate.
  mImpl->mCancelUpdate = false;
}

//-----
void Client::moveClient(Client&& other)
{
  // Cancel this Client's network update if one exists.
  cancelScheduledNetworkUpdate();

  // If there's a network update thread running in the moved-from Client, it can't be simply moved. Since it still holds
  // a reference to the moved-from Client, the thread must be stopped and restarted in this Client with the remaining
  // time so that the Client reference can be updated to this Client and no longer be pointing to a moved-from Client.
  if (other.mImpl->mNetworkUpdateThread)
  {
    // Cancel the update.
    other.cancelScheduledNetworkUpdate();

    // Move the implementation to this Client.
    mImpl = std::move(other.mImpl);

    // Start the network update thread with the remaining time.
    startNetworkUpdateThread(
      mImpl->mStartNetworkUpdateWaitTime +
      std::chrono::duration_cast<std::chrono::system_clock::duration>(mImpl->mNetworkUpdatePeriod) -
      std::chrono::system_clock::now());
  }
  else
  {
    // If there was no update thread running, simply move the implementation.
    mImpl = std::move(other.mImpl);
  }
}

} // namespace Hedera
