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
#include "ScheduleSignTransaction.h"
#include "impl/Node.h"

#include <grpcpp/client_context.h>
#include <proto/schedule_sign.pb.h>
#include <proto/transaction.pb.h>
#include <stdexcept>

namespace Hedera
{
//-----
ScheduleSignTransaction::ScheduleSignTransaction(const proto::TransactionBody& transactionBody)
  : Transaction<ScheduleSignTransaction>(transactionBody)
{
  if (!transactionBody.has_schedulesign())
  {
    throw std::invalid_argument("Transaction body doesn't contain ScheduleSign data");
  }

  const proto::ScheduleSignTransactionBody& body = transactionBody.schedulesign();

  if (body.has_scheduleid())
  {
    mScheduleId = ScheduleId::fromProtobuf(body.scheduleid());
  }
}

//-----
ScheduleSignTransaction& ScheduleSignTransaction::setScheduleId(const ScheduleId& ScheduleId)
{
  requireNotFrozen();
  mScheduleId = ScheduleId;
  return *this;
}

//-----
ScheduleSignTransaction& ScheduleSignTransaction::clearScheduleId()
{
  requireNotFrozen();
  mScheduleId.reset();
  return *this;
}

//-----
proto::Transaction ScheduleSignTransaction::makeRequest(const Client& client,
                                                        const std::shared_ptr<internal::Node>&) const
{
  return signTransaction(generateTransactionBody(&client), client);
}

//-----
grpc::Status ScheduleSignTransaction::submitRequest(const Client& client,
                                                    const std::chrono::system_clock::time_point& deadline,
                                                    const std::shared_ptr<internal::Node>& node,
                                                    proto::TransactionResponse* response) const
{
  return node->submitTransaction(
    proto::TransactionBody::DataCase::kScheduleSign, makeRequest(client, node), deadline, response);
}

//-----
void ScheduleSignTransaction::addToBody(proto::TransactionBody& body) const
{
  body.set_allocated_schedulesign(build());
}

//-----
proto::ScheduleSignTransactionBody* ScheduleSignTransaction::build() const
{
  auto body = std::make_unique<proto::ScheduleSignTransactionBody>();

  if (mScheduleId.has_value())
  {
    body->set_allocated_scheduleid(mScheduleId->toProtobuf().release());
  }

  return body.release();
}

} // namespace Hedera