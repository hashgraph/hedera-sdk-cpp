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
#include "AddressBookQuery.h"
#include "Client.h"
#include "NodeAddress.h"
#include "NodeAddressBook.h"
#include "exceptions/MaxAttemptsExceededException.h"
#include "impl/MirrorNetwork.h"
#include "impl/MirrorNode.h"

#include <proto/mirror/mirror_network_service.pb.h>
#include <thread>
#include <vector>

namespace Hedera
{
//-----
NodeAddressBook AddressBookQuery::execute(const Client& client)
{
  return execute(client, client.getRequestTimeout());
}

//-----
NodeAddressBook AddressBookQuery::execute(const Client& client, const std::chrono::system_clock::duration& timeout)
{
  const std::chrono::system_clock::time_point timeoutTime = std::chrono::system_clock::now() + timeout;

  for (unsigned int attempt = 0U;; ++attempt)
  {
    if (attempt >= mMaxAttempts)
    {
      throw MaxAttemptsExceededException("Max number of attempts made (max attempts allowed: " +
                                         std::to_string(mMaxAttempts));
    }

    // Grab the MirrorNode to use to send this AddressBookQuery and make sure its connected.
    std::shared_ptr<internal::MirrorNode> node = client.getMirrorNetwork()->getNextMirrorNode();
    while (node->channelFailedToConnect())
    {
      std::cout << "Failed to connect to node " << node->getAddress().toString() << " on attempt " << attempt
                << std::endl;
      node = client.getMirrorNetwork()->getNextMirrorNode();
    }

    // Send this AddressBookQuery.
    grpc::ClientContext context;
    context.set_deadline(timeoutTime);
    auto reader = node->getNetworkServiceStub()->getNodes(&context, build());

    // Container in which to put the received NodeAddresses.
    std::vector<NodeAddress> nodeAddresses;

    // Read node addresses until there are none more to read.
    proto::NodeAddress nodeAddress;
    while (reader->Read(&nodeAddress))
    {
      nodeAddresses.push_back(NodeAddress::fromProtobuf(nodeAddress));
    }

    grpc::Status status = reader->Finish();
    if (const grpc::StatusCode errorCode = status.error_code(); errorCode == grpc::StatusCode::UNAVAILABLE ||
                                                                errorCode == grpc::StatusCode::RESOURCE_EXHAUSTED ||
                                                                errorCode == grpc::StatusCode::INTERNAL)
    {
      // Sleep and retry.
      std::this_thread::sleep_for(std::min(std::chrono::duration_cast<std::chrono::system_clock::duration>(
                                             DEFAULT_MIN_BACKOFF * pow(static_cast<double>(attempt), 2.0)),
                                           mMaxBackoff));
      continue;
    }

    return NodeAddressBook().setNodeAddresses(nodeAddresses);
  }
}

//-----
AddressBookQuery& AddressBookQuery::setFileId(const FileId& fileId)
{
  mFileId = fileId;
  return *this;
}

//-----
AddressBookQuery& AddressBookQuery::setLimit(unsigned int limit)
{
  mLimit = limit;
  return *this;
}

//-----
AddressBookQuery& AddressBookQuery::setMaxAttempts(unsigned int attempts)
{
  mMaxAttempts = attempts;
  return *this;
}

//-----
AddressBookQuery& AddressBookQuery::setMaxBackoff(const std::chrono::system_clock::duration& backoff)
{
  mMaxBackoff = backoff;
  return *this;
}

//-----
com::hedera::mirror::api::proto::AddressBookQuery AddressBookQuery::build() const
{
  com::hedera::mirror::api::proto::AddressBookQuery addressBookQuery;

  if (!(mFileId == FileId()))
  {
    addressBookQuery.set_allocated_file_id(mFileId.toProtobuf().release());
  }

  addressBookQuery.set_limit(static_cast<int32_t>(mLimit));

  return addressBookQuery;
}

} // namespace Hedera
