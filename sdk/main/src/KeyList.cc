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
#include "KeyList.h"

#include <algorithm>
#include <cstddef>
#include <proto/basic_types.pb.h>

namespace Hedera
{
//-----
KeyList KeyList::fromProtobuf(const proto::KeyList& proto, int threshold)
{
  KeyList keyList;
  keyList.mThreshold = threshold;

  for (int i = 0; i < proto.keys_size(); ++i)
  {
    keyList.mKeys.emplace_back(Key::fromProtobuf(proto.keys(i)).release());
  }

  return keyList;
}

//-----
KeyList KeyList::of(const std::vector<std::shared_ptr<Key>>& keys)
{
  KeyList keyList;
  keyList.mKeys = keys;
  return keyList;
}

//-----
std::unique_ptr<Key> KeyList::clone() const
{
  return std::make_unique<KeyList>(*this);
}

//-----
std::unique_ptr<proto::Key> KeyList::toProtobufKey() const
{
  auto key = std::make_unique<proto::Key>();
  if (mThreshold > 0)
  {
    key->mutable_thresholdkey()->set_allocated_keys(toProtobuf().release());
    key->mutable_thresholdkey()->set_threshold(static_cast<uint32_t>(mThreshold));
  }
  else
  {
    key->set_allocated_keylist(toProtobuf().release());
  }

  return key;
}

//-----
std::unique_ptr<proto::KeyList> KeyList::toProtobuf() const
{
  auto keyList = std::make_unique<proto::KeyList>();

  for (const auto& key : mKeys)
  {
    *keyList->add_keys() = *key->toProtobufKey();
  }

  return keyList;
}

//-----
KeyList& KeyList::setThreshold(int threshold)
{
  mThreshold = threshold;
  return *this;
}

//-----
size_t KeyList::size() const
{
  return mKeys.size();
}

//-----
bool KeyList::empty() const
{
  return mKeys.empty();
}

//-----
bool KeyList::contains(const Key* key) const
{
  const std::vector<std::byte> keyBytes = key->toBytes();
  return std::any_of(mKeys.cbegin(),
                     mKeys.cend(),
                     [&keyBytes](const std::shared_ptr<Key>& listKey) { return listKey->toBytes() == keyBytes; });
}

//-----
void KeyList::push_back(const std::shared_ptr<Key>& key)
{
  mKeys.push_back(key);
}

//-----
void KeyList::remove(const Key* key)
{
  const std::vector<std::byte> keyBytes = key->toBytes();
  const auto eraseIter =
    std::remove_if(mKeys.begin(),
                   mKeys.end(),
                   [&keyBytes](const std::shared_ptr<Key>& listKey) { return listKey->toBytes() == keyBytes; });

  // Erasing an element pointed to by an iterator that equals end() is undefined, so this check is necessary.
  if (eraseIter != mKeys.end())
  {
    mKeys.erase(eraseIter, mKeys.end());
  }
}

//-----
void KeyList::clear()
{
  mKeys.clear();
}

} // namespace Hedera
