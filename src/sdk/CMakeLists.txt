add_subdirectory(examples)
add_subdirectory(main)
# Optional Tests build
option(BUILD_TESTS "Build the test suite" OFF)

if(BUILD_TESTS)
    message(STATUS "Including tests in the build.")
    add_subdirectory(tests)
else()
    message(STATUS "Tests are not included in the build.")
endif()
