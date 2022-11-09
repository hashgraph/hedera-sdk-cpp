/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#ifndef NODE_ADDRESS_H_
#define NODE_ADDRESS_H_

#include <string>
#include <vector>

#include "Endpoint.h"
#include "AccountId.h"

namespace proto
{
class NodeAddress;
}

namespace Hedera
{
class NodeAddress
{
public:
  inline static int PORT_MIRROR_PLAIN = 5600;
  inline static int PORT_MIRROR_TLS = 443;
  inline static int PORT_NODE_PLAIN = 50211;
  inline static int PORT_NODE_TLS = 50212;

  NodeAddress();

  [[nodiscard]] std::shared_ptr<AccountId> getAccountId() const;

  [[nodiscard]] const std::vector<Endpoint>& getEndpoints() const;

  static NodeAddress fromProtobuf(const proto::NodeAddress& protoNodeAddress);

  [[nodiscard]] std::string toString() const;

private:
  std::vector<Endpoint> mEndpoints;

  std::string mRSAPublicKey;

  int64_t mNodeId;

  std::string mCertificateHash;

  std::string mDescription;

  std::shared_ptr<AccountId> mAccountId;
};
} // namespace Hedera

#endif // NODE_ADDRESS_H_