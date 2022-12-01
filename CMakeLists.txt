cmake_minimum_required(VERSION 3.15...3.24)
project(hedera-sdk-cpp VERSION 0.1.0 DESCRIPTION "Hedera SDK C++" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 14)

set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/${CMAKE_HOST_SYSTEM_NAME}/${CMAKE_HOST_SYSTEM_PROCESSOR})
#message(WARNING "CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")

include(FetchContent)

find_package(upb REQUIRED)
find_package(Threads REQUIRED)

include(SystemLibraries.cmake)
include(HederaApi.cmake)

enable_testing()

add_subdirectory(sdk)
