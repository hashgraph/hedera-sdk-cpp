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
#include "impl/BaseNode.h"
#include "impl/BaseNodeAddress.h"
#include "impl/HederaCertificateVerifier.h"
#include "impl/MirrorNode.h"
#include "impl/Node.h"

#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <thread>
#include <utility>

namespace Hedera::internal
{
//-----
template<typename NodeType, typename KeyType>
void BaseNode<NodeType, KeyType>::close()
{
  std::unique_lock lock(*mMutex);
  closeChannel();
}

//-----
template<typename NodeType, typename KeyType>
void BaseNode<NodeType, KeyType>::increaseBackoff()
{
  std::unique_lock lock(*mMutex);
  ++mBadGrpcStatusCount;
  mReadmitTime =
    std::chrono::system_clock::now() + std::chrono::duration_cast<std::chrono::system_clock::duration>(mCurrentBackoff);
  mCurrentBackoff *= 2.0;

  // Make sure the current backoff doesn't go over the max backoff.
  if (mCurrentBackoff > mMaxNodeBackoff)
  {
    mCurrentBackoff = mMaxNodeBackoff;
  }
}

//-----
template<typename NodeType, typename KeyType>
void BaseNode<NodeType, KeyType>::decreaseBackoff()
{
  std::unique_lock lock(*mMutex);
  mCurrentBackoff /= 2.0;

  // Make sure the current backoff doesn't go below the min backoff.
  if (mCurrentBackoff < mMinNodeBackoff)
  {
    mCurrentBackoff = mMinNodeBackoff;
  }
}

//-----
template<typename NodeType, typename KeyType>
bool BaseNode<NodeType, KeyType>::isHealthy() const
{
  std::unique_lock lock(*mMutex);
  return mReadmitTime < std::chrono::system_clock::now();
}

//-----
template<typename NodeType, typename KeyType>
bool BaseNode<NodeType, KeyType>::channelFailedToConnect()
{
  std::unique_lock lock(*mMutex);
  if (mIsConnected)
  {
    return false;
  }

  const std::chrono::system_clock::time_point timeoutTime = std::chrono::system_clock::now() + GET_STATE_TIMEOUT;
  mIsConnected = getChannel()->WaitForConnected(timeoutTime);

  return !mIsConnected;
}

//-----
template<typename NodeType, typename KeyType>
std::chrono::duration<double> BaseNode<NodeType, KeyType>::getRemainingTimeForBackoff() const
{
  std::unique_lock lock(*mMutex);
  return mReadmitTime - std::chrono::system_clock::now();
}

//-----
template<typename NodeType, typename KeyType>
NodeType& BaseNode<NodeType, KeyType>::setMinNodeBackoff(const std::chrono::duration<double>& backoff)
{
  std::unique_lock lock(*mMutex);
  if (mCurrentBackoff == mMinNodeBackoff)
  {
    mCurrentBackoff = backoff;
  }

  mMinNodeBackoff = backoff;
  return static_cast<NodeType&>(*this);
}

//-----
template<typename NodeType, typename KeyType>
NodeType& BaseNode<NodeType, KeyType>::setMaxNodeBackoff(const std::chrono::duration<double>& backoff)
{
  std::unique_lock lock(*mMutex);
  mMaxNodeBackoff = backoff;
  return static_cast<NodeType&>(*this);
}

//-----
template<typename NodeType, typename KeyType>
BaseNode<NodeType, KeyType>::BaseNode(BaseNodeAddress address)
  : mAddress(std::move(address))
{
}

//-----
template<typename NodeType, typename KeyType>
NodeType& BaseNode<NodeType, KeyType>::setAddress(const BaseNodeAddress& address)
{
  // Close the connection since the address is changing.
  closeChannel();

  mAddress = address;
  return static_cast<NodeType&>(*this);
}

//-----
template<typename NodeType, typename KeyType>
std::shared_ptr<grpc::Channel> BaseNode<NodeType, KeyType>::getChannel()
{
  if (!mChannel)
  {
    grpc::ChannelArguments channelArguments;
    channelArguments.SetInt(GRPC_ARG_ENABLE_RETRIES, 0);
    channelArguments.SetInt(GRPC_ARG_KEEPALIVE_TIMEOUT_MS, 10000);
    channelArguments.SetInt(GRPC_ARG_KEEPALIVE_PERMIT_WITHOUT_CALLS, 1);

    if (const std::string authority = getAuthority(); !authority.empty())
    {
      channelArguments.SetString(GRPC_ARG_DEFAULT_AUTHORITY, authority);
    }

    mChannel = grpc::CreateCustomChannel(mAddress.toString(),
                                         mAddress.isTransportSecurity() ? getTlsChannelCredentials()
                                                                        : grpc::InsecureChannelCredentials(),
                                         channelArguments);
    initializeStubs();
  }

  return mChannel;
}

//-----
template<typename NodeType, typename KeyType>
std::shared_ptr<grpc::ChannelCredentials> BaseNode<NodeType, KeyType>::getTlsChannelCredentials() const
{
  return grpc::experimental::TlsCredentials(grpc::experimental::TlsChannelCredentialsOptions());
}

//-----
template<typename NodeType, typename KeyType>
void BaseNode<NodeType, KeyType>::closeChannel()
{
  closeStubs();

  // The connection is closed automatically upon destruction of the channel.
  mChannel = nullptr;
}

/**
 * Explicit template instantiations.
 */
template class BaseNode<Node, AccountId>;
template class BaseNode<MirrorNode, BaseNodeAddress>;

} // namespace Hedera::internal
