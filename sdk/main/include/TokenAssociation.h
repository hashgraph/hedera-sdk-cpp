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
#ifndef HEDERA_SDK_CPP_TOKEN_ASSOCIATION_H_
#define HEDERA_SDK_CPP_TOKEN_ASSOCIATION_H_

#include "AccountId.h"
#include "TokenId.h"

namespace proto
{
class TokenAssociation;
}

namespace Hedera
{
/**
 * A helper class that represents an account's association with a particular token.
 */
class TokenAssociation
{
public:
  /**
   * Create a TokenAssociation object from a TokenAssociation protobuf object.
   *
   * @param proto The TokenAssociation protobuf object from which to create a TokenAssociation object.
   * @return The constructed TokenAssociation object.
   */
  [[nodiscard]] static TokenAssociation fromProtobuf(const proto::TokenAssociation& proto);

  /**
   * Create a TokenAssociation object from an array of bytes.
   *
   * @param bytes The bytes from which to create a TokenAssociation object.
   * @return The constructed TokenAssociation object.
   */
  [[nodiscard]] static TokenAssociation fromBytes(const std::vector<std::byte>& bytes);

  /**
   * Construct a TokenAssociation protobuf object from this TokenAssociation object.
   *
   * @return A pointer to the created TokenAssociation protobuf object filled with this TokenAssociation object's
   *         data.
   */
  [[nodiscard]] std::unique_ptr<proto::TokenAssociation> toProtobuf() const;

  /**
   * Create a byte array representation of this TokenAssociation object.
   *
   * @return A byte array representation of this TokenAssociation object.
   */
  [[nodiscard]] std::vector<std::byte> toBytes() const;

  /**
   * The ID of the account associated with the token.
   */
  AccountId mAccountId;

  /**
   * The ID of the token associated with the account.
   */
  TokenId mTokenId;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_TOKEN_ASSOCIATION_H_
