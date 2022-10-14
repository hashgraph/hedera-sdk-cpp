include(FetchContent)

FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1)
FetchContent_MakeAvailable(googletest)

set(TestSources
        ED25519KeypairTest.cpp
        )

add_executable(${PROJECT_NAME}-tests ${TestSources})

target_link_libraries(${PROJECT_NAME}-tests
        PRIVATE
        gtest_main
        ${PROJECT_NAME})

add_test(
        NAME ${PROJECT_NAME}-tests
        COMMAND ${PROJECT_NAME}-tests)