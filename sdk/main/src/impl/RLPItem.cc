/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
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

#include <algorithm>
#include <stdexcept>

namespace Hedera
{
namespace
{
/*
 * Get the raw byte-representation of a number.
 *
 * @param num The number of which to get the byte-representation.
 * @return The byte-representation of the input number.
 */
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

/*
 * Encode a number to its byte-representation, given an offset.
 *
 * @param num    The number to encode.
 * @param offset The offset of the length.
 * @return The byte-encoding of the number.
 */
std::vector<std::byte> encodeLength(size_t num, unsigned char offset)
{
  if (num < 56)
  {
    return { std::byte(offset + num) };
  }
  else
  {
    const std::vector<std::byte> encodedLength = encodeBinary(num);
    return internal::Utilities::concatenateVectors(
      { { std::byte(encodedLength.size() + offset + 55) }, encodedLength });
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
  if (mType != type)
  {
    clear();
    mType = type;
  }
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
      return internal::Utilities::concatenateVectors({ { encodeLength(mValue.size(), 0x80) }, mValue });
    }
  }

  else
  {
    std::vector<std::byte> bytes;
    for (const RLPItem& item : mValues)
    {
      bytes = internal::Utilities::concatenateVectors({ bytes, item.write() });
    }

    return internal::Utilities::concatenateVectors({ { encodeLength(bytes.size(), 0xC0) }, bytes });
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

  try
  {
    long index = 0;
    decodeBytes(bytes, index);
  }
  catch (const std::exception&)
  {
    throw std::invalid_argument("Input byte array does not follow RLP encoding standards and cannot be read");
  }
}

//-----
void RLPItem::decodeBytes(const std::vector<std::byte>& bytes, long& index)
{
  const std::byte prefix = bytes.at(index);
  ++index;

  // Single byte case
  if (prefix < std::byte(0x80))
  {
    mValue = { prefix };
    mType = RLPType::VALUE_TYPE;
  }

  // Short string (<56 bytes) case
  else if (prefix < std::byte(0xB8))
  {
    const long stringLength = static_cast<long>(prefix) - 0x80;
    mValue = { bytes.cbegin() + index, bytes.cbegin() + index + stringLength };
    mType = RLPType::VALUE_TYPE;
    index += stringLength;
  }

  // Long string (>56 bytes) case
  else if (prefix < std::byte(0xC0))
  {
    const long stringLengthLength = static_cast<long>(prefix) - 0xB7;
    long stringLength = 0;
    for (int i = 0; i < stringLengthLength; ++i)
    {
      stringLength <<= 8;
      stringLength += static_cast<long>(bytes.at(index));
      ++index;
    }

    mValue = { bytes.cbegin() + index, bytes.cbegin() + index + stringLength };
    mType = RLPType::VALUE_TYPE;
    index += stringLength;
  }

  // Short list (<56 bytes) case
  else if (prefix < std::byte(0xF7))
  {
    const long listLength = static_cast<long>(prefix) - 0xC0;
    const long startIndex = index;
    while (index < startIndex + listLength)
    {
      RLPItem item;
      item.decodeBytes(bytes, index);
      mValues.push_back(item);
    }

    mType = RLPType::LIST_TYPE;
  }

  // Long list (>56 bytes) case
  else
  {
    const long listLengthLength = static_cast<long>(prefix) - 0xF7;
    long listLength = 0;
    for (int i = 0; i < listLengthLength; ++i)
    {
      listLength <<= 8;
      listLength += static_cast<long>(bytes.at(index));
      ++index;
    }

    const long startIndex = index;
    while (index < startIndex + listLength)
    {
      RLPItem item;
      item.decodeBytes(bytes, index);
      mValues.push_back(item);
    }

    mType = RLPType::LIST_TYPE;
  }
}

} // namespace Hedera
