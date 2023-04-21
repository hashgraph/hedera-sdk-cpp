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
#ifndef HEDERA_SDK_CPP_CONTRACT_FUNCTION_SELECTOR_H_
#define HEDERA_SDK_CPP_CONTRACT_FUNCTION_SELECTOR_H_

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace Hedera
{
class ContractFunctionSelector
{
public:
  /**
   * Add the function name.
   *
   * @param name The name of the function
   * @return A reference to this ContractFunctionSelector with the newly-added function name.
   */
  ContractFunctionSelector& addName(std::string_view name);

  /**
   * Add a string parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added string parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addString();

  /**
   * Add a string array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added string array parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addStringArray();

  /**
   * Add a bytes parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added bytes parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addBytes();

  /**
   * Add a bytes array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added bytes array parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addBytesArray();

  /**
   * Add a bytes32 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added bytes32 parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addBytes32();

  /**
   * Add a bytes32 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added bytes32 array parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addBytes32Array();

  /**
   * Add a bool parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added bool parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addBool();

  /**
   * Add an int8 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int8 parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addInt8();

  /**
   * Add an int32 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int32 parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addInt32();

  /**
   * Add an int64 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int64 parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addInt64();

  /**
   * Add an int256 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int256 parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addInt256();

  /**
   * Add an int8 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int8 array parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addInt8Array();

  /**
   * Add an int32 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int32 array parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addInt32Array();

  /**
   * Add an int64 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int64 array parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addInt64Array();

  /**
   * Add an int256 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int256 array parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addInt256Array();

  /**
   * Add a uint8 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint8 parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addUint8();

  /**
   * Add a uint32 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint32 parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addUint32();

  /**
   * Add a uint64 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint64 parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addUint64();

  /**
   * Add a uint256 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint256 parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addUint256();

  /**
   * Add a uint8 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint8 array parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addUint8Array();

  /**
   * Add a uint32 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint32 array parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addUint32Array();

  /**
   * Add a uint64 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint64 array parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addUint64Array();

  /**
   * Add a uint256 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint256 array parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addUint256Array();

  /**
   * Add an address parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added address parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addAddress();

  /**
   * Add an address array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added address array parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addAddressArray();

  /**
   * Add a function parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added function parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addFunction();

  /**
   * Get the function selector bytes for all currently added parameters. No more parameters may be added after this
   * method call. However, this can be called multiple times to return the same result.
   *
   * @return The Solidity function selector bytes.
   */
  std::vector<std::byte> finish();

private:
  /**
   * Add a Solidity parameter to this function selector.
   *
   * @param type The name of the type to add.
   * @return A reference to this ContractFunctionSelector with the newly-added parameter.
   * @throws IllegalStateException If the Solidity function selector bytes have already been compiled.
   */
  ContractFunctionSelector& addParameter(std::string_view type);

  /**
   * The bytes of the function.
   */
  std::vector<std::byte> mFunctionCall;

  /**
   * The final compiled Solidity function selector bytes.
   */
  std::vector<std::byte> mSelectorBytes;

  /**
   * Should a comma be added before adding a parameter?
   */
  bool addComma = false;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_FUNCTION_SELECTOR_H_
