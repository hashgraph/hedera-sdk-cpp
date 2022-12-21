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
#ifndef HEDERA_SDK_CPP_IMPL_HEDERA_CERTIFICATE_VERIFIER_H_
#define HEDERA_SDK_CPP_IMPL_HEDERA_CERTIFICATE_VERIFIER_H_

#include <grpcpp/security/credentials.h>
#include <string>

namespace Hedera::internal
{
/**
 * Custom TLS verifier which checks if node TLS cert chain matches the expected hash
 */
class HederaCertificateVerifier : public grpc::experimental::ExternalCertificateVerifier
{
public:
  /**
   * Construct with a node certificate chain hash.
   *
   * @param certificateHash The claimed hash of the node certificate chain.
   */
  explicit HederaCertificateVerifier(std::string certificateHash);

private:
  /**
   * The hash of the certificate chain for the node, from the address book.
   */
  std::string mExpectedHash;

  /**
   * The verification logic that will be performed after the TLS handshake completes.
   *
   * @param request     The verification information associated with this request.
   * @param callback    Callback for asynchronous requests. This is unused since the SDK does all requests
   *                    synchronously.
   * @param sync_status Pointer to the gRPC status that should be updated to indicate the success or the failure of the
   *                    check.
   * @return \c TRUE because all checks are done synchronously.
   */
  bool Verify(grpc::experimental::TlsCustomVerificationCheckRequest* request,
              std::function<void(grpc::Status)> /*callback*/,
              grpc::Status* sync_status) override;

  /**
   * Cancels a verification request previously started via Verify(). Used when the connection attempt times out or is
   * cancelled while an async verification request is pending. The impl should abort whatever async operation it is
   * waiting for and quickly invoke the callback that was passed to Verify() with a status indicating the cancellation.
   *
   * @param request A pointer to the verification information associated with this request.
   */
  [[maybe_unused]] void Cancel(grpc::experimental::TlsCustomVerificationCheckRequest* request) override
  { // Intentionally unimplemented
  }
};

} // namespace Hedera::internal

#endif // HEDERA_SDK_CPP_IMPL_HEDERA_CERTIFICATE_VERIFIER_H_
