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
#include "FileInfoQuery.h"
#include "Client.h"
#include "FileId.h"
#include "FileInfo.h"
#include "Status.h"
#include "TransferTransaction.h"
#include "impl/Node.h"

#include <proto/file_get_info.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
FileInfoQuery& FileInfoQuery::setFileId(const FileId& fileId)
{
  mFileId = fileId;
  return *this;
}

//-----
proto::Query FileInfoQuery::makeRequest(const Client& client, const std::shared_ptr<internal::Node>& node) const
{
  proto::Query query;
  proto::FileGetInfoQuery* getFileInfoQuery = query.mutable_filegetinfo();

  proto::QueryHeader* header = getFileInfoQuery->mutable_header();
  header->set_responsetype(proto::ANSWER_ONLY);

  TransferTransaction tx = TransferTransaction()
                             .setTransactionId(TransactionId::generate(*client.getOperatorAccountId()))
                             .setNodeAccountIds({ node->getAccountId() })
                             .setMaxTransactionFee(Hbar(1LL))
                             .addHbarTransfer(*client.getOperatorAccountId(), Hbar(-1LL))
                             .addHbarTransfer(node->getAccountId(), Hbar(1LL));
  tx.onSelectNode(node);
  header->set_allocated_payment(new proto::Transaction(tx.makeRequest(client, node)));

  getFileInfoQuery->set_allocated_fileid(mFileId.toProtobuf().release());

  return query;
}

//-----
FileInfo FileInfoQuery::mapResponse(const proto::Response& response) const
{
  return FileInfo::fromProtobuf(response.filegetinfo().fileinfo());
}

//-----
Status FileInfoQuery::mapResponseStatus(const proto::Response& response) const
{
  return gProtobufResponseCodeToStatus.at(response.filegetinfo().header().nodetransactionprecheckcode());
}

//-----
grpc::Status FileInfoQuery::submitRequest(const Client& client,
                                          const std::chrono::system_clock::time_point& deadline,
                                          const std::shared_ptr<internal::Node>& node,
                                          proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kFileGetInfo, makeRequest(client, node), deadline, response);
}

} // namespace Hedera
