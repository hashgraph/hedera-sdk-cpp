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
#ifndef HELPER_INIT_TYPE_H_
#define HELPER_INIT_TYPE_H_

namespace Hedera
{
template<typename T>
class InitType
{
public:
  /**
   * Constructor
   */
  InitType()
    : mValue()
    , mInitialized(false)
  {
  }

  /**
   * Constructor with value
   *
   * @param val  The value with which to initialize
   */
  InitType(const T& val) { setValue(val); }

  /**
   * Get the value
   *
   * @return The value
   */
  inline T getValue() const { return mValue; }

  /**
   * Set the value
   *
   * @param val  The value to set
   */
  inline void setValue(const T& val)
  {
    mValue = val;
    mInitialized = true;
  }

  /**
   * @brief Determine if this value has been initialized
   *
   * @return \c TRUE if this value has been initialized,
   *         otherwise \c FALSE
   */
  inline bool isValid() const { return mInitialized; }

private:
  /**
   * Value to keep
   */
  T mValue;

  /**
   * Determine if this value has been initialized
   */
  bool mInitialized;
};

} // namespace Hedera

#endif // HELPER_INIT_TYPE_H_