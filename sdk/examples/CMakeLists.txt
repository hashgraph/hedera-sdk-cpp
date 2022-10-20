set(EXAMPLE_NAME ${PROJECT_NAME}-blade-example)

add_executable(${EXAMPLE_NAME} BladeExample.cpp)

target_link_libraries(${EXAMPLE_NAME} PRIVATE gtest_main ${PROJECT_NAME})
