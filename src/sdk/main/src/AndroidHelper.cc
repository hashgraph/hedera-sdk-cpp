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
#include "AndroidHelper.h"

namespace Hedera::Android
{
namespace
{
AAssetManager* ANDROID_ASSET_MANAGER = nullptr;
}

//-----
void setAssetManager(AAssetManager* assetManager)
{
  ANDROID_ASSET_MANAGER = assetManager;
}

//-----
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
