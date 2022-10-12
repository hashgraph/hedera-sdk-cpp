cmake_minimum_required(VERSION 3.15...3.24)
project(hedera-sdk-cpp VERSION 0.1.0 DESCRIPTION "Hedera SDK C++" LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED 14)

set(CMAKE_INSTALL_PREFIX  ${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR})
message(WARNING "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

include(FetchContent)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

set(HAPI_LIBRARY_HASH "783876b3a61f38827acd798cda441e18e48fc09eeb5f00561c9fac5e2223f112" CACHE STRING "Use the configured hash to verify the Hedera API protobuf library release")
set(HAPI_LIBRARY_URL "https://github.com/hashgraph/hedera-protobufs-cpp/releases/download/v0.30.0/hapi-library-af52e908.tar.gz" CACHE STRING "Use the configured URL to download the Hedera API protobuf library package")

# Fetch the HAPI Library
FetchContent_Declare(
        hapi
        URL         ${HAPI_LIBRARY_URL}
        URL_HASH    SHA256=${HAPI_LIBRARY_HASH}
)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(hapi)

set(HAPI_INCLUDE_DIR ${hapi_SOURCE_DIR}/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR}/include)
set(HAPI_LIB_DIR ${hapi_SOURCE_DIR}/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR}/lib)
file(GLOB HAPI_LINK_TARGET ${HAPI_LIB_DIR}/*hapi.*)

message(WARNING "HAPI_INCLUDE_DIR=${HAPI_INCLUDE_DIR}")
message(WARNING "HAPI_LIB_DIR=${HAPI_LIB_DIR}")
message(WARNING "HAPI_LINK_TARGET=${HAPI_LINK_TARGET}")

add_library(hapi STATIC IMPORTED)
set_target_properties(hapi PROPERTIES IMPORTED_LOCATION ${HAPI_LINK_TARGET})

enable_testing()

add_subdirectory(sdk)
