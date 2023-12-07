FetchContent_Declare(
        json-rpc-cxx
        GIT_REPOSITORY https://github.com/jsonrpcx/json-rpc-cxx
        GIT_TAG v0.3.1
)
set(COMPILE_TESTS OFF)
set(COMPILE_EXAMPLES OFF)
FetchContent_MakeAvailable(json-rpc-cxx)

set(TCK_SERVER_NAME ${PROJECT_NAME}-tck)
add_executable(${TCK_SERVER_NAME}
        src/main.cpp)

target_link_libraries(${TCK_SERVER_NAME} ${PROJECT_NAME})

target_include_directories(${TCK_SERVER_NAME} PRIVATE ${json-rpc-cxx_SOURCE_DIR}/include)

install(TARGETS ${TCK_SERVER_NAME} RUNTIME DESTINATION tck)
