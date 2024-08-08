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
#ifndef HEDERA_SDK_CPP_NODE_CREATE_TRANSACTION_H_
#define HEDERA_SDK_CPP_NODE_CREATE_TRANSACTION_H_

#include "AccountId.h"
#include "Endpoint.h"
#include "Transaction.h"

namespace proto
{
class NodeCreateTransactionBody;
class TransactionBody;
}

namespace Hedera
{
/**
 * A transaction to add a new consensus node to the network address book.
 *
 * This transaction SHALL be considered a "privileged transaction".
 *
 * This message supports a transaction to create a new node in the network
 * address book. The transaction, once complete, enables a new consensus node
 * to join the network, and requires governing council authorization.
 *
 * - A `NodeCreateTransactionBody` MUST be signed by the governing council.
 * - A `NodeCreateTransactionBody` MUST be signed by the `Key` assigned to the
 *   `admin_key` field.
 * - The newly created node information SHALL be added to the network address
 *   book information in the network state.
 * - The new entry SHALL be created in "state" but SHALL NOT participate in
 *   network consensus and SHALL NOT be present in network "configuration"
 *   until the next "upgrade" transaction (as noted below).
 * - All new address book entries SHALL be added to the active network
 *   configuration during the next `freeze` transaction with the field
 *   `freeze_type` set to `PREPARE_UPGRADE`.
 *
 * ### Record Stream Effects
 * Upon completion the newly assigned `node_id` SHALL be in the transaction
 * receipt.
 */
class NodeCreateTransaction : public Transaction<NodeCreateTransaction>
{
public:
  NodeCreateTransaction() = default;

  /**
   * Construct from a TransactionBody protobuf object.
   *
   * @param transactionBody The TransactionBody protobuf object from which to construct.
   * @throws std::invalid_argument If the input TransactionBody does not represent a NodeCreate transaction.
   */
  explicit NodeCreateTransaction(const proto::TransactionBody& transactionBody);

  /**
   * Construct from a map of TransactionIds to node account IDs and their respective Transaction protobuf objects.
   *
   * @param transactions The map of TransactionIds to node account IDs and their respective Transaction protobuf
   *                     objects.
   */
  explicit NodeCreateTransaction(const std::map<TransactionId, std::map<AccountId, proto::Transaction>>& transactions);

  /**
   * Get the node account identifier.
   *
   * @return An account identifier.
   */
  [[nodiscard]] const AccountId& getAccountId() const { return mAccountId; };

  /**
   * Get the short description of the node.
   *
   * @return An optional string containing the description.
   */
  [[nodiscard]] const std::optional<std::string>& getDescription() const { return mDescription; };

  /**
   * Get the list of service endpoints for gossip.
   *
   * @return A vector of gossip service endpoints.
   */
  [[nodiscard]] const std::vector<Endpoint>& getGossipEndpoints() const { return gossipEndpoints; };

  /**
   * Get the list of service endpoints for gRPC calls.
   *
   * @return A vector of gRPC service endpoints.
   */
  [[nodiscard]] const std::vector<Endpoint>& getServiceEndpoints() const { return serviceEndpoints; };

  /**
   * Get the certificate used to sign gossip events.
   *
   * @return A vector of bytes representing the gossip CA certificate in DER encoding.
   */
  [[nodiscard]] const std::vector<std::byte>& getGossipCaCertificate() const { return mGossipCaCertificate; };

  /**
   * Get the hash of the node gRPC TLS certificate.
   *
   * @return A vector of bytes representing the SHA-384 hash of the gRPC TLS certificate.
   */
  [[nodiscard]] const std::optional<std::vector<std::byte>>& getGrpcCertificateHash() const
  {
    return mGrpcCertificateHash;
  };

  /**
   * Get the administrative key controlled by the node operator.
   *
   * @return A shared pointer to the administrative Key.
   */
  [[nodiscard]] const std::shared_ptr<Key>& getAdminKey() const { return mAdminKey; };

  /**
   * Set the node account identifier.
   *
   * @param accountId The AccountId to set.
   * @return A reference to this NodeCreateTransaction with the newly-set account identifier.
   */
  NodeCreateTransaction& setAccountId(const AccountId& accountId);

  /**
   * Set the description for the node.
   *
   * @param description A string containing the description.
   * @return A reference to this NodeCreateTransaction with the newly-set description.
   */
  NodeCreateTransaction& setDescription(const std::optional<std::string>& description);

  /**
   * Set the list of service endpoints for gossip.
   *
   * @param endpoints A vector of Endpoint objects for gossip.
   * @return A reference to this NodeCreateTransaction with the newly-set gossip endpoints.
   */
  NodeCreateTransaction& setGossipEndpoints(const std::vector<Endpoint>& endpoints);

  /**
   * Set the list of service endpoints for gRPC calls.
   *
   * @param endpoints A vector of Endpoint objects for gRPC.
   * @return A reference to this NodeCreateTransaction with the newly-set service endpoints.
   */
  NodeCreateTransaction& setServiceEndpoints(const std::vector<Endpoint>& endpoints);

  /**
   * Set the certificate used to sign gossip events.
   *
   * @param certificate A vector of bytes representing the gossip CA certificate in DER encoding.
   * @return A reference to this NodeCreateTransaction with the newly-set gossip CA certificate.
   */
  NodeCreateTransaction& setGossipCaCertificate(const std::vector<std::byte>& certificate);

