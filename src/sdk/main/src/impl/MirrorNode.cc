/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "impl/BaseNodeAddress.h"

namespace Hiero::internal
{
//-----
MirrorNode::MirrorNode(const BaseNodeAddress& address)
  : BaseNode<MirrorNode, BaseNodeAddress>(address)
{
}

//-----
MirrorNode::MirrorNode(std::string_view address)
  : BaseNode<MirrorNode, BaseNodeAddress>(BaseNodeAddress::fromString(address))
{
}

//-----
void MirrorNode::initializeStubs()
{
  if (!mConsensusStub)
  {
    mConsensusStub = com::hedera::mirror::api::proto::ConsensusService::NewStub(getChannel());
  }

  if (!mNetworkStub)
  {
    mNetworkStub = com::hedera::mirror::api::proto::NetworkService::NewStub(getChannel());
  }
}

//-----
void MirrorNode::closeStubs()
{
  mConsensusStub = nullptr;
  mNetworkStub = nullptr;
}

} // namespace Hiero::internal
