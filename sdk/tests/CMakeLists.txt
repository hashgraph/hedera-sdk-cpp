include(FetchContent)

FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1)
FetchContent_MakeAvailable(googletest)

set(TestSources
        ED25519PrivateKeyTest.cc
        ED25519PublicKeyTest.cc
        ExchangeRateTest.cc
        HbarTest.cc
        )

set(TEST_PROJECT_NAME ${PROJECT_NAME}-tests)

add_executable(${TEST_PROJECT_NAME} ${TestSources})

target_link_libraries(${TEST_PROJECT_NAME}
        PRIVATE
        gtest_main
        ${PROJECT_NAME})

add_test(
        NAME ${TEST_PROJECT_NAME}
        COMMAND ${TEST_PROJECT_NAME})