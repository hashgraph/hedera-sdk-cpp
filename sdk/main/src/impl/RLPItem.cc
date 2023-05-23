/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2022 Hedera Hashgraph, LLC
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
#include "impl/RLPItem.h"
#include "impl/Utilities.h"

namespace Hedera
{
namespace
{
//-----
std::vector<std::byte> encodeBinary(size_t num)
{
  std::vector<std::byte> bytes;
  while (num != 0)
  {
    bytes.insert(bytes.begin(), std::byte(num & 0xFF));
    num >>= 8;
  }

  return bytes;
}

//-----
std::vector<std::byte> encodeLength(size_t num, std::byte offset)
{
  if (num < 56)
  {
    return { std::byte(static_cast<unsigned char>(offset) + num) };
  }
  else
  {
    const std::vector<std::byte> encodedLength = encodeBinary(num);
    return internal::Utilities::concatenateVectors(
      { { std::byte(encodedLength.size() + static_cast<unsigned char>(offset) + 55) }, encodedLength });
  }
}

//-----
size_t toInteger(const std::vector<std::byte>& bytes)
{
  if (bytes.size() == 1)
  {
    return static_cast<size_t>(bytes.at(0));
  }
  else
  {
    size_t size = 0;
    for (const std::byte& byte : bytes)
    {
      size <<= 8;
      size |= static_cast<unsigned char>(byte);
    }

    return size;
  }
}

//-----
std::tuple<long, long, RLPItem::RLPType> decodeLength(const std::vector<std::byte>& bytes)
{
  if (bytes.empty())
  {
    return { 0, 0, RLPItem::RLPType::VALUE_TYPE };
  }

  const std::byte prefix = bytes.at(0);

  if (prefix < std::byte(0x7F))
  {
    return { 0, 1, RLPItem::RLPType::VALUE_TYPE };
  }
  else if (prefix <= std::byte(0xB7))
  {
    return { 1, static_cast<unsigned char>(prefix) - 0x80, RLPItem::RLPType::VALUE_TYPE };
  }
  else if (prefix <= std::byte(0xBF))
  {
    const long lenOfStrLen = static_cast<unsigned char>(prefix) - 0xB7;
    return { 1 + lenOfStrLen,
             toInteger({ bytes.cbegin() + 1, bytes.cbegin() + 1 + lenOfStrLen }),
             RLPItem::RLPType::VALUE_TYPE };
  }
  else if (prefix <= std::byte(0xF7))
  {
    return { 1, static_cast<unsigned char>(prefix) - 0xC0, RLPItem::RLPType::LIST_TYPE };
  }
  else
  {
    const long lenOfListLen = static_cast<unsigned char>(prefix) - 0xF7;
    return { 1 + lenOfListLen,
             toInteger({ bytes.cbegin() + 1, bytes.cbegin() + 1 + lenOfListLen }),
             RLPItem::RLPType::LIST_TYPE };
  }
}

} // namespace

//-----
RLPItem::RLPItem(RLPItem::RLPType type)
  : mType(type)
{
}

//-----
RLPItem::RLPItem(std::vector<std::byte> value)
  : mValue(std::move(value))
{
}

//-----
RLPItem::RLPItem(std::string_view value)
  : mValue(internal::Utilities::stringToByteVector(value))
{
}

//-----
void RLPItem::clear()
{
  mValue.clear();
  mValues.clear();
}

//-----
void RLPItem::assign(const std::vector<std::byte>& value)
{
  clear();
  mType = RLPType::VALUE_TYPE;
  mValue = value;
}

//-----
void RLPItem::assign(std::string_view value)
{
  clear();
  mType = RLPType::VALUE_TYPE;
  mValue = internal::Utilities::stringToByteVector(value);
}

//-----
void RLPItem::setType(RLPItem::RLPType type)
{
  clear();
  mType = type;
}

//-----
void RLPItem::pushBack(const RLPItem& value)
{
  mValues.push_back(value);
}

//-----
void RLPItem::pushBack(const std::vector<std::byte>& value)
{
  mValues.emplace_back(value);
}

//-----
void RLPItem::pushBack(std::string_view value)
{
  mValues.emplace_back(value);
}

//-----
size_t RLPItem::size() const
{
  if (mType == RLPType::VALUE_TYPE)
  {
    return mValue.size();
  }
  else
  {
    size_t size = 0;
    std::for_each(mValues.cbegin(), mValues.cend(), [&size](const RLPItem& item) { size += item.size(); });
    return size;
  }
}

//-----
std::vector<std::byte> RLPItem::write() const
{
  if (mType == RLPType::VALUE_TYPE)
  {
    if (mValue.size() == 1 && mValue.at(0) < std::byte(0x80))
    {
      return mValue;
    }
    else
    {
      return internal::Utilities::concatenateVectors({ { encodeLength(mValue.size(), std::byte(0x80)) }, mValue });
    }
  }

  else
  {
    std::vector<std::byte> bytes;
    for (const RLPItem& item : mValues)
    {
      bytes = internal::Utilities::concatenateVectors({ bytes, item.write() });
    }

    return internal::Utilities::concatenateVectors({ { encodeLength(bytes.size(), std::byte(0xC0)) }, bytes });
  }
}

//-----
void RLPItem::read(const std::vector<std::byte>& bytes)
{
  clear();

  if (bytes.empty())
  {
    return;
  }

  const auto [offset, len, type] = decodeLength(bytes);
  if (type == RLPType::VALUE_TYPE)
  {
    mValue = { bytes.cbegin() + offset, bytes.cbegin() + offset + len };
  }
  else
  {
    long bytesConsumed = offset;
    while (bytesConsumed < len)
    {
      RLPItem item;
      item.read({ bytes.cbegin() + bytesConsumed, bytes.cbegin() + len });
      bytesConsumed -= static_cast<long>(item.size());
      mValues.push_back(item);
    }
  }

  mType = type;
}

} // namespace Hedera
