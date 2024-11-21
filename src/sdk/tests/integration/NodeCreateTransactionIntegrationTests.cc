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
#include "BaseIntegrationTest.h"
#include "ED25519PrivateKey.h"
#include "FileId.h"
#include "FreezeTransaction.h"
#include "NodeCreateTransaction.h"
#include "TransactionRecord.h"
#include "TransactionResponse.h"
#include "impl/HexConverter.h"

#include <gtest/gtest.h>

using namespace Hedera;

class NodeCreateTransactionIntegrationTests : public BaseIntegrationTest
{
protected:
  [[nodiscard]] const AccountId& getAccountId() const { return mAccountId; }
  [[nodiscard]] const FileId& getFileId() const { return mFileId; }
  [[nodiscard]] const std::vector<Endpoint>& getGossipEndpoints() const { return mGossipEndpoints; }
  [[nodiscard]] const std::vector<Endpoint>& getGrpcServiceEndpoints() const { return mGrpcServiceEndpoints; }
  [[nodiscard]] const std::vector<std::byte> getGossipCertificate() const
  {
    return internal::HexConverter::hexToBytes(mGossipCertificateDer);
  }
  [[nodiscard]] const std::vector<std::byte> getFileHash() const
  {
    return internal::HexConverter::hexToBytes(mFileHash);
  }

private:
  const AccountId mAccountId = AccountId::fromString("0.0.4");
  const FileId mFileId = FileId::fromString("0.0.150");
  // The file hash needs to be taken from the network context to be correct
  const std::string mFileHash =
    "ce52a3c62cf51f046ae2f85ff1c895da2b32876d6aa74d2454b6de9d11f58344e5065c807af5f2a1eb5850b26d016c3f";
  const Endpoint endpoint1 = Endpoint().setDomainName("test.com").setPort(123);
  const Endpoint endpoint2 = Endpoint().setDomainName("test2.com").setPort(123);
  const std::vector<Endpoint> mGossipEndpoints = { endpoint1, endpoint2 };
  const std::vector<Endpoint> mGrpcServiceEndpoints = { endpoint1, endpoint2 };
  const std::string mGossipCertificateDer =
    "3082052830820310a003020102020101300d06092a864886f70d01010c05003010310e300c060355040313056e6f6465333024170d32343130"
    "30383134333233395a181332313234313030383134333233392e3337395a3010310e300c060355040313056e6f64653330820222300d06092a"
    "864886f70d01010105000382020f003082020a0282020100af111cff0c4ad8125d2f4b8691ce87332fecc867f7a94ddc0f3f96514cc4224d44"
    "af516394f7384c1ef0a515d29aa6116b65bc7e4d7e2d848cf79fbfffedae3a6583b3957a438bdd780c4981b800676ea509bc8c619ae04093b5"
    "fc642c4484152f0e8bcaabf19eae025b630028d183a2f47caf6d9f1075efb30a4248679d871beef1b7e9115382270cbdb68682fae4b1fd592c"
    "adb414d918c0a8c23795c7c5a91e22b3e90c410825a2bc1a840efc5bf9976a7f474c7ed7dc047e4ddd2db631b68bb4475f173baa3edc234c4b"
    "ed79c83e2f826f79e07d0aade2d984da447a8514135bfa4145274a7f62959a23c4f0fae5adc6855974e7c04164951d052beb5d45cb1f3cdfd0"
    "05da894dea9151cb62ba43f4731c6bb0c83e10fd842763ba6844ef499f71bc67fa13e4917fb39f2ad18112170d31cdcb3c61c9e3253accf703"
    "dbd8427fdcb87ece78b787b6cfdc091e8fedea8ad95dc64074e1fc6d0e42ea2337e18a5e54e4aaab3791a98dfcef282e2ae1caec9cf986fabe"
    "8f36e6a21c8711647177e492d264415e765a86c58599cd97b103cb4f6a01d2edd06e3b60470cf64daca7aecf831197b466cae04baeeac19840"
    "a05394bef628aed04b611cfa13677724b08ddfd662b02fd0ef0af17eb7f4fb8c1c17fbe9324f6dc7bcc02449622636cc45ec04909b3120ab4d"
    "f4726b21bf79e955fe8f832699d2196dcd7a58bfeafb170203010001a38186308183300f0603551d130101ff04053003020100300e0603551d"
    "0f0101ff0404030204b030200603551d250101ff0416301406082b0601050507030106082b06010505070302301d0603551d0e041604146431"
    "18e05209035edd83d44a0c368de2fb2fe4c0301f0603551d23041830168014643118e05209035edd83d44a0c368de2fb2fe4c0300d06092a86"
    "4886f70d01010c05000382020100ad41c32bb52650eb4b76fce439c9404e84e4538a94916b3dc7983e8b5c58890556e7384601ca7440dde682"
    "33bb07b97bf879b64487b447df510897d2a0a4e789c409a9b237a6ad240ad5464f2ce80c58ddc4d07a29a74eb25e1223db6c00e334d7a27d32"
    "bfa6183a82f5e35bccf497c2445a526eabb0c068aba9b94cc092ea4756b0dcfb574f6179f0089e52b174ccdbd04123eeb6d70daeabd8513fcb"
    "a6be0bc2b45ca9a69802dae11cc4d9ff6053b3a87fd8b0c6bf72fffc3b81167f73cca2b3fd656c5d353c8defca8a76e2ad535f984870a590af"
    "4e28fed5c5a125bf360747c5e7742e7813d1bd39b5498c8eb6ba72f267eda034314fdbc596f6b967a0ef8be5231d364e634444c84e64bd7919"
    "425171016fcd9bb05f01c58a303dee28241f6e860fc3aac3d92aad7dac2801ce79a3b41a0e1f1509fc0d86e96d94edb18616c000152490f645"
    "61713102128990fedd3a5fa642f2ff22dc11bc4dc5b209986a0c3e4eb2bdfdd40e9fdf246f702441cac058dd8d0d51eb0796e2bea2ce1b37b2"
    "a2f468505e1f8980a9f66d719df034a6fbbd2f9585991d259678fb9a4aebdc465d22c240351ed44abffbdd11b79a706fdf7c40158d3da87f68"
    "d7bd557191a8016b5b899c07bf1b87590feb4fa4203feea9a2a7a73ec224813a12b7a21e5dc93fcde4f0a7620f570d31fe27e9b8d65b74db7d"
    "c18a5e51adc42d7805d4661938";
};

