add_library(exp
  STATIC
    condition.c
    reservation.c
)

enable_testing()

add_executable(TestCondition condition.test.cpp)
target_link_libraries(TestCondition exp gtest_main)

include(GoogleTest)
gtest_discover_tests(TestCondition)
