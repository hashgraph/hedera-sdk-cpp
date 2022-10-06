cmake_minimum_required(VERSION 2.8...3.24)
project(hedera-sdk-cpp VERSION 0.1.0 DESCRIPTION "Hedera SDK C++" LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED 11)

enable_testing()

add_subdirectory(sdk)
