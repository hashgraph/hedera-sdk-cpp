cmake_minimum_required(VERSION 3.15...3.24)
project(hedera-sdk-cpp VERSION 0.1.0 DESCRIPTION "Hedera SDK C++" LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED 14)

set(CMAKE_INSTALL_PREFIX  ${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR})
message(WARNING "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

include(FetchContent)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

set(HAPI_LIBRARY_HASH "959526156e2ac9fc07b6592997cf03f71bba83b3226824cebb5725e67af5bbb4" CACHE STRING "Use the configured hash to verify the Hedera API protobuf library release")
set(HAPI_LIBRARY_URL "https://github.com/hashgraph/hedera-protobufs-cpp/releases/download/v0.30.0/hapi-library-716ff263.tar.gz" CACHE STRING "Use the configured URL to download the Hedera API protobuf library package")

# Fetch the HAPI Library
FetchContent_Declare(
        hapi
        URL         ${HAPI_LIBRARY_URL}
        URL_HASH    SHA256=${HAPI_LIBRARY_HASH}
)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(hapi)

set(HAPI_ROOT_DIR ${hapi_SOURCE_DIR}/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR})

if (NOT EXISTS ${HAPI_ROOT_DIR})
    set(HAPI_ROOT_DIR ${hapi_SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR})

    if (NOT EXISTS ${HAPI_ROOT_DIR})
        message(FATAL_ERROR "Failed to the HAPI_ROOT_DIR at `${HAPI_ROOT_DIR}`")
    endif()
endif()

set(HAPI_INCLUDE_DIR ${HAPI_ROOT_DIR}/include)
set(HAPI_LIB_DIR ${HAPI_ROOT_DIR}/lib)

set(HAPI_LINK_TARGET ${HAPI_LIB_DIR}/libhapi.a)

if (NOT EXISTS ${HAPI_LINK_TARGET})
    set(HAPI_LINK_TARGET ${HAPI_LIB_DIR}/hapi.lib)

    if (NOT EXISTS ${HAPI_LINK_TARGET})
        message(FATAL_ERROR "Failed to locate the HAPI_LINK_TARGET at `${HAPI_LIB_DIR}/(libhapi.a|hapi.lib)`")
    endif()
endif()

set(PROTOBUF_LINK_TARGET ${HAPI_LIB_DIR}/libprotobuf.a)

if (NOT EXISTS ${PROTOBUF_LINK_TARGET})
    set (PROTOBUF_LINK_TARGET ${HAPI_LIB_DIR}/protobuf.lib)

    if (NOT EXISTS ${PROTOBUF_LINK_TARGET})
        message(FATAL_ERROR "Failed to locate the PROTOBUF_LINK_TARGET at `${HAPI_LIB_DIR}/(libprotobuf.a|protobuf.lib)`")
    endif()
endif()

set(GRPC_LINK_TARGET ${HAPI_LIB_DIR}/libgrpc.a)

if (NOT EXISTS ${GRPC_LINK_TARGET})
    set (GRPC_LINK_TARGET ${HAPI_LIB_DIR}/grpc.lib)

    if (NOT EXISTS ${GRPC_LINK_TARGET})
        message(FATAL_ERROR "Failed to locate the GRPC_LINK_TARGET at `${HAPI_LIB_DIR}/(libgrpc.a|grpc.lib)`")
    endif()
endif()

set(GRPC_CPP_LINK_TARGET ${HAPI_LIB_DIR}/libgrpc++.a)

if (NOT EXISTS ${GRPC_CPP_LINK_TARGET})
    set (GRPC_CPP_LINK_TARGET ${HAPI_LIB_DIR}/grpc++.lib)

    if (NOT EXISTS ${GRPC_CPP_LINK_TARGET})
        message(FATAL_ERROR "Failed to locate the GRPC_CPP_LINK_TARGET at `${HAPI_LIB_DIR}/(libgrpc++.a|grpc++.lib)`")
    endif()
endif()

#message(WARNING "HAPI_INCLUDE_DIR=${HAPI_INCLUDE_DIR}")
#message(WARNING "HAPI_LIB_DIR=${HAPI_LIB_DIR}")
#message(WARNING "HAPI_LINK_TARGET=${HAPI_LINK_TARGET}")

add_library(hapi STATIC IMPORTED)
set_target_properties(hapi PROPERTIES IMPORTED_LOCATION ${HAPI_LINK_TARGET})

add_library(protobuf::protobuf STATIC IMPORTED)
set_target_properties(protobuf::protobuf PROPERTIES IMPORTED_LOCATION ${PROTOBUF_LINK_TARGET})

add_library(gRPC::grpc STATIC IMPORTED)
set_target_properties(gRPC::grpc PROPERTIES IMPORTED_LOCATION ${GRPC_LINK_TARGET})

add_library(gRPC::grpc++ STATIC IMPORTED)
set_target_properties(gRPC::grpc++ PROPERTIES IMPORTED_LOCATION ${GRPC_CPP_LINK_TARGET})

enable_testing()

add_subdirectory(sdk)
