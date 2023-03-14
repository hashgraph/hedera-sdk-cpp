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
#ifndef HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_OPENSSL_OBJECT_WRAPPER_H_
#define HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_OPENSSL_OBJECT_WRAPPER_H_

#include <functional>
#include <memory>

namespace Hedera::internal::OpenSSLUtils
{
/**
 * Templated base wrapper class to be used for OpenSSL objects that require custom deleter and copier functions.
 *
 * @tparam ObjectType  The type of OpenSSL object this class should wrap.
 * @tparam CopierFunc  The copier type (function signature) for the OpenSSL object.
 */
template<typename ObjectType, typename CopierFunc = std::function<ObjectType*(const ObjectType*)>>
class OpenSSLObjectWrapper
{
public:
  virtual ~OpenSSLObjectWrapper() = default;

  /**
   * Copy constructor and assignment operator use this OpenSSLObjectWrapper's custom copier to copy the wrapped OpenSSL
   * object.
   */
  OpenSSLObjectWrapper(const OpenSSLObjectWrapper& other)
    : mObject({ other.mCopier(other.mObject.get()), other.mObject.get_deleter() })
    , mCopier(other.mCopier)
  {
  }

  OpenSSLObjectWrapper& operator=(const OpenSSLObjectWrapper& other)
  {
    if (this != &other)
    {
      mObject = { other.mCopier(other.mObject.get()), other.mObject.get_deleter() };
      mCopier = other.mCopier;
    }

    return *this;
  }

  OpenSSLObjectWrapper(OpenSSLObjectWrapper&&) noexcept = default;
  OpenSSLObjectWrapper& operator=(OpenSSLObjectWrapper&&) noexcept = default;

  /**
   * Get the wrapped OpenSSL object.
   *
   * @return A pointer to the wrapped OpenSSL object, nullptr if no object exists.
   */
  [[nodiscard]] ObjectType* get() { return mObject.get(); }
  [[nodiscard]] const ObjectType* get() const { return mObject.get(); }

  /**
   * Release ownership of the wrapped OpenSSL object. This will cause memory leaks if ownership has not already been
   * taken by another object.
   */
  void release() { mObject.release(); }

  /**
   * Determine if this OpenSSLObjectWrapper has a valid OpenSSL object.
   *
   * @return \c TRUE if there exists an OpenSSL object, otherwise \c FALSE.
   */
  [[nodiscard]] explicit operator bool() const { return mObject != nullptr; }

protected:
  OpenSSLObjectWrapper() = default;

  /**
   * Construct with values for the object, its custom deleter, and optionally a custom copier.
   *
   * @param object  The OpenSSL object to wrap.
   * @param deleter The deleter function for the OpenSSL object.
   * @param copier  The copier function for the OpenSSL object.
   */
  OpenSSLObjectWrapper(ObjectType* object,
                       const std::function<void(ObjectType*)>& deleter,
                       const CopierFunc& copier = CopierFunc())
    : mObject({ object, deleter })
    , mCopier(copier)
  {
  }

private:
  /**
   * Pointer to the OpenSSL object with its associated deleter.
   */
  std::unique_ptr<ObjectType, std::function<void(ObjectType*)>> mObject = { nullptr,
                                                                            std::function<void(ObjectType*)>() };

  /**
   * The copier function to use to copy the wrapped OpenSSL object.
   */
  CopierFunc mCopier;
};

} // namespace Hedera::internal::Utils

#endif // HEDERA_SDK_CPP_IMPL_OPENSSL_UTILS_OPENSSL_OBJECT_WRAPPER_H_
