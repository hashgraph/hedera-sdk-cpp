if (NOT LINUX)
    include(FetchContent)
    FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1)

    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    include(googletest)
else ()
    find_package(GTest REQUIRED)
endif ()

add_subdirectory(unit)
add_subdirectory(integration)
