include(FetchContent)

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1)
FetchContent_MakeAvailable(googletest)

#add_executable(sdk_test test.cpp)

#target_link_libraries(sdk_test
#    PRIVATE
#    gtest_main
#    ${PROJECT_NAME})

#add_test(test sdk_test)