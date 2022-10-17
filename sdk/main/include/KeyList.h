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
#ifndef KEY_LIST_H_
#define KEY_LIST_H_

#include <proto/basic_types.pb.h>

namespace Hedera
{
class KeyList
{
public:
  static KeyList fromProtobuf(const proto::KeyList& proto) { return KeyList(); }
  proto::KeyList* toProtobuf() const { return new proto::KeyList; }
};
} // namespace Hedera

#endif // KEY_LIST_H_