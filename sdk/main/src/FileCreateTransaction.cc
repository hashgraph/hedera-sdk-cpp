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
#include "FileCreateTransaction.h"

#include "helper/InstantConverter.h"

namespace Hedera
{
//-----
FileCreateTransaction::FileCreateTransaction()
  : Transaction()
  , mExpirationTime()
  , mKeys()
  , mContents()
  , mMemo()
{
}

//-----
FileCreateTransaction::FileCreateTransaction(
  const std::unordered_map<
    TransactionId,
    std::unordered_map<AccountId, proto::TransactionBody>>& transactions)
{
  initFromTransactionBody();
}

//-----
FileCreateTransaction::FileCreateTransaction(
  const proto::TransactionBody& transaction)
{
  initFromTransactionBody();
}

//-----
void
FileCreateTransaction::validateChecksums(const Client& client) const
{
  return;
}

//-----
proto::FileCreateTransactionBody
FileCreateTransaction::build() const
{
  proto::FileCreateTransactionBody body;

  if (mExpirationTime.isValid())
  {
    body.set_allocated_expirationtime(
      InstantConverter::toProtobuf(mExpirationTime.getValue()));
  }

  if (mKeys.isValid())
  {
    body.set_allocated_keys(mKeys.getValue().toProtobuf());
  }

  body.set_contents(mContents);
  body.set_memo(mMemo);

  return body;
}

//-----
FileCreateTransaction&
FileCreateTransaction::setExpirationTime(
  const std::chrono::nanoseconds& expirationTime)
{
  requireNotFrozen();

  mExpirationTime.setValue(expirationTime);
  return *this;
}

//-----
FileCreateTransaction&
FileCreateTransaction::setKeys(const KeyList& keys)
{
  requireNotFrozen();

  mKeys.setValue(keys);
  return *this;
}

//-----
FileCreateTransaction&
FileCreateTransaction::setContents(const std::string& contents)
{
  requireNotFrozen();

  mContents = contents;
  return *this;
}

//-----
FileCreateTransaction&
FileCreateTransaction::setFileMemo(const std::string& memo)
{
  requireNotFrozen();

  mMemo = memo;
  return *this;
}

//-----
void
FileCreateTransaction::initFromTransactionBody()
{
  if (mSourceTransactionBody.has_filecreate())
  {
    const proto::FileCreateTransactionBody& body =
      mSourceTransactionBody.filecreate();

    if (body.has_expirationtime())
    {
      mExpirationTime.setValue(
        InstantConverter::fromProtobuf(body.expirationtime()));
    }

    if (body.has_keys())
    {
      mKeys.setValue(KeyList::fromProtobuf(body.keys()));
    }

    mContents = body.contents();
    mMemo = body.memo();
  }
}

} // namespace Hedera
