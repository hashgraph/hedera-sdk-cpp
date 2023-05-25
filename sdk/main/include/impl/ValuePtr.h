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
#ifndef HEDERA_SDK_CPP_VALUE_PTR_H_
#define HEDERA_SDK_CPP_VALUE_PTR_H_

#include <memory>

namespace Hedera
{
template<typename T>
struct DefaultCloner;
}

namespace Hedera
{
/**
 * Helper class used to give value semantics to pointers (notably abstract base classes).
 *
 * @tparam T       The type of value.
 * @tparam Cloner  The cloner function type.
 * @tparam Deleter The deleter function type.
 */
template<class T, class Cloner = DefaultCloner<T>, class Deleter = std::default_delete<T>>
class ValuePtr
{
public:
  ValuePtr() = default;
  ~ValuePtr() = default;

  ValuePtr(const ValuePtr& other)
    : mPtr(other.mCloner(other.mPtr.get()), other.mPtr.get_deleter())
    , mCloner(other.mCloner)
  {
  }

  ValuePtr& operator=(const ValuePtr& other)
  {
    if (this != &other)
    {
      mPtr = { other.mCloner(other.mPtr.get()), other.mPtr.get_deleter() };
      mCloner = other.mCloner;
    }

    return *this;
  }

  ValuePtr(ValuePtr&&) noexcept = default;
  ValuePtr& operator=(ValuePtr&&) noexcept = default;

  explicit ValuePtr(const T& val)
    : mPtr(Cloner()(val), Deleter())
  {
  }

  explicit ValuePtr(T&& val)
    : mPtr(Cloner()(std::move(val)), Deleter())
  {
  }

  explicit ValuePtr(T* val)
    : mPtr(val, Deleter())
  {
  }

  T* release() { return mPtr.release(); }

  T* get() noexcept { return mPtr.get(); }
  Cloner& getCloner() noexcept { return mCloner; }
  Deleter& getDeleter() noexcept { return mPtr.get_deleter(); }
  T& operator*() { return *mPtr.get(); }
  T* operator->() { return mPtr.get(); }

  const T* get() const noexcept { return mPtr.get(); }
  const Cloner& getCloner() const noexcept { return mCloner; };
  const Deleter& getDeleter() const noexcept { return mPtr.get_deleter(); }
  const T& operator*() const { return *mPtr.get(); }
  const T* operator->() const { return mPtr.get(); }

  [[nodiscard]] explicit operator bool() const noexcept { return mPtr != nullptr; }

private:
  /**
   * Underlying std::unique_ptr that contains the value.
   */
  std::unique_ptr<T, Deleter> mPtr = { nullptr, Deleter() };

  /**
   * The cloner function to use to clone this ValuePtr.
   */
  Cloner mCloner;
};

template<typename T>
struct DefaultCloner
{
  DefaultCloner() = default;
  T* operator()(const T& val) const { return new T(val); }
  T* operator()(T&& val) const { return new T(std::move(val)); }
  T* operator()(const T* val) const { return new T(*val); }
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_VALUE_PTR_H_