  /**
   * Set the hash of the node gRPC TLS certificate.
   *
   * @param hash A vector of bytes representing the SHA-384 hash of the gRPC TLS certificate.
   * @return A reference to this NodeCreateTransaction with the newly-set gRPC certificate hash.
   */
  NodeCreateTransaction& setGrpcCertificateHash(const std::vector<std::byte>& hash);

  /**
   * Set the administrative key controlled by the node operator.
   *
   * @param key A shared pointer to a Key object representing the administrative key.
   * @return A reference to this NodeCreateTransaction with the newly-set administrative key.
   */
  NodeCreateTransaction& setAdminKey(const std::shared_ptr<Key>& key);

private:
  friend class WrappedTransaction;

  /**
   * Derived from Executable. Submit a Transaction protobuf object which contains this NodeCreateTransaction's data to
   * a Node.
   *
   * @param request  The Transaction protobuf object to submit.
   * @param node     The Node to which to submit the request.
   * @param deadline The deadline for submitting the request.
   * @param response Pointer to the ProtoResponseType object that gRPC should populate with the response information
   *                 from the gRPC server.
   * @return The gRPC status of the submission.
   */
  [[nodiscard]] grpc::Status submitRequest(const proto::Transaction& request,
                                           const std::shared_ptr<internal::Node>& node,
                                           const std::chrono::system_clock::time_point& deadline,
                                           proto::TransactionResponse* response) const override;

  /**
   * Derived from Transaction. Verify that all the checksums in this NodeCreateTransaction are valid.
   *
   * @param client The Client that should be used to validate the checksums.
   * @throws BadEntityException This NodeCreateTransaction's checksums are not valid.
   */
  void validateChecksums(const Client& client) const override;

  /**
   * Derived from Transaction. Build and add the NodeCreateTransaction protobuf representation to the Transaction
   * protobuf object.
   *
   * @param body The TransactionBody protobuf object being built.
   */
  void addToBody(proto::TransactionBody& body) const override;

  /**
   * Initialize this NodeCreateTransaction from its source TransactionBody protobuf object.
   */
  void initFromSourceTransactionBody();

  /**
   * Build a NodeCreateTransactionBody protobuf object from this NodeCreateTransaction object.
   *
   * @return A pointer to a NodeCreateTransactionBody protobuf object filled with this NodeCreateTransaction object's
   *         data.
   */
  [[nodiscard]] proto::NodeCreateTransactionBody* build() const;

  /**
   * A Node account identifier.
   * This account identifier MUST be in the "account number" form.
   * This account identifier MUST NOT use the alias field.
   * If the identified account does not exist, this transaction SHALL fail.
   * Multiple nodes MAY share the same node account.
   * This field is REQUIRED.
   */
  AccountId mAccountId;

  /**
   * A short description of the node.
   * This value, if set, MUST NOT exceed 100 bytes when encoded as UTF-8.
   * This field is OPTIONAL.
   */
  std::optional<std::string> mDescription;

  /**
   * A list of service endpoints for gossip.
   * These endpoints SHALL represent the published endpoints to which other
   * consensus nodes may _gossip_ transactions.
   * These endpoints MUST specify a port.
   * This list MUST NOT be empty.
   * This list MUST NOT contain more than `10` entries.
   * The first two entries in this list SHALL be the endpoints published to
   * all consensus nodes.
   * All other entries SHALL be reserved for future use.
   * Each network may have additional requirements for these endpoints.
   * A client MUST check network-specific documentation for those
   * details.
   * If the network configuration value `gossipFqdnRestricted` is set, then
   * all endpoints in this list MUST supply only IP address.
   * If the network configuration value `gossipFqdnRestricted` is _not_ set,
   * then endpoints in this list MAY supply either IP address or FQDN, but
   * MUST NOT supply both values for the same endpoint.
   */
  std::vector<Endpoint> gossipEndpoints;

  /**
   * A list of service endpoints for gRPC calls.
   * These endpoints SHALL represent the published gRPC endpoints to which
   * clients may submit transactions.
   * These endpoints MUST specify a port.
   * Endpoints in this list MAY supply either IP address or FQDN, but MUST
   * NOT supply both values for the same endpoint.
   * This list MUST NOT be empty.
   * This list MUST NOT contain more than `8` entries.
   */
  std::vector<Endpoint> serviceEndpoints;

  /**
   * A certificate used to sign gossip events.
   * This value MUST be a certificate of a type permitted for gossip
   * signatures.
   * This value MUST be the DER encoding of the certificate presented.
   * This field is REQUIRED and MUST NOT be empty.
   */
  std::vector<std::byte> mGossipCaCertificate;

  /**
   * A hash of the node gRPC TLS certificate.
   * This value MAY be used to verify the certificate presented by the node
   * during TLS negotiation for gRPC.
   * This value MUST be a SHA-384 hash.
   * The TLS certificate to be hashed MUST first be in PEM format and MUST be
   * encoded with UTF-8 NFKD encoding to a stream of bytes provided to
   * the hash algorithm.
   * This field is OPTIONAL.
   */
  std::optional<std::vector<std::byte>> mGrpcCertificateHash;

  /**
   * An administrative key controlled by the node operator.
   * This key MUST sign this transaction.
   * This key MUST sign each transaction to update this node.
   * This field MUST contain a valid `Key` value.
   * This field is REQUIRED and MUST NOT be set to an empty `KeyList`.
   */
  std::shared_ptr<Key> mAdminKey;
};

} // namespace Hedera

#endif // HEDERA_SDK_CPP_NODE_CREATE_TRANSACTION_H_
