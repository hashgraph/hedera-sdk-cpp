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
#include "ContractInfoQuery.h"
#include "Client.h"
#include "ContractInfo.h"
#include "Status.h"
#include "TransferTransaction.h"
#include "impl/Node.h"

#include <proto/contract_get_info.pb.h>
#include <proto/query.pb.h>
#include <proto/query_header.pb.h>
#include <proto/response.pb.h>

namespace Hedera
{
//-----
ContractInfoQuery& ContractInfoQuery::setContractId(const ContractId& contractId)
{
  mContractId = contractId;
  return *this;
}

//-----
proto::Query ContractInfoQuery::makeRequest(const Client& client, const std::shared_ptr<internal::Node>& node) const
{
  proto::Query query;
  proto::ContractGetInfoQuery* getContractInfoQuery = query.mutable_contractgetinfo();

  proto::QueryHeader* header = getContractInfoQuery->mutable_header();
  header->set_responsetype(proto::ANSWER_ONLY);

  TransferTransaction tx = TransferTransaction()
                             .setTransactionId(TransactionId::generate(*client.getOperatorAccountId()))
                             .setNodeAccountIds({ node->getAccountId() })
                             .setMaxTransactionFee(Hbar(1LL))
                             .addHbarTransfer(*client.getOperatorAccountId(), Hbar(-1LL))
                             .addHbarTransfer(node->getAccountId(), Hbar(1LL));
  tx.onSelectNode(node);
  header->set_allocated_payment(new proto::Transaction(tx.makeRequest(client, node)));

  getContractInfoQuery->set_allocated_contractid(mContractId.toProtobuf().release());

  return query;
}

//-----
ContractInfo ContractInfoQuery::mapResponse(const proto::Response& response) const
{
  return ContractInfo::fromProtobuf(response.contractgetinfo().contractinfo());
}

//-----
Status ContractInfoQuery::mapResponseStatus(const proto::Response& response) const
{
  return gProtobufResponseCodeToStatus.at(response.contractgetinfo().header().nodetransactionprecheckcode());
}

//-----
grpc::Status ContractInfoQuery::submitRequest(const Client& client,
                                              const std::chrono::system_clock::time_point& deadline,
                                              const std::shared_ptr<internal::Node>& node,
                                              proto::Response* response) const
{
  return node->submitQuery(proto::Query::QueryCase::kContractGetInfo, makeRequest(client, node), deadline, response);
}

} // namespace Hedera
