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
#ifndef FILE_ID_H_
#define FILE_ID_H_

namespace Hedera
{
class Client;
}

namespace proto
{
class FileID;
}

namespace Hedera
{
class FileId
{
public:
  static FileId fromProtobuf(const proto::FileID& proto) { return FileId(); }
  proto::FileID* toProtobuf() const { return new proto::FileID(); }
  void validateChecksum(const Client& client) const { return; }
};
} // namespace Hedera

#endif // FILE_ID_H_