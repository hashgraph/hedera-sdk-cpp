set(HAPI_LIBRARY_HASH "dafc0ad0181a05f1bccd0bc041f32be8b8cff3c7b7c7ffbec8062d7355c3e832" CACHE STRING "Use the configured hash to verify the Hedera API protobuf library release")
set(HAPI_LIBRARY_URL "https://github.com/hashgraph/hedera-protobufs-cpp/releases/download/v0.30.0/hapi-library-61c288b7.tar.gz" CACHE STRING "Use the configured URL to download the Hedera API protobuf library package")

set(HAPI_LOCAL_LIBRARY_PATH "" CACHE STRING "Overrides the configured HAPI_LIBRARY_URL setting and instead uses the local path to retrieve the artifacts")

if(HAPI_LOCAL_LIBRARY_PATH STREQUAL "")
    # Fetch the HAPI Library
    FetchContent_Declare(
            hapi
            URL ${HAPI_LIBRARY_URL}
            URL_HASH SHA256=${HAPI_LIBRARY_HASH}
    )
    set(FETCHCONTENT_QUIET OFF)
    FetchContent_MakeAvailable(hapi)
else()
    set(hapi_SOURCE_DIR ${HAPI_LOCAL_LIBRARY_PATH})
endif()

set(HAPI_ROOT_DIR ${hapi_SOURCE_DIR}/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR})

if (NOT EXISTS ${HAPI_ROOT_DIR})
    set(HAPI_ROOT_DIR ${hapi_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR})

    if (NOT EXISTS ${HAPI_ROOT_DIR})
        message(FATAL_ERROR "Failed to the HAPI_ROOT_DIR at `${HAPI_ROOT_DIR}`")
    endif ()
endif ()

set(HAPI_INCLUDE_DIR ${HAPI_ROOT_DIR}/include)
set(HAPI_LIB_DIR ${HAPI_ROOT_DIR}/lib)

function(create_req_link_target prefix name nixlib winlib)
    string(TOUPPER "${prefix}" prefixUpper)
    string(TOUPPER "${name}" nameUpper)
    if (prefix STREQUAL "")
        set(variableName "${nameUpper}_LINK_TARGET")
    else()
        set(variableName "${prefixUpper}_${nameUpper}_LINK_TARGET")
    endif()

    set(linkTarget "${HAPI_LIB_DIR}/${nixlib}")
    if (NOT EXISTS ${linkTarget})
        set(linkTarget "${HAPI_LIB_DIR}/${winlib}")

        if(NOT EXISTS ${linkTarget})
            message(FATAL_ERROR "Failed to locate the ${variableName} at `${HAPI_LIB_DIR}/(${nixlib}|${winlib})`")
        endif()
    endif()

    set(${variableName} ${linkTarget} PARENT_SCOPE)
endfunction()

function(create_opt_link_target prefix name nixlib winlib)
    string(TOUPPER "${prefix}" prefixUpper)
    string(TOUPPER "${name}" nameUpper)
    if (prefix STREQUAL "")
        set(variableName "${nameUpper}_LINK_TARGET")
    else()
        set(variableName "${prefixUpper}_${nameUpper}_LINK_TARGET")
    endif()

    set(linkTarget "${HAPI_LIB_DIR}/${nixlib}")
    if (NOT EXISTS ${linkTarget})
        set(linkTarget "${HAPI_LIB_DIR}/${winlib}")

        if(NOT EXISTS ${linkTarget})
            set(linkTarget "")
            message(WARNING "Failed to locate the ${variableName} at `${HAPI_LIB_DIR}/(${nixlib}|${winlib})`")
        endif()
    endif()

    set(${variableName} ${linkTarget} PARENT_SCOPE)
endfunction()

function(create_external_package prefix)
    math(EXPR lastIndex "${ARGC}-1")
    string(TOUPPER "${prefix}" prefixUpper)

    # Create all the imported library targets first
    foreach(index RANGE 1 ${lastIndex})
        string(TOUPPER "${ARGV${index}}" nameUpper)
        if(prefix STREQUAL "")
            set(LIBNAME${index} "${ARGV${index}}")
            set(LINKTARGET${index} "${nameUpper}_LINK_TARGET")
        else()
            set(LIBNAME${index} "${prefix}::${ARGV${index}}")
            set(LINKTARGET${index} "${prefixUpper}_${nameUpper}_LINK_TARGET")
        endif()

