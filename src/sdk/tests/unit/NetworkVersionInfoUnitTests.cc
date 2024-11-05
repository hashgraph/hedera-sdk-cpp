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
#include "NetworkVersionInfo.h"
#include "SemanticVersion.h"
#include "impl/Utilities.h"

#include <gtest/gtest.h>
#include <network_get_version_info.pb.h>

using namespace Hedera;

class NetworkVersionInfoUnitTests : public ::testing::Test
{
protected:
  [[nodiscard]] inline const SemanticVersion& getTestHapiVersion() const { return mTestHapiVersion; }
  [[nodiscard]] inline const SemanticVersion& getTestServicesVersion() const { return mTestServicesVersion; }

private:
  const SemanticVersion mTestHapiVersion = SemanticVersion(1, 2, 3);
  const SemanticVersion mTestServicesVersion = SemanticVersion(4, 5, 6);
};

//-----
TEST_F(NetworkVersionInfoUnitTests, ConstructWithValues)
{
  // Given / When
  const NetworkVersionInfo networkVersionInfo(getTestHapiVersion(), getTestServicesVersion());

  // Then
  EXPECT_EQ(networkVersionInfo.mProtobufVersion, getTestHapiVersion());
  EXPECT_EQ(networkVersionInfo.mServicesVersion, getTestServicesVersion());
}

//-----
TEST_F(NetworkVersionInfoUnitTests, FromProtobuf)
{
  // Given
  proto::NetworkGetVersionInfoResponse networkGetVersionInfoResponse;
  networkGetVersionInfoResponse.set_allocated_hapiprotoversion(getTestHapiVersion().toProtobuf().release());
  networkGetVersionInfoResponse.set_allocated_hederaservicesversion(getTestServicesVersion().toProtobuf().release());

  // When
  const NetworkVersionInfo networkVersionInfo = NetworkVersionInfo::fromProtobuf(networkGetVersionInfoResponse);

  // Then
  EXPECT_EQ(networkVersionInfo.mProtobufVersion, getTestHapiVersion());
  EXPECT_EQ(networkVersionInfo.mServicesVersion, getTestServicesVersion());
}

//-----
TEST_F(NetworkVersionInfoUnitTests, FromBytes)
{
  // Given
  proto::NetworkGetVersionInfoResponse networkGetVersionInfoResponse;
  networkGetVersionInfoResponse.set_allocated_hapiprotoversion(getTestHapiVersion().toProtobuf().release());
  networkGetVersionInfoResponse.set_allocated_hederaservicesversion(getTestServicesVersion().toProtobuf().release());

  // When
  const NetworkVersionInfo networkVersionInfo = NetworkVersionInfo::fromBytes(
    internal::Utilities::stringToByteVector(networkGetVersionInfoResponse.SerializeAsString()));

  // Then
  EXPECT_EQ(networkVersionInfo.mProtobufVersion, getTestHapiVersion());
  EXPECT_EQ(networkVersionInfo.mServicesVersion, getTestServicesVersion());
}

//-----
TEST_F(NetworkVersionInfoUnitTests, ToProtobuf)
{
  // Given
  const NetworkVersionInfo networkVersionInfo(getTestHapiVersion(), getTestServicesVersion());

  // When
  const std::unique_ptr<proto::NetworkGetVersionInfoResponse> protoNetworkVersionInfo = networkVersionInfo.toProtobuf();

  // Then
  EXPECT_EQ(protoNetworkVersionInfo->hapiprotoversion().major(), getTestHapiVersion().mMajor);
  EXPECT_EQ(protoNetworkVersionInfo->hapiprotoversion().minor(), getTestHapiVersion().mMinor);
  EXPECT_EQ(protoNetworkVersionInfo->hapiprotoversion().patch(), getTestHapiVersion().mPatch);
  EXPECT_EQ(protoNetworkVersionInfo->hapiprotoversion().pre(), getTestHapiVersion().mPre);
  EXPECT_EQ(protoNetworkVersionInfo->hapiprotoversion().build(), getTestHapiVersion().mBuild);

  EXPECT_EQ(protoNetworkVersionInfo->hederaservicesversion().major(), getTestServicesVersion().mMajor);
  EXPECT_EQ(protoNetworkVersionInfo->hederaservicesversion().minor(), getTestServicesVersion().mMinor);
  EXPECT_EQ(protoNetworkVersionInfo->hederaservicesversion().patch(), getTestServicesVersion().mPatch);
  EXPECT_EQ(protoNetworkVersionInfo->hederaservicesversion().pre(), getTestServicesVersion().mPre);
  EXPECT_EQ(protoNetworkVersionInfo->hederaservicesversion().build(), getTestServicesVersion().mBuild);
}

//-----
TEST_F(NetworkVersionInfoUnitTests, ToBytes)
{
  // Given
  const NetworkVersionInfo networkVersionInfo(getTestHapiVersion(), getTestServicesVersion());

  // When
  const std::vector<std::byte> bytes = networkVersionInfo.toBytes();

  // Then
  EXPECT_EQ(bytes, internal::Utilities::stringToByteVector(networkVersionInfo.toProtobuf()->SerializeAsString()));
}
