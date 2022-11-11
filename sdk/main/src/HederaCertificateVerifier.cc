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

#include "HederaCertificateVerifier.h"
#include "helper/HexConverter.h"
#include "helper/OpenSSLHasher.h"

HederaCertificateVerifier::HederaCertificateVerifier(const std::string& certificateHash)
{
  mExpectedHash = certificateHash;
}

bool HederaCertificateVerifier::Verify(grpc::experimental::TlsCustomVerificationCheckRequest* request,
                                       std::function<void(grpc::Status)> callback,
                                       grpc::Status* sync_status)
{
  auto grpcCertificateChain = request->peer_cert_full_chain();
  std::string certificateChain = { grpcCertificateChain.cbegin(), grpcCertificateChain.cend() };

  std::vector<unsigned char> hashBytes = Hedera::OpenSSLHasher::computeSHA384(certificateChain);

  if (mExpectedHash != Hedera::HexConverter::bytesToHex(hashBytes))
  {
    *sync_status = grpc::Status(grpc::StatusCode::UNAUTHENTICATED,
                                "Hash of node certificate chain doesn't match hash contained in address book");

    return true;
  }

  *sync_status = grpc::Status(grpc::StatusCode::OK, "Certificate chain hash matches expected");

  // always true, since this is called synchronously
  return true;
}

void HederaCertificateVerifier::Cancel(grpc::experimental::TlsCustomVerificationCheckRequest* request) {}