#        message("LIBNAME${index}=${LIBNAME${index}}")
#        message("LINKTARGET${index}=${LINKTARGET${index}}")
        if (NOT ${${LINKTARGET${index}}} STREQUAL "")
            add_library(${LIBNAME${index}} STATIC IMPORTED)
        endif()
    endforeach()

    # Now import the link targets
    foreach(index RANGE 1 ${lastIndex})
        if (NOT ${${LINKTARGET${index}}} STREQUAL "")
            set_target_properties(${LIBNAME${index}} PROPERTIES IMPORTED_LOCATION ${${LINKTARGET${index}}})
        endif()
    endforeach()
endfunction()


create_req_link_target(hapi proto libhapi.a hapi.lib)
create_external_package(hapi proto)

#create_req_link_target(c-ares cares libcares.a cares.lib)
#create_external_package(c-ares cares)
#
#create_req_link_target(zlib zlib libz.a zlib.lib)
#create_external_package(zlib zlib)
#
#create_req_link_target(re2 re2 libre2.a re2.lib)
#create_external_package(re2 re2)

#create_req_link_target(upb upb libupb.a upb.lib)
#create_req_link_target(upb descriptor_upb_proto libdescriptor_upb_proto.a descriptor_upb_proto.lib)
#create_req_link_target(upb collections libupb_collections.a upb_collections.lib)
#create_req_link_target(upb extension_registry libupb_extension_registry.a upb_extension_registry.lib)
#create_req_link_target(upb fastdecode libupb_fastdecode.a upb_fastdecode.lib)
#create_req_link_target(upb json libupb_json.a upb_json.lib)
#create_req_link_target(upb mini_table libupb_mini_table.a upb_mini_table.lib)
#create_req_link_target(upb reflection libupb_reflection.a upb_reflection.lib)
#create_req_link_target(upb textformat libupb_textformat.a upb_textformat.lib)
#create_req_link_target(upb utf8_range libupb_utf8_range.a upb_utf8_range.lib)
#create_external_package(upb
#        upb
#        collections
#        descriptor_upb_proto
#        extension_registry
#        fastdecode
#        json
#        mini_table
#        reflection
#        textformat
#        utf8_range)

