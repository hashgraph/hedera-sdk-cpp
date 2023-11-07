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
   * Add a string parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added string parameter.
   */
  ContractFunctionSelector& addString();

  /**
   * Add a string array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added string array parameter.
   */
  ContractFunctionSelector& addStringArray();

  /**
   * Add a bytes parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added bytes parameter.
   */
  ContractFunctionSelector& addBytes();

  /**
   * Add a bytes array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added bytes array parameter.
   */
  ContractFunctionSelector& addBytesArray();

  /**
   * Add a bytes32 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added bytes32 parameter.
   */
  ContractFunctionSelector& addBytes32();

  /**
   * Add a bytes32 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added bytes32 array parameter.
   */
  ContractFunctionSelector& addBytes32Array();

  /**
   * Add a bool parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added bool parameter.
   */
  ContractFunctionSelector& addBool();

  /**
   * Add an int8 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int8 parameter.
   */
  ContractFunctionSelector& addInt8();

  /**
   * Add an int32 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int32 parameter.
   */
  ContractFunctionSelector& addInt32();

  /**
   * Add an int64 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int64 parameter.
   */
  ContractFunctionSelector& addInt64();

  /**
   * Add an int256 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int256 parameter.
   */
  ContractFunctionSelector& addInt256();

  /**
   * Add an int8 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int8 array parameter.
   */
  ContractFunctionSelector& addInt8Array();

  /**
   * Add an int32 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int32 array parameter.
   */
  ContractFunctionSelector& addInt32Array();

  /**
   * Add an int64 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int64 array parameter.
   */
  ContractFunctionSelector& addInt64Array();

  /**
   * Add an int256 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added int256 array parameter.
   */
  ContractFunctionSelector& addInt256Array();

  /**
   * Add a uint8 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint8 parameter.
   */
  ContractFunctionSelector& addUint8();

  /**
   * Add a uint32 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint32 parameter.
   */
  ContractFunctionSelector& addUint32();

  /**
   * Add a uint64 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint64 parameter.
   */
  ContractFunctionSelector& addUint64();

  /**
   * Add a uint256 parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint256 parameter.
   */
  ContractFunctionSelector& addUint256();

  /**
   * Add a uint8 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint8 array parameter.
   */
  ContractFunctionSelector& addUint8Array();

  /**
   * Add a uint32 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint32 array parameter.
   */
  ContractFunctionSelector& addUint32Array();

  /**
   * Add a uint64 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint64 array parameter.
   */
  ContractFunctionSelector& addUint64Array();

  /**
   * Add a uint256 array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added uint256 array parameter.
   */
  ContractFunctionSelector& addUint256Array();

  /**
   * Add an address parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added address parameter.
   */
  ContractFunctionSelector& addAddress();

  /**
   * Add an address array parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added address array parameter.
   */
  ContractFunctionSelector& addAddressArray();

  /**
   * Add a function parameter.
   *
   * @return A reference to this ContractFunctionSelector with the newly-added function parameter.
   */
  ContractFunctionSelector& addFunction();

  /**
   * Get the function selector bytes for all currently added parameters.
   *
   * @param name The name of the function.
   * @return The Solidity function selector bytes.
   */
  [[nodiscard]] std::vector<std::byte> finish(std::string_view name = "") const;

private:
  /**
   * Add a Solidity parameter to this function selector.
   *
   * @param type The name of the type to add.
   * @return A reference to this ContractFunctionSelector with the newly-added parameter.
   */
  ContractFunctionSelector& addParameter(std::string_view type);

  /**
   * The bytes of the function.
   */
  std::vector<std::byte> mFunctionCall;

  /**
   * Should a comma be added before adding a parameter?
   */
  bool addComma = false;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_CONTRACT_FUNCTION_SELECTOR_H_
