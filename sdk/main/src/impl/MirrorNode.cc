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
#include "impl/MirrorNode.h"
#include "exceptions/UninitializedException.h"
#include "impl/HederaCertificateVerifier.h"

#include <algorithm>
#include <grpcpp/create_channel.h>

namespace Hedera::internal
{
//-----
MirrorNode::MirrorNode(std::string_view address)
  : mAddress(address)
{
}

//-----
bool MirrorNode::connect(const std::chrono::system_clock::time_point& timeout)
{
  return mIsInitialized || initializeChannel(timeout);
}

//-----
void MirrorNode::shutdown()
{
  mConsensusStub = nullptr;
  mChannel = nullptr;

  mIsInitialized = false;
}

//-----
void MirrorNode::setMinBackoff(const std::chrono::duration<double>& backoff)
{
  mMinBackoff = backoff;
}

//-----
void MirrorNode::setMaxBackoff(const std::chrono::duration<double>& backoff)
{
  mMaxBackoff = backoff;
}

//-----
bool MirrorNode::isHealthy() const
{
  return mReadmitTime < std::chrono::system_clock::now();
}

//-----
void MirrorNode::increaseBackoff()
{
  mReadmitTime =
    std::chrono::system_clock::now() + std::chrono::duration_cast<std::chrono::system_clock::duration>(mCurrentBackoff);
  mCurrentBackoff = std::min(mCurrentBackoff * 2, mMaxBackoff);
}

//-----
void MirrorNode::decreaseBackoff()
{
  mCurrentBackoff /= 2.0;
  mCurrentBackoff = std::max(mCurrentBackoff / 2.0, mMinBackoff);
}

//-----
std::chrono::duration<double> MirrorNode::getRemainingTimeForBackoff() const
{
  return mReadmitTime - std::chrono::system_clock::now();
}

//-----
bool MirrorNode::initializeChannel(const std::chrono::system_clock::time_point& deadline)
{
  shutdown();

  grpc::ChannelArguments channelArguments;
  channelArguments.SetInt(GRPC_ARG_ENABLE_RETRIES, 0);
  channelArguments.SetInt(GRPC_ARG_KEEPALIVE_TIMEOUT_MS, 10000);
  channelArguments.SetInt(GRPC_ARG_KEEPALIVE_PERMIT_WITHOUT_CALLS, 1);

  mChannel = grpc::CreateCustomChannel(mAddress, grpc::InsecureChannelCredentials(), channelArguments);

  if (mChannel->WaitForConnected(deadline))
  {
    mConsensusStub = com::hedera::mirror::api::proto::ConsensusService::NewStub(mChannel);
    mIsInitialized = true;
  }

  return mIsInitialized;
}

} // namespace Hedera::internal
