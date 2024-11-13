add_subdirectory(sdk)
# Add optional TCK subdirectory build
option(BUILD_TCK "Build the TCK tests" OFF)

if(BUILD_TCK)
    message(STATUS "Including TCK tests in the build.")
    add_subdirectory(tck)
else()
    message(STATUS "TCK tests are not included in the build.")
endif()
