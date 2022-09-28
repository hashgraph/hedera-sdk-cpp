/*-
 *
 * Hedera Java SDK
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
#include "FileAppendTransaction.h"

#include "Hbar.h"

namespace Hedera
{
//-----
FileAppendTransaction::FileAppendTransaction()
  : ChunkedTransaction()
  , mFileId()
  , mContents()
{
  mDefaultMaxTransactionFee = Hbar(5LL);
  setChunkSize(2048);
}

//-----
FileAppendTransaction::FileAppendTransaction(
  const std::unordered_map<
    TransactionId,
    std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
  : ChunkedTransaction(transactions)
{
  initFromTransactionBody();
}

//-----
FileAppendTransaction::FileAppendTransaction(
  const proto::TransactionBody& transaction)
  : ChunkedTransaction(transaction)
{
  initFromTransactionBody();
}

//-----
void
FileAppendTransaction::validateChecksums(const Client& client) const
{
  if (mFileId.isValid())
  {
    mFileId.getValue().validateChecksum(client);
  }
}

//-----
proto::FileAppendTransactionBody
FileAppendTransaction::build() const
{
  proto::FileAppendTransactionBody body;

  if (mFileId.isValid())
  {
    body.set_allocated_fileid(mFileId.getValue().toProtobuf());
  }

  body.set_contents(mContents);

  return body;
}

//-----
FileAppendTransaction&
FileAppendTransaction::setFileId(const FileId& fileId)
{
  requireNotFrozen();

  mFileId.setValue(fileId);
  return *this;
}

//-----
FileAppendTransaction&
FileAppendTransaction::setContents(const std::string& contents)
{
  requireNotFrozen();

  mContents = contents;
  return *this;
}

//-----
void
FileAppendTransaction::initFromTransactionBody()
{
}

} // namespace Hedera
