/*-
 *
 * Hiero C++ SDK
 *
 * Copyright (C) 2020 - 2024 Hiero
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
#include "SemanticVersion.h"
#include "impl/Utilities.h"

#include <basic_types.pb.h>

namespace Hiero
{
//-----
SemanticVersion::SemanticVersion(int major, int minor, int patch, std::string_view pre, std::string_view build)
  : mMajor(major)
  , mMinor(minor)
  , mPatch(patch)
  , mPre(pre)
  , mBuild(build)
{
}

//-----
bool SemanticVersion::operator==(const SemanticVersion& other) const
{
  return (mMajor == other.mMajor) && (mMinor == other.mMinor) && (mPatch == other.mPatch) && (mPre == other.mPre) &&
         (mBuild == other.mBuild);
}

//-----
SemanticVersion SemanticVersion::fromProtobuf(const proto::SemanticVersion& proto)
{
  return { proto.major(), proto.minor(), proto.patch(), proto.pre(), proto.build() };
}

//-----
SemanticVersion SemanticVersion::fromBytes(const std::vector<std::byte>& bytes)
{
  proto::SemanticVersion semanticVersion;
  semanticVersion.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()));
  return fromProtobuf(semanticVersion);
}

//-----
std::unique_ptr<proto::SemanticVersion> SemanticVersion::toProtobuf() const
{
  auto proto = std::make_unique<proto::SemanticVersion>();
  proto->set_major(mMajor);
  proto->set_minor(mMinor);
  proto->set_patch(mPatch);
  proto->set_pre(mPre);
  proto->set_build(mBuild);
  return proto;
}

//-----
std::vector<std::byte> SemanticVersion::toBytes() const
{
  return internal::Utilities::stringToByteVector(toProtobuf()->SerializeAsString());
}

//-----
std::string SemanticVersion::toString() const
{
  std::string str = std::to_string(mMajor) + '.' + std::to_string(mMinor) + '.' + std::to_string(mPatch);

  if (!mPre.empty())
  {
    str += '-' + mPre;
  }

  if (!mBuild.empty())
  {
    str += '+' + mBuild;
  }

  return str;
}

} // namespace Hiero
