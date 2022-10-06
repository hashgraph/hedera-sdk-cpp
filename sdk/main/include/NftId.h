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
#ifndef NFT_ID_H_
#define NFT_ID_H_

/**
 * Library includes
 */
#include "TokenId.h"

namespace Hedera
{
/**
 * The (non-fungible) token of which this NFT is an instance
 */
class NftId
{
public:
  TokenId mTokenId;
  long long mSerial;
};

} // namespace Hedera

#endif // NFT_ID_H_