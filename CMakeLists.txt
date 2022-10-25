cmake_minimum_required(VERSION 3.15...3.24)
project(hedera-sdk-cpp VERSION 0.1.0 DESCRIPTION "Hedera SDK C++" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 14)

set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR})
message(WARNING "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

include(FetchContent)

#find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

if (APPLE)
    if (NOT DEFINED OSX_CORE_FOUNDATION)
        find_library(OSX_CORE_FOUNDATION CoreFoundation)
    endif()

    if (NOT DEFINED OSX_CF_NETWORK)
        find_library(OSX_CF_NETWORK CFNetwork)
    endif()
endif()

include(HederaApi.cmake)

if (NOT DEFINED RESOLV_LIBRARY)
    find_library(RESOLV_LIBRARY resolv)
endif()

add_library(sys::resolv STATIC IMPORTED)
set_target_properties(sys::resolv PROPERTIES IMPORTED_LOCATION ${RESOLV_LIBRARY})

#
# Set the HAPI_LINK_TARGET
#
#set(HAPI_LINK_TARGET ${HAPI_LIB_DIR}/libhapi.a)
#if (NOT EXISTS ${HAPI_LINK_TARGET})
#    set(HAPI_LINK_TARGET ${HAPI_LIB_DIR}/hapi.lib)
#
#    if (NOT EXISTS ${HAPI_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the HAPI_LINK_TARGET at `${HAPI_LIB_DIR}/(libhapi.a|hapi.lib)`")
#    endif ()
#endif ()



#set(ABSL_BASE_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_base.a)
#
#if (NOT EXISTS ${ABSL_BASE_LINK_TARGET})
#    set(ABSL_BASE_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_base.lib)
#
#    if (NOT EXISTS ${ABSL_BASE_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the ABSL_BASE_LINK_TARGET at `${HAPI_LIB_DIR}/(libabsl_base.a|libabsl_base.lib)`")
#    endif ()
#endif ()
#
#set(ABSL_FLAGS_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_flags.a)
#
#if (NOT EXISTS ${ABSL_FLAGS_LINK_TARGET})
#    set(ABSL_FLAGS_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_flags.lib)
#
#    if (NOT EXISTS ${ABSL_FLAGS_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the ABSL_FLAGS_LINK_TARGET at `${HAPI_LIB_DIR}/(libabsl_flags.a|libabsl_flags.lib)`")
#    endif ()
#endif ()
#
#set(ABSL_HASH_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_hash.a)
#
#if (NOT EXISTS ${ABSL_HASH_LINK_TARGET})
#    set(ABSL_HASH_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_hash.lib)
#
#    if (NOT EXISTS ${ABSL_HASH_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the ABSL_HASH_LINK_TARGET at `${HAPI_LIB_DIR}/(libabsl_hash.a|libabsl_hash.lib)`")
#    endif ()
#endif ()
#
#set(ABSL_STATUS_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_status.a)
#
#if (NOT EXISTS ${ABSL_STATUS_LINK_TARGET})
#    set(ABSL_STATUS_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_status.lib)
#
#    if (NOT EXISTS ${ABSL_STATUS_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the ABSL_STATUS_LINK_TARGET at `${HAPI_LIB_DIR}/(libabsl_status.a|libabsl_status.lib)`")
#    endif ()
#endif ()
#
#set(ABSL_STRINGS_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_strings.a)
#
#if (NOT EXISTS ${ABSL_STRINGS_LINK_TARGET})
#    set(ABSL_STRINGS_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_strings.lib)
#
#    if (NOT EXISTS ${ABSL_STRINGS_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the ABSL_STRINGS_LINK_TARGET at `${HAPI_LIB_DIR}/(libabsl_strings.a|libabsl_strings.lib)`")
#    endif ()
#endif ()
#
#set(ABSL_SYNCHRONIZATION_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_synchronization.a)
#
#if (NOT EXISTS ${ABSL_SYNCHRONIZATION_LINK_TARGET})
#    set(ABSL_SYNCHRONIZATION_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_synchronization.lib)
#
#    if (NOT EXISTS ${ABSL_SYNCHRONIZATION_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the ABSL_SYNCHRONIZATION_LINK_TARGET at `${HAPI_LIB_DIR}/(libabsl_synchronization.a|libabsl_synchronization.lib)`")
#    endif ()
#endif ()
#
#set(ABSL_TIME_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_time.a)
#
#if (NOT EXISTS ${ABSL_TIME_LINK_TARGET})
#    set(ABSL_TIME_LINK_TARGET ${HAPI_LIB_DIR}/libabsl_time.lib)
#
#    if (NOT EXISTS ${ABSL_TIME_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the ABSL_TIME_LINK_TARGET at `${HAPI_LIB_DIR}/(libabsl_time.a|libabsl_time.lib)`")
#    endif ()
#endif ()
#
#set(C_ARES_LINK_TARGET ${HAPI_LIB_DIR}/libcares.a)
#
#if (NOT EXISTS ${C_ARES_LINK_TARGET})
#    set(CARES_LINK_TARGET ${HAPI_LIB_DIR}/libcares.lib)
#
#    if (NOT EXISTS ${C_ARES_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the C_ARES_LINK_TARGET at `${HAPI_LIB_DIR}/(libcares.a|libcares.lib)`")
#    endif ()
#endif ()
#
#set(OPENSSL_CRYPTO_LINK_TARGET ${HAPI_LIB_DIR}/libcrypto.a)
#
#if (NOT EXISTS ${OPENSSL_CRYPTO_LINK_TARGET})
#    set(OPENSSL_CRYPTO_LINK_TARGET ${HAPI_LIB_DIR}/libcrypto.lib)
#
#    if (NOT EXISTS ${OPENSSL_CRYPTO_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the OPENSSL_CRYPTO_LINK_TARGET at `${HAPI_LIB_DIR}/(libcrypto.a|libcrypto.lib)`")
#    endif ()
#endif ()
#
#set(GRPC_CPP_LINK_TARGET ${HAPI_LIB_DIR}/libgrpc++.a)
#
#if (NOT EXISTS ${GRPC_CPP_LINK_TARGET})
#    set(GRPC_CPP_LINK_TARGET ${HAPI_LIB_DIR}/grpc++.lib)
#
#    if (NOT EXISTS ${GRPC_CPP_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the GRPC_CPP_LINK_TARGET at `${HAPI_LIB_DIR}/(libgrpc++.a|grpc++.lib)`")
#    endif ()
#endif ()
#
#set(GRPC_LINK_TARGET ${HAPI_LIB_DIR}/libgrpc.a)
#
#if (NOT EXISTS ${GRPC_LINK_TARGET})
#    set(GRPC_LINK_TARGET ${HAPI_LIB_DIR}/grpc.lib)
#
#    if (NOT EXISTS ${GRPC_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the GRPC_LINK_TARGET at `${HAPI_LIB_DIR}/(libgrpc.a|grpc.lib)`")
#    endif ()
#endif ()
#
#set(PROTOBUF_LINK_TARGET ${HAPI_LIB_DIR}/libprotobuf.a)
#
#if (NOT EXISTS ${PROTOBUF_LINK_TARGET})
#    set(PROTOBUF_LINK_TARGET ${HAPI_LIB_DIR}/libprotobuf.lib)
#
#    if (NOT EXISTS ${PROTOBUF_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the PROTOBUF_LINK_TARGET at `${HAPI_LIB_DIR}/(libprotobuf.a|libprotobuf.lib)`")
#    endif ()
#endif ()
#
#set(RE2_LINK_TARGET ${HAPI_LIB_DIR}/libre2.a)
#
#if (NOT EXISTS ${RE2_LINK_TARGET})
#    set(RE2_LINK_TARGET ${HAPI_LIB_DIR}/libre2.lib)
#
#    if (NOT EXISTS ${RE2_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the RE2_LINK_TARGET at `${HAPI_LIB_DIR}/(libre2.a|libre2.lib)`")
#    endif ()
#endif ()
#
#set(OPENSSL_SSL_LINK_TARGET ${HAPI_LIB_DIR}/libssl.a)
#
#if (NOT EXISTS ${OPENSSL_SSL_LINK_TARGET})
#    set(OPENSSL_SSL_LINK_TARGET ${HAPI_LIB_DIR}/libssl.lib)
#
#    if (NOT EXISTS ${OPENSSL_SSL_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the OPENSSL_SSL_LINK_TARGET at `${HAPI_LIB_DIR}/(libssl.a|libssl.lib)`")
#    endif ()
#endif ()
#
#set(UPB_LINK_TARGET ${HAPI_LIB_DIR}/libupb.a)
#
#if (NOT EXISTS ${UPB_LINK_TARGET})
#    set(UPB_LINK_TARGET ${HAPI_LIB_DIR}/libupb.lib)
#
#    if (NOT EXISTS ${UPB_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the UPB_LINK_TARGET at `${HAPI_LIB_DIR}/(libupb.a|libupb.lib)`")
#    endif ()
#endif ()
#
#set(ZLIB_LINK_TARGET ${HAPI_LIB_DIR}/libz.a)
#
#if (NOT EXISTS ${ZLIB_LINK_TARGET})
#    set(ZLIB_LINK_TARGET ${HAPI_LIB_DIR}/libz.lib)
#
#    if (NOT EXISTS ${ZLIB_LINK_TARGET})
#        message(FATAL_ERROR "Failed to locate the ZLIB_LINK_TARGET at `${HAPI_LIB_DIR}/(libz.a|libz.lib)`")
#    endif ()
#endif ()

#message(WARNING "HAPI_INCLUDE_DIR=${HAPI_INCLUDE_DIR}")
#message(WARNING "HAPI_LIB_DIR=${HAPI_LIB_DIR}")
#message(WARNING "HAPI_LINK_TARGET=${HAPI_LINK_TARGET}")

#add_library(hapi STATIC IMPORTED)
#set_target_properties(hapi PROPERTIES IMPORTED_LOCATION ${HAPI_LINK_TARGET})

#add_library(protobuf::protobuf STATIC IMPORTED)
#set_target_properties(protobuf::protobuf PROPERTIES IMPORTED_LOCATION ${PROTOBUF_LINK_TARGET})
#
#add_library(gRPC::grpc STATIC IMPORTED)
#add_library(gRPC::grpc++ STATIC IMPORTED)
#set_target_properties(gRPC::grpc PROPERTIES IMPORTED_LOCATION ${GRPC_LINK_TARGET})
#set_target_properties(gRPC::grpc++ PROPERTIES IMPORTED_LOCATION ${GRPC_CPP_LINK_TARGET})
#
#
#add_library(OpenSSL::Crypto STATIC IMPORTED)
#add_library(OpenSSL::SSL STATIC IMPORTED)
#set_target_properties(OpenSSL::Crypto PROPERTIES IMPORTED_LOCATION ${OPENSSL_CRYPTO_LINK_TARGET})
#set_target_properties(OpenSSL::SSL PROPERTIES IMPORTED_LOCATION ${OPENSSL_SSL_LINK_TARGET})
#
#add_library(abseil::base STATIC IMPORTED)
#add_library(abseil::flags STATIC IMPORTED)
#add_library(abseil::hash STATIC IMPORTED)
#add_library(abseil::status STATIC IMPORTED)
#add_library(abseil::strings STATIC IMPORTED)
#add_library(abseil::synchronization STATIC IMPORTED)
#add_library(abseil::time STATIC IMPORTED)
#set_target_properties(abseil::base PROPERTIES IMPORTED_LOCATION ${ABSL_BASE_LINK_TARGET})
#set_target_properties(abseil::flags PROPERTIES IMPORTED_LOCATION ${ABSL_FLAGS_LINK_TARGET})
#set_target_properties(abseil::hash PROPERTIES IMPORTED_LOCATION ${ABSL_HASH_LINK_TARGET})
#set_target_properties(abseil::status PROPERTIES IMPORTED_LOCATION ${ABSL_STATUS_LINK_TARGET})
#set_target_properties(abseil::strings PROPERTIES IMPORTED_LOCATION ${ABSL_STRINGS_LINK_TARGET})
#set_target_properties(abseil::synchronization PROPERTIES IMPORTED_LOCATION ${ABSL_SYNCHRONIZATION_LINK_TARGET})
#set_target_properties(abseil::time PROPERTIES IMPORTED_LOCATION ${ABSL_TIME_LINK_TARGET})
#
#add_library(c-ares STATIC IMPORTED)
#set_target_properties(c-ares PROPERTIES IMPORTED_LOCATION ${C_ARES_LINK_TARGET})
#
#add_library(re2 STATIC IMPORTED)
#add_library(upb STATIC IMPORTED)
#set_target_properties(re2 PROPERTIES IMPORTED_LOCATION ${RE2_LINK_TARGET})
#set_target_properties(upb PROPERTIES IMPORTED_LOCATION ${UPB_LINK_TARGET})
#
#add_library(zlib STATIC IMPORTED)
#set_target_properties(zlib PROPERTIES IMPORTED_LOCATION ${ZLIB_LINK_TARGET})

enable_testing()

add_subdirectory(sdk)
