set(TCK_SERVER_NAME ${PROJECT_NAME}-tck)
add_executable(${TCK_SERVER_NAME}
        src/JsonRpcException.cc
        src/JsonUtils.cc
        src/main.cc
        src/SdkClient.cc
        src/TckServer.cc)

target_link_libraries(${TCK_SERVER_NAME} ${PROJECT_NAME})

target_include_directories(${TCK_SERVER_NAME} PRIVATE include)
target_include_directories(${TCK_SERVER_NAME} PRIVATE ${json-rpc-cxx_SOURCE_DIR}/include)

install(TARGETS ${TCK_SERVER_NAME} RUNTIME DESTINATION tck)
