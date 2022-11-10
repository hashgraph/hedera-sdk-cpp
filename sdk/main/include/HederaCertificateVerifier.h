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

#ifndef HEDERA_SDK_CPP_HEDERACERTIFICATEVERIFIER_H
#define HEDERA_SDK_CPP_HEDERACERTIFICATEVERIFIER_H

#include <grpcpp/security/credentials.h>

/**
 * Custom TLS verifier which checks if node TLS cert chain matches the expected hash
 */
class HederaCertificateVerifier : public grpc::experimental::ExternalCertificateVerifier
{
public:
  /**
   * Constructor
   *
   * @param certificateHash the claimed hash of the node certificate chain
   */
  explicit HederaCertificateVerifier(const std::string& certificateHash);

private:
  /**
   * The hash of the certificate chain for the node, from the address book
   */
  std::string mExpectedHash;

  /**
   * The verification logic that will be performed after the TLS handshake
   * completes
   *
   * @param request the verification information associated with this request
   * @param callback only used for asynchronous verification (therefore unused in this case, since verification is
   * currently synchronous)
   * @param sync_status This should only be used if your check is done synchronously. Modifies this value to indicate
   * the success or the failure of the check.
   * @return true if your check is done synchronously, otherwise return false. always true in this case
   */
  bool Verify(grpc::experimental::TlsCustomVerificationCheckRequest* request,
              std::function<void(grpc::Status)> callback,
              grpc::Status* sync_status) override;

  /**
   * Cancels a verification request previously started via Verify().
   * Used when the connection attempt times out or is cancelled while an async
   * verification request is pending. The implementation should abort whatever
   * async operation it is waiting for and quickly invoke the callback that was
   * passed to Verify() with a status indicating the cancellation.
   *
   * @param request the verification information associated with this request
   */
  void Cancel(grpc::experimental::TlsCustomVerificationCheckRequest* request) override;
};

#endif // HEDERA_SDK_CPP_HEDERACERTIFICATEVERIFIER_H