//-----
TEST_F(NodeCreateTransactionIntegrationTests, DISABLED_CanExecuteNodeCreateTransaction)
{
  // Given
  const std::shared_ptr<PrivateKey> adminKey = ED25519PrivateKey::generatePrivateKey();

  // When / Then
  TransactionResponse txResponse;

  ASSERT_NO_THROW(txResponse = NodeCreateTransaction()
                                 .setAccountId(getAccountId())
                                 .setGossipEndpoints(getGossipEndpoints())
                                 .setServiceEndpoints(getGrpcServiceEndpoints())
                                 .setGossipCaCertificate(getGossipCertificate())
                                 .setAdminKey(adminKey->getPublicKey())
                                 .freezeWith(&getTestClient())
                                 .sign(adminKey)
                                 .execute(getTestClient()));

  ASSERT_NO_THROW(txResponse = FreezeTransaction()
                                 .setFreezeType(FreezeType::PREPARE_UPGRADE)
                                 .setFileId(getFileId())
                                 .setFileHash(getFileHash())
                                 .freezeWith(&getTestClient())
                                 .execute(getTestClient()));

  ASSERT_NO_THROW(txResponse = FreezeTransaction()
                                 .setFreezeType(FreezeType::FREEZE_UPGRADE)
                                 .setStartTime(std::chrono::system_clock::now() + std::chrono::seconds(5))
                                 .setFileId(getFileId())
                                 .setFileHash(getFileHash())
                                 .freezeWith(&getTestClient())
                                 .execute(getTestClient()));

  TransactionReceipt txReceipt;
  ASSERT_NO_THROW(txReceipt = txResponse.getReceipt(getTestClient()));
}