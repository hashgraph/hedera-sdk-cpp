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
#ifndef HEDERA_SDK_CPP_IMPL_MIRROR_NODE_H_
#define HEDERA_SDK_CPP_IMPL_MIRROR_NODE_H_

#include <proto/mirror/consensus_service.grpc.pb.h>

#include "BaseNode.h"

#include <memory>
#include <string_view>

namespace Hedera::internal
{
class BaseNodeAddress;
}

namespace Hedera::internal
{
/**
 * Internal utility class used to represent a mirror node on a Hedera network.
 */
class MirrorNode : public BaseNode<MirrorNode, BaseNodeAddress>
{
public:
  /**
   * Construct with the address of the MirrorNode.
   *
   * @param address The address of the MirrorNode.
   */
  explicit MirrorNode(const BaseNodeAddress& address);

  /**
   * Construct with the address of the MirrorNode as a string.
   *
   * @param address The address of the MirrorNode as a string.
   */
  explicit MirrorNode(std::string_view address);

  /**
   * Derived from BaseNode. Get this MirrorNode's key, which is its BaseNodeAddress.
   *
   * @return This MirrorNode's key, which is its BaseNodeAddress.
   */
  [[nodiscard]] inline BaseNodeAddress getKey() const override { return getAddress(); }

  /**
   * Get a pointer to the ConsensusService stub used by this MirrorNode.
   *
   * @return A pointer ot the ConsensusService stub used by this MirrorNode.
   */
  [[nodiscard]] std::shared_ptr<com::hedera::mirror::api::proto::ConsensusService::Stub> getConsensusServiceStub() const
  {
    return mConsensusStub;
  }

private:
  /**
   * Derived from BaseNode. Get the authority of this MirrorNode.
   *
   * @return There is no MirrorNode authority, so returns an empty string;
   */
  [[nodiscard]] inline std::string getAuthority() const override { return {}; }

  /**
   * Derived from BaseNode. Initialize the stubs in this MirrorNode with a gRPC channel.
   *
   * @param channel The gRPC channel with which to initialize the stubs.
   */
  void initializeStubs(const std::shared_ptr<grpc::Channel>& channel) override;

  /**
   * Derived from BaseNode. Close the stubs in this MirrorNode.
   */
  void closeStubs() override;

  /**
   * Pointer to the gRPC stub used to communicate with the consensus service living on the remote mirror node.
   */
  std::shared_ptr<com::hedera::mirror::api::proto::ConsensusService::Stub> mConsensusStub = nullptr;
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_MIRROR_NODE_H_
