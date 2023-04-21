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
#include "ContractFunctionParameters.h"
#include "exceptions/OpenSSLException.h"
#include "impl/HexConverter.h"
#include "impl/Utilities.h"

#include <cmath>
#include <stdexcept>

namespace Hedera
{
namespace
{
// The length of a solidity address.
constexpr int SOLIDITY_ADDRESS_LEN = 20;
// The length of a hex-encoded solidity address.
constexpr int SOLIDITY_ADDRESS_LEN_HEX = SOLIDITY_ADDRESS_LEN * 2;
// Padding that can substring without new allocations.
const std::vector<std::byte> POSITIVE_PADDING(31, std::byte(0x00));
const std::vector<std::byte> NEGATIVE_PADDING(31, std::byte(0xFF));

/**
 * Get the number of Solidity words there must be to contain the input byte array.
 *
 * @param bytes The bytes of which to determine the Solidity word count.
 * @return The number of Solidity words taken up by the byte array.
 */
[[nodiscard]] size_t getNumSolidityWords(const std::vector<std::byte>& bytes)
{
  return static_cast<size_t>(ceil(static_cast<double>(bytes.size()) / 32.0));
}

/**
 * Get the number of bytes that are required to pad the input byte array to be a full Solidity word.
 *
 * @param bytes The byte array to pad.
 * @return The number of bytes that are required to pad the input byte array to be a full Solidity word.
 */
[[nodiscard]] long getPaddingAmount(const std::vector<std::byte>& bytes)
{
  return static_cast<long>(getNumSolidityWords(bytes) * 32ULL - bytes.size());
}

/**
 * Pad the left of the input byte array to be a full Solidity word (or a multiple of).
 *
 * @param bytes    The byte array to pad.
 * @param negative \c TRUE if the bytes represent a negative integer value, otherwise \c FALSE.
 * @return The byte array padded to be a full Solidity word (or a multiple of).
 */
[[nodiscard]] std::vector<std::byte> leftPad(const std::vector<std::byte>& bytes, bool negative = false)
{
  const std::vector<std::byte>& padding = negative ? NEGATIVE_PADDING : POSITIVE_PADDING;
  return internal::Utilities::concatenateVectors({
    {padding.cbegin(), padding.cbegin() + getPaddingAmount(bytes)},
    bytes
  });
}

/**
 * Pad the right of the input byte array to be a full Solidity word (or a multiple of).
 *
 * @param bytes    The byte array to pad.
 * @return The byte array padded to be a full Solidity word (or a multiple of).
 */
[[nodiscard]] std::vector<std::byte> rightPad(const std::vector<std::byte>& bytes, bool = false)
{
  return internal::Utilities::concatenateVectors({
    bytes, {POSITIVE_PADDING.cbegin(), POSITIVE_PADDING.cbegin() + getPaddingAmount(bytes)}
  });
}

/**
 * Checks the length of a bytes32 byte array and throws if too large.
 *
 * @param bytes The byte array of which to check the length.
 * @return The input byte array.
 * @throws std::invalid_argument If the byte array is larger than 32 bytes.
 */
[[nodiscard]] std::vector<std::byte> checkBytes32(const std::vector<std::byte>& bytes)
{
  if (bytes.size() > 32ULL)
  {
    throw std::invalid_argument("bytes32 encoding forbids byte array length greater than 32");
  }

  return bytes;
}

/**
 * Checks the length of an integer 256 byte array and throws if not 32 bytes.
 *
 * @param bytes The byte array of which to check the length.
 * @return The input byte array.
 * @throws std::invalid_argument If the byte array is not 32 bytes.
 */
[[nodiscard]] std::vector<std::byte> checkInt256AndUint256(const std::vector<std::byte>& bytes)
{
  if (bytes.size() != 32ULL)
  {
    throw std::invalid_argument("int256 and uint256 types require exactly 32 bytes");
  }

  return bytes;
}

/**
 * Get the bytes (in big endian) that represent an integral type.
 *
 * @tparam T  The type of integer of which to get the bytes.
 * @param val The value of which to get the bytes.
 * @return An array of bytes that represents the input value.
 */
template<typename T>
[[nodiscard]] std::vector<std::byte> getBytes(const T& val)
{
  // Only allow integral types
  static_assert(std::is_integral_v<T>, "getBytes works only with integral types");

  std::vector<std::byte> bytes(sizeof(T));
  auto byte = internal::Utilities::toTypePtr<std::byte>(&val);
  for (size_t i = 0; i < sizeof(T); ++i)
  {
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
    bytes[sizeof(T) - i - 1] = *byte++;
#else
    bytes[i] = *byte++;
#endif
  }

  return bytes;
}

/**
 * Encode a value to its representative Solidity byte array.
 *
 * @tparam ValType         The type of value to encode.
 * @tparam ToBytesFuncType The function type that can convert the value.
 * @tparam PaddingFuncType The function type that will pad the value.
 * @param val         The value to encode.
 * @param toBytesFunc The function to use to encode the value.
 * @param paddingFunc The function that will pad the encoded value.
 * @param negative    \c TRUE if the value is signed and should be padded as a negative number (i.e. padded with 0xFF
 *                    values instead of 0x00), otherwise \c FALSE.
 * @return The encoded and padded value.
 */
template<typename ValType, class ToBytesFuncType, class PaddingFuncType>
[[nodiscard]] std::vector<std::byte> encodeValue(const ValType& val,
                                                 const ToBytesFuncType& toBytesFunc,
                                                 const PaddingFuncType& paddingFunc,
                                                 bool negative = false)
{
  return paddingFunc(toBytesFunc(val), negative);
}

/**
 * Encode an array of non-integral types to its representative Solidity byte array.
 *
 * @tparam ValType         The type of values in the array to encode.
 * @tparam ToBytesFuncType The function type that can convert the values in the array.
 * @tparam PaddingFuncType The function type that will pad the values in the array.
 * @param valArray    The array to encode.
 * @param toBytesFunc The function to use to encode the values in the array.
 * @param paddingFunc The function that will pad the encoded values in the array.
 * @return The encoded byte array.
 */
template<typename ValType,
         typename ToBytesFuncType,
         typename PaddingFuncType,
         typename std::enable_if_t<!std::is_integral_v<ValType>, int> = 0>
[[nodiscard]] std::vector<std::byte> encodeArray(const std::vector<ValType>& valArray,
                                                 const ToBytesFuncType& toBytesFunc,
                                                 const PaddingFuncType& paddingFunc)
{
  // The first word of an array always contains the number of elements in the array.
  std::vector<std::byte> bytes = encodeValue(valArray.size(), getBytes<decltype(valArray.size())>, leftPad);
  for (const ValType& val : valArray)
  {
    bytes = internal::Utilities::concatenateVectors({ bytes, encodeValue(val, toBytesFunc, paddingFunc) });
  }

  return bytes;
}

/**
 * Encode an array of integral types to its representative Solidity byte array.
 *
 * @tparam ValType         The type of values in the array to encode.
 * @tparam ToBytesFuncType The function type that can convert the values in the array.
 * @tparam PaddingFuncType The function type that will pad the values in the array.
 * @param valArray    The array to encode.
 * @param toBytesFunc The function to use to encode the values in the array.
 * @param paddingFunc The function that will pad the encoded values in the array.
 * @return The encoded byte array.
 */
template<typename ValType,
         typename ToBytesFuncType,
         typename PaddingFuncType,
         typename std::enable_if_t<std::is_integral_v<ValType>, int> = 0>
[[nodiscard]] std::vector<std::byte> encodeArray(const std::vector<ValType>& valArray,
                                                 const ToBytesFuncType& toBytesFunc,
                                                 const PaddingFuncType& paddingFunc)
{
  // The first word of an array always contains the number of elements in the array.
  std::vector<std::byte> bytes = encodeValue(valArray.size(), getBytes<decltype(valArray.size())>, leftPad);
  for (const ValType& val : valArray)
  {
    bytes = internal::Utilities::concatenateVectors({ bytes, encodeValue(val, toBytesFunc, paddingFunc, val < 0) });
  }

  return bytes;
}

/**
 * Encode an array of dynamic types to its representative Solidity byte array.
 *
 * @tparam ValType         The type of values in the array to encode.
 * @tparam ToBytesFuncType The function type that can convert the values in the array.
 * @param valArray    The array to encode.
 * @param toBytesFunc The function to use to encode the values in the array.
 * @return The encoded byte array.
 */
template<typename ValType, typename ToBytesFuncType>
[[nodiscard]] std::vector<std::byte> encodeDynamicArray(const std::vector<ValType>& valArray,
                                                        const ToBytesFuncType& toBytesFunc)
{
  // Make one pass over all the byte arrays, keeping track of several things:
  //  - The offsets of each byte array.
  //  - The length of each byte array.
  //  - The byte array itself.
  std::vector<std::byte> offsets = encodeValue(valArray.size(), getBytes<decltype(valArray.size())>, leftPad);
  std::vector<std::byte> lengthsAndBytes;
  size_t offset = 32ULL * valArray.size();
  for (const auto& val : valArray)
  {
    // Add the current byte array offset to the offsets vector.
    offsets = internal::Utilities::concatenateVectors({ offsets, encodeValue(offset, getBytes<size_t>, leftPad) });

    // Add the byte array length and the right-padded byte array to the lengths and bytes vector.
    const std::vector<std::byte> valBytes = toBytesFunc(val);
    lengthsAndBytes = internal::Utilities::concatenateVectors({ lengthsAndBytes, valBytes });

    // Update the offset with the number of Solidity words in the encoded value.
    offset += 32ULL * getNumSolidityWords(valBytes);
  }

  // Concatenate the offsets and value arrays together.
  return internal::Utilities::concatenateVectors({ offsets, lengthsAndBytes });
}

/**
 * Encode a Solidity address to a byte array.
 *
 * @param address The Solidity address to encode.
 * @return The encoded Solidity address.
 */
[[nodiscard]] std::vector<std::byte> encodeAddress(std::string_view address)
{
  // Remove 0x prefix if it exists
  if (address.find("0x") == 0ULL)
  {
    address.remove_prefix(2);
  }

  if (address.size() != SOLIDITY_ADDRESS_LEN_HEX)
  {
    throw std::invalid_argument("Solidity addresses must be " + std::to_string(SOLIDITY_ADDRESS_LEN_HEX) +
                                " characters");
  }

  try
  {
    return internal::HexConverter::hexToBytes(address);
  }
  catch (const OpenSSLException&)
  {
    throw std::invalid_argument("Solidity address unable to be decoded");
  }
}

/**
 * Encode a byte array.
 *
 * @param bytes The byte array to encode.
 * @return The encoded byte array.
 */
[[nodiscard]] std::vector<std::byte> encodeBytes(const std::vector<std::byte>& bytes)
{
  return internal::Utilities::concatenateVectors(
    { encodeValue(bytes.size(), getBytes<decltype(bytes.size())>, leftPad), rightPad(bytes) });
}

/**
 * Encode a string.
 *
 * @param str The string to encode.
 * @return The encoded string.
 */
[[nodiscard]] std::vector<std::byte> encodeString(std::string_view str)
{
  return encodeBytes(internal::Utilities::stringToByteVector(str));
}

/**
 * A no-op function to be used if a value doesn't need any encoding or padding.
 *
 * @param bytes The byte array with which to do nothing.
 * @return The input byte array, unchanged.
 */
[[nodiscard]] std::vector<std::byte> noop(const std::vector<std::byte>& bytes, bool = false)
{
  return bytes;
}

} // namespace

//-----
ContractFunctionParameters& ContractFunctionParameters::addString(std::string_view param)
{
  mFunction.addString();
  mArguments.emplace_back(encodeValue(param, encodeString, noop), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addStringArray(const std::vector<std::string>& param)
{
  mFunction.addStringArray();
  mArguments.emplace_back(encodeDynamicArray(param, encodeString), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addBytes(const std::vector<std::byte>& param)
{
  mFunction.addBytes();
  mArguments.emplace_back(encodeValue(param, encodeBytes, noop), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addBytesArray(const std::vector<std::vector<std::byte>>& param)
{
  mFunction.addBytesArray();
  mArguments.emplace_back(encodeDynamicArray(param, encodeBytes), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addBytes32(const std::vector<std::byte>& param)
{
  mFunction.addBytes32();
  mArguments.emplace_back(encodeValue(param, checkBytes32, rightPad), false);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addBytes32Array(
  const std::vector<std::vector<std::byte>>& param)
{
  mFunction.addBytes32Array();
  mArguments.emplace_back(encodeArray(param, checkBytes32, rightPad), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addBool(bool param)
{
  mFunction.addBool();
  mArguments.emplace_back(encodeValue(
                            param,
                            [](bool boolParam) {
                              return boolParam ? std::vector<std::byte>{ std::byte(0x01) }
                                               : std::vector<std::byte>{ std::byte(0x00) };
                            },
                            leftPad),
                          false);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addInt8(int8_t param)
{
  mFunction.addInt8();
  mArguments.emplace_back(encodeValue(param, getBytes<int8_t>, leftPad, param < 0), false);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addInt32(int32_t param)
{
  mFunction.addInt32();
  mArguments.emplace_back(encodeValue(param, getBytes<int32_t>, leftPad, param < 0), false);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addInt64(int64_t param)
{
  mFunction.addInt64();
  mArguments.emplace_back(encodeValue(param, getBytes<int64_t>, leftPad, param < 0), false);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addInt256(const std::vector<std::byte>& param)
{
  mFunction.addInt256();
  mArguments.emplace_back(encodeValue(param, checkInt256AndUint256, noop), false);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addInt8Array(const std::vector<int8_t>& param)
{
  mFunction.addInt8Array();
  mArguments.emplace_back(encodeArray(param, getBytes<int8_t>, leftPad), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addInt32Array(const std::vector<int32_t>& param)
{
  mFunction.addInt32Array();
  mArguments.emplace_back(encodeArray(param, getBytes<int32_t>, leftPad), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addInt64Array(const std::vector<int64_t>& param)
{
  mFunction.addInt64Array();
  mArguments.emplace_back(encodeArray(param, getBytes<int64_t>, leftPad), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addInt256Array(const std::vector<std::vector<std::byte>>& param)
{
  mFunction.addInt256Array();
  mArguments.emplace_back(encodeArray(param, checkInt256AndUint256, noop), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addUint8(uint8_t param)
{
  mFunction.addUint8();
  mArguments.emplace_back(encodeValue(param, getBytes<uint8_t>, leftPad), false);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addUint32(uint32_t param)
{
  mFunction.addUint32();
  mArguments.emplace_back(encodeValue(param, getBytes<uint32_t>, leftPad), false);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addUint64(uint64_t param)
{
  mFunction.addUint64();
  mArguments.emplace_back(encodeValue(param, getBytes<uint64_t>, leftPad), false);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addUint256(const std::vector<std::byte>& param)
{
  mFunction.addUint256();
  mArguments.emplace_back(encodeValue(param, checkInt256AndUint256, noop), false);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addUint8Array(const std::vector<uint8_t>& param)
{
  mFunction.addUint8Array();
  mArguments.emplace_back(encodeArray(param, getBytes<uint8_t>, leftPad), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addUint32Array(const std::vector<uint32_t>& param)
{
  mFunction.addUint32Array();
  mArguments.emplace_back(encodeArray(param, getBytes<uint32_t>, leftPad), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addUint64Array(const std::vector<uint64_t>& param)
{
  mFunction.addUint64Array();
  mArguments.emplace_back(encodeArray(param, getBytes<uint64_t>, leftPad), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addUint256Array(
  const std::vector<std::vector<std::byte>>& param)
{
  mFunction.addUint256Array();
  mArguments.emplace_back(encodeArray(param, checkInt256AndUint256, noop), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addAddress(std::string_view param)
{
  mFunction.addAddress();
  mArguments.emplace_back(encodeValue(param, encodeAddress, leftPad), false);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addAddressArray(const std::vector<std::string>& param)
{
  mFunction.addAddressArray();
  mArguments.emplace_back(encodeArray(param, encodeAddress, leftPad), true);
  return *this;
}

//-----
ContractFunctionParameters& ContractFunctionParameters::addFunction(std::string_view address,
                                                                    ContractFunctionSelector& selector)
{
  mFunction.addFunction();
  mArguments.emplace_back(
    encodeValue(
      std::pair<std::string_view, std::vector<std::byte>>{ address, selector.finish() },
      [](const std::pair<std::string_view, std::vector<std::byte>>& functionParams) {
        return internal::Utilities::concatenateVectors({ encodeAddress(functionParams.first), functionParams.second });
      },
      rightPad),
    false);
  return *this;
}

//-----
std::vector<std::byte> ContractFunctionParameters::toBytes(std::string_view name)
{
  // Keep track of the offset for dynamic-length data. They come immediately after the value arguments.
  size_t dynamicOffset = mArguments.size() * 32ULL;

  std::vector<std::byte> paramsBytes;
  std::vector<std::byte> dynamicBytes;

  for (const Argument& arg : mArguments)
  {
    if (arg.mIsDynamic)
    {
      paramsBytes =
        internal::Utilities::concatenateVectors({ paramsBytes, encodeValue(dynamicOffset, getBytes<size_t>, leftPad) });
      dynamicBytes = internal::Utilities::concatenateVectors({ dynamicBytes, arg.mValue });
      dynamicOffset += arg.mValue.size();
    }
    else
    {
      paramsBytes = internal::Utilities::concatenateVectors({ paramsBytes, arg.mValue });
    }
  }

  // Encode the function name if one exists.
  std::vector<std::byte> returnBytes;
  if (!name.empty())
  {
    mFunction.addName(name);
    returnBytes = mFunction.finish();
  }

  return internal::Utilities::concatenateVectors({ returnBytes, paramsBytes, dynamicBytes });
}

} // namespace Hedera
