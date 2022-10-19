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
#include "Transaction.h"

#include "AccountCreateTransaction.h"

namespace Hedera
{
//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setValidTransactionDuration(const std::chrono::duration<double>& duration)
{
  mTransactionValidDuration = duration;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setMaxTransactionFee(const Hedera::Hbar& fee)
{
  mMaxTransactionFee = fee;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setTransactionMemo(const std::string& memo)
{
  mTransactionMemo = memo;
  return static_cast<SdkRequestType&>(*this);
}

//-----
template<typename SdkRequestType>
SdkRequestType& Transaction<SdkRequestType>::setTransactionId(const TransactionId& id)
{
  mTransactionId = id;
  return static_cast<SdkRequestType&>(*this);
}

/**
 * Explicit template instantiations.
 */
template class Transaction<AccountCreateTransaction>;

} // namespace Hedera