#create_req_link_target(OpenSSL Crypto libcrypto.a libcrypto.lib)
#create_req_link_target(OpenSSL SSL libssl.a libssl.lib)
#create_external_package(OpenSSL Crypto SSL)
#
#create_req_link_target(gRPC address_sorting libaddress_sorting.a address_sorting.lib)
#create_req_link_target(gRPC gpr libgpr.a gpr.lib)
#create_req_link_target(gRPC grpc libgrpc.a grpc.lib)
#create_req_link_target(gRPC grpc_plugin_support libgrpc_plugin_support.a grpc_plugin_support.lib)
#create_req_link_target(gRPC grpc_unsecure libgrpc_unsecure.a grpc_unsecure.lib)
#create_req_link_target(gRPC grpc++ libgrpc++.a grpc++.lib)
#create_req_link_target(gRPC grpc++_alts libgrpc++_alts.a grpc++_alts.lib)
#create_req_link_target(gRPC grpc++_error_details libgrpc++_error_details.a grpc++_error_details.lib)
#create_req_link_target(gRPC grpc++_unsecure libgrpc++_unsecure.a grpc++_unsecure.lib)
#
#create_opt_link_target(gRPC grpc++_reflection libgrpc++_reflection.a grpc++_reflection.lib)
#create_opt_link_target(gRPC grpcpp_channelz libgrpcpp_channelz.a grpcpp_channelz.lib)
#
#
#create_external_package(gRPC
#        address_sorting
#        gpr
#        grpc
#        grpc_plugin_support
#        grpc_unsecure
#        grpcpp_channelz
#        grpc++
#        grpc++_alts
#        grpc++_error_details
#        grpc++_reflection
#        grpc++_unsecure)
#
#
#create_req_link_target(protobuf protobuf libprotobuf.a libprotobuf.lib)
#create_external_package(protobuf protobuf)
#
#create_req_link_target(absl bad_any_cast_impl libabsl_bad_any_cast_impl.a absl_bad_any_cast_impl.lib)
#create_req_link_target(absl bad_optional_access libabsl_bad_optional_access.a absl_bad_optional_access.lib)
#create_req_link_target(absl bad_variant_access libabsl_bad_variant_access.a absl_bad_variant_access.lib)
#create_req_link_target(absl base libabsl_base.a absl_base.lib)
#create_req_link_target(absl city libabsl_city.a absl_city.lib)
#create_req_link_target(absl civil_time libabsl_civil_time.a absl_civil_time.lib)
#create_req_link_target(absl cord libabsl_cord.a absl_cord.lib)
#create_req_link_target(absl cord_internal libabsl_cord_internal.a absl_cord_internal.lib)
#create_req_link_target(absl cordz_functions libabsl_cordz_functions.a absl_cordz_functions.lib)
#create_req_link_target(absl cordz_handle libabsl_cordz_handle.a absl_cordz_handle.lib)
#create_req_link_target(absl cordz_info libabsl_cordz_info.a absl_cordz_info.lib)
#create_req_link_target(absl cordz_sample_token libabsl_cordz_sample_token.a absl_cordz_sample_token.lib)
#create_req_link_target(absl debugging_internal libabsl_debugging_internal.a absl_debugging_internal.lib)
#create_req_link_target(absl demangle_internal libabsl_demangle_internal.a absl_demangle_internal.lib)
#create_req_link_target(absl examine_stack libabsl_examine_stack.a absl_examine_stack.lib)
#create_req_link_target(absl exponential_biased libabsl_exponential_biased.a absl_exponential_biased.lib)
#create_req_link_target(absl failure_signal_handler libabsl_failure_signal_handler.a absl_failure_signal_handler.lib)
#create_req_link_target(absl flags libabsl_flags.a absl_flags.lib)
#create_req_link_target(absl flags_commandlineflag libabsl_flags_commandlineflag.a absl_flags_commandlineflag.lib)
#create_req_link_target(absl flags_commandlineflag_internal libabsl_flags_commandlineflag_internal.a absl_flags_commandlineflag_internal.lib)
#create_req_link_target(absl flags_config libabsl_flags_config.a absl_flags_config.lib)
#create_req_link_target(absl flags_internal libabsl_flags_internal.a absl_flags_internal.lib)
#create_req_link_target(absl flags_marshalling libabsl_flags_marshalling.a absl_flags_marshalling.lib)
#create_req_link_target(absl flags_parse libabsl_flags_parse.a absl_flags_parse.lib)
#create_req_link_target(absl flags_private_handle_accessor libabsl_flags_private_handle_accessor.a absl_flags_private_handle_accessor.lib)
#create_req_link_target(absl flags_program_name libabsl_flags_program_name.a absl_flags_program_name.lib)
#create_req_link_target(absl flags_reflection libabsl_flags_reflection.a absl_flags_reflection.lib)
#create_req_link_target(absl flags_usage libabsl_flags_usage.a absl_flags_usage.lib)
#create_req_link_target(absl flags_usage_internal libabsl_flags_usage_internal.a absl_flags_usage_internal.lib)
#create_req_link_target(absl graphcycles_internal libabsl_graphcycles_internal.a absl_graphcycles_internal.lib)
#create_req_link_target(absl hash libabsl_hash.a absl_hash.lib)
#create_req_link_target(absl hashtablez_sampler libabsl_hashtablez_sampler.a absl_hashtablez_sampler.lib)
#create_req_link_target(absl int128 libabsl_int128.a absl_int128.lib)
#create_req_link_target(absl leak_check libabsl_leak_check.a absl_leak_check.lib)
#create_req_link_target(absl log_severity libabsl_log_severity.a absl_log_severity.lib)
#create_req_link_target(absl low_level_hash libabsl_low_level_hash.a absl_low_level_hash.lib)
#create_req_link_target(absl malloc_internal libabsl_malloc_internal.a absl_malloc_internal.lib)
#create_req_link_target(absl periodic_sampler libabsl_periodic_sampler.a absl_periodic_sampler.lib)
#create_req_link_target(absl random_distributions libabsl_random_distributions.a absl_random_distributions.lib)
#create_req_link_target(absl random_internal_distribution_test_util libabsl_random_internal_distribution_test_util.a absl_random_internal_distribution_test_util.lib)
#create_req_link_target(absl random_internal_platform libabsl_random_internal_platform.a absl_random_internal_platform.lib)
#create_req_link_target(absl random_internal_pool_urbg libabsl_random_internal_pool_urbg.a absl_random_internal_pool_urbg.lib)
#create_req_link_target(absl random_internal_randen libabsl_random_internal_randen.a absl_random_internal_randen.lib)
#create_req_link_target(absl random_internal_randen_hwaes libabsl_random_internal_randen_hwaes.a absl_random_internal_randen_hwaes.lib)
#create_req_link_target(absl random_internal_randen_hwaes_impl libabsl_random_internal_randen_hwaes_impl.a absl_random_internal_randen_hwaes_impl.lib)
#create_req_link_target(absl random_internal_randen_slow libabsl_random_internal_randen_slow.a absl_random_internal_randen_slow.lib)
#create_req_link_target(absl random_internal_seed_material libabsl_random_internal_seed_material.a absl_random_internal_seed_material.lib)
#create_req_link_target(absl random_seed_gen_exception libabsl_random_seed_gen_exception.a absl_random_seed_gen_exception.lib)
#create_req_link_target(absl random_seed_sequences libabsl_random_seed_sequences.a absl_random_seed_sequences.lib)
#create_req_link_target(absl raw_hash_set libabsl_raw_hash_set.a absl_raw_hash_set.lib)
#create_req_link_target(absl raw_logging_internal libabsl_raw_logging_internal.a absl_raw_logging_internal.lib)
#create_req_link_target(absl scoped_set_env libabsl_scoped_set_env.a absl_scoped_set_env.lib)
#create_req_link_target(absl spinlock_wait libabsl_spinlock_wait.a absl_spinlock_wait.lib)
#create_req_link_target(absl stacktrace libabsl_stacktrace.a absl_stacktrace.lib)
#create_req_link_target(absl status libabsl_status.a absl_status.lib)
#create_req_link_target(absl statusor libabsl_statusor.a absl_statusor.lib)
#create_req_link_target(absl str_format_internal libabsl_str_format_internal.a absl_str_format_internal.lib)
#create_req_link_target(absl strerror libabsl_strerror.a absl_strerror.lib)
#create_req_link_target(absl strings libabsl_strings.a absl_strings.lib)
#create_req_link_target(absl strings_internal libabsl_strings_internal.a absl_strings_internal.lib)
#create_req_link_target(absl symbolize libabsl_symbolize.a absl_symbolize.lib)
#create_req_link_target(absl synchronization libabsl_synchronization.a absl_synchronization.lib)
#create_req_link_target(absl throw_delegate libabsl_throw_delegate.a absl_throw_delegate.lib)
#create_req_link_target(absl time libabsl_time.a absl_time.lib)
#create_req_link_target(absl time_zone libabsl_time_zone.a absl_time_zone.lib)
#create_external_package(absl
#        bad_any_cast_impl
#        bad_optional_access
#        bad_variant_access
#        base
#        city
#        civil_time
#        cord
#        cord_internal
#        cordz_functions
#        cordz_handle
#        cordz_info
#        cordz_sample_token
#        debugging_internal
#        demangle_internal
#        examine_stack
#        exponential_biased
#        failure_signal_handler
#        flags
#        flags_commandlineflag
#        flags_commandlineflag_internal
#        flags_config
#        flags_internal
#        flags_marshalling
#        flags_parse
#        flags_private_handle_accessor
#        flags_program_name
#        flags_reflection
#        flags_usage
#        flags_usage_internal
#        graphcycles_internal
#        hash
#        hashtablez_sampler
#        int128
#        leak_check
#        log_severity
#        low_level_hash
#        malloc_internal
#        periodic_sampler
#        random_distributions
#        random_internal_distribution_test_util
#        random_internal_platform
#        random_internal_pool_urbg
#        random_internal_randen
#        random_internal_randen_hwaes
#        random_internal_randen_hwaes_impl
#        random_internal_randen_slow
#        random_internal_seed_material
#        random_seed_gen_exception
#        random_seed_sequences
#        raw_hash_set
#        raw_logging_internal
#        scoped_set_env
#        spinlock_wait
#        stacktrace
#        status
#        statusor
#        str_format_internal
#        strerror
#        strings
#        strings_internal
#        symbolize
#        synchronization
#        throw_delegate
#        time
#        time_zone)
