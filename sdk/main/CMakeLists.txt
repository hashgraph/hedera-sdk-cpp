add_library(${PROJECT_NAME} STATIC
        #    src/AccountAllowanceApproveTransaction.cc
        #    src/AccountAllowanceDeleteTransaction.cc
        src/AccountBalance.cc
        src/AccountBalanceQuery.cc
        src/AccountCreateTransaction.cc
        #    src/AccountDeleteTransaction.cc
        src/AccountId.cc
        #    src/AccountInfo.cc
        #    src/AccountInfoQuery.cc
        #    src/AccountRecordsQuery.cc
        #    src/AccountStakersQuery.cc
        #    src/AccountUpdateTransaction.cc
        src/Client.cc
        src/ContractId.cc
        #    src/ContractByteCodeQuery.cc
        #    src/ContractCallQuery.cc
        #    src/ContractCreateTransaction.cc
        #    src/ContractDeleteTransaction.cc
        #    src/ContractExecuteTransaction.cc
        #    src/ContractFunctionResult.cc
        #    src/ContractInfo.cc
        #    src/ContractInfoQuery.cc
        #    src/ContractUpdateTransaction.cc
        src/ECDSAPrivateKey.cc
        src/ECDSAPublicKey.cc
        src/ED25519PrivateKey.cc
        src/ED25519PublicKey.cc
        #    src/EthereumTransaction.cc
        #    src/EvmAddress.cc
        src/ExchangeRate.cc
        src/ExchangeRateSet.cc
        src/Executable.cc
        #    src/FileAppendTransaction.cc
        #    src/FileContentsQuery.cc
        #    src/FileCreateTransaction.cc
        src/Mnemonic.cc
        src/MnemonicBIP39.cc
        src/PublicKey.cc
        src/Status.cc
        src/Transaction.cc
        src/TransactionId.cc
        src/TransactionReceipt.cc
        src/TransactionReceiptQuery.cc
        src/TransactionRecord.cc
        src/TransactionRecordQuery.cc
        src/TransactionResponse.cc
        src/Transfer.cc
        src/TransferTransaction.cc

        src/impl/BigNumber.cc
        src/impl/DerivationPathUtils.cc
        src/impl/Endpoint.cc
        src/impl/DurationConverter.cc
        src/impl/HederaCertificateVerifier.cc
        src/impl/HexConverter.cc
        src/impl/IPv4Address.cc
        src/impl/Network.cc
        src/impl/Node.cc
        src/impl/NodeAddress.cc
        src/impl/NodeAddressBook.cc
        src/impl/OpenSSLRandom.cc
        src/impl/OpenSSLHasher.cc
        src/impl/TimestampConverter.cc)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PUBLIC ${HAPI_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${ZLIB_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENSSL_INCLUDE_DIR})

if (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OSX_CORE_FOUNDATION})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OSX_CF_NETWORK})
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE hapi::proto)

#target_link_libraries(${PROJECT_NAME} PRIVATE ${RESOLV_LIBRARY})
if (NOT WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE sys::resolv)
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenSSL::Crypto
        OpenSSL::SSL)

target_link_libraries(${PROJECT_NAME} PRIVATE
        gRPC::address_sorting
        gRPC::gpr
        gRPC::grpc
        gRPC::grpc_plugin_support
        gRPC::grpc_unsecure
        protobuf::libprotobuf)

target_link_libraries(${PROJECT_NAME} PRIVATE
        gRPC::grpc++
        gRPC::grpc++_alts
        gRPC::grpc++_error_details
        gRPC::grpc++_unsecure)

if (TARGET gRPC::grpc++_reflection)
    target_link_libraries(${PROJECT_NAME} PRIVATE gRPC::grpc++_reflection)
endif ()

if (TARGET gRPC::grpcpp_channelz)
    target_link_libraries(${PROJECT_NAME} PRIVATE gRPC::grpcpp_channelz)
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE
        absl::bad_any_cast_impl
        absl::bad_optional_access
        absl::bad_variant_access
        absl::base
        absl::city
        absl::civil_time
        absl::cord
        absl::cord_internal
        absl::cordz_functions
        absl::cordz_handle
        absl::cordz_info
        absl::cordz_sample_token
        absl::debugging_internal
        absl::demangle_internal
        absl::examine_stack
        absl::exponential_biased
        absl::failure_signal_handler
        absl::flags
        absl::flags_commandlineflag
        absl::flags_commandlineflag_internal
        absl::flags_config
        absl::flags_internal
        absl::flags_marshalling
        absl::flags_parse
        absl::flags_private_handle_accessor
        absl::flags_program_name
        absl::flags_reflection
        absl::flags_usage
        absl::flags_usage_internal
        absl::graphcycles_internal
        absl::hash
        absl::hashtablez_sampler
        absl::int128
        absl::leak_check
        absl::log_severity
        absl::low_level_hash
        absl::malloc_internal
        absl::periodic_sampler
        absl::random_distributions
        absl::random_internal_distribution_test_util
        absl::random_internal_platform
        absl::random_internal_pool_urbg
        absl::random_internal_randen
        absl::random_internal_randen_hwaes
        absl::random_internal_randen_hwaes_impl
        absl::random_internal_randen_slow
        absl::random_internal_seed_material
        absl::random_seed_gen_exception
        absl::random_seed_sequences
        absl::raw_hash_set
        absl::raw_logging_internal
        absl::scoped_set_env
        absl::spinlock_wait
        absl::stacktrace
        absl::status
        absl::statusor
        absl::str_format_internal
        absl::strerror
        absl::strings
        absl::strings_internal
        absl::symbolize
        absl::synchronization
        absl::throw_delegate
        absl::time
        absl::time_zone)

target_link_libraries(${PROJECT_NAME} PRIVATE
        upb::upb
        upb::descriptor_upb_proto
        upb::extension_registry
        upb::fastdecode
        upb::json
        upb::mini_table
        upb::reflection
        upb::textformat
        upb::utf8_range)

target_link_libraries(${PROJECT_NAME} PRIVATE
        c-ares::cares
        re2::re2
        ${ZLIB_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSIONS 0.1.0)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

# Install Library & Headers
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(DIRECTORY include/ TYPE INCLUDE FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${HAPI_INCLUDE_DIR}/ TYPE INCLUDE)
install(DIRECTORY ${HAPI_LIB_DIR}/ TYPE LIB)
install(DIRECTORY ${Protobuf_INCLUDE_DIRS} DESTINATION ${CMAKE_INSTALL_PREFIX})