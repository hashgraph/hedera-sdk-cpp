/*-
 *
 * Hedera C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hedera Hashgraph, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef HEDERA_SDK_CPP_ANDROID_HELPER_H_
#define HEDERA_SDK_CPP_ANDROID_HELPER_H_

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <cstddef>
#include <jni.h>
#include <string_view>
#include <vector>

namespace Hedera
{
/**
 * The AssetManager used by Android APK's to get their assets. There will only ever need to be one of these for each
 * APK. The lifetime of this is managed by the Android framework.
 */
AAssetManager* ANDROID_ASSET_MANAGER = nullptr;

/**
 * Get an asset from the Android APK.
 *
 * @param filepath The filepath to the asset.
 * @return The bytes of the asset.
 */
std::vector<std::byte> getAsset(std::string_view filepath)
{
  AAsset* asset = AAssetManager_open(ANDROID_ASSET_MANAGER, filepath.data(), AASSET_MODE_UNKNOWN);
  if (asset)
  {
    size_t size = AAsset_getLength(asset);
    std::vector<std::byte> bytes;
    bytes.reserve(size);
    AAsset_read(asset, bytes.data(), size);
    AAsset_close(asset);

    return bytes;
  }

  return {};
}

} // namespace Hedera

#endif // HEDERA_SDK_CPP_ANDROID_HELPER_H_
