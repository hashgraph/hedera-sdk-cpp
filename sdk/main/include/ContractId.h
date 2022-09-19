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
#ifndef CONTRACT_ID_H_
#define CONTRACT_ID_H_

/**
 * Library includes
 */
#include "helper/InitType.h"

/**
 * STL includes
 */
#include <string>

/**
 * Protobuf forward declarations
 */
namespace proto
{
class ContractID;
} // namespace proto

/**
 * Hedera forward declarations
 */
namespace Hedera
{
template<typename T>
class InitType;

class Client;
} // namespace Hedera

namespace Hedera
{
/**
 * The ID for a smart contract instance on Hedera.
 */
class ContractId
{
public:
  ContractId() {}

  static ContractId fromProtobuf(const proto::ContractID& id)
  {
    return ContractId();
  }

  proto::ContractID* toProtobuf() const { return new proto::ContractID; }

  inline std::string toString() const { return std::string(); }

  inline void validateChecksum(const Client& client) const {}

  unsigned long long mRealm;
  unsigned long long mShard;

  InitType<unsigned long long> mContractNum;
  InitType<std::string> mEvmAddress;
};

} // namespace Hedera

#endif // CONTRACT_ID_H_